#ifndef CARGADORMODELO3D_H_
#define CARGADORMODELO3D_H_

#include <string>
#include <vector>
#include <map>
#include "../Modelo3D.h"
#include "../../Visualization/Material.h"
#include "../../Visualization/Texturas/Textura.h"
#include "../../Visualization/Texturas/CargadorTexturas.h"


class CargadorModelo3D {
public:
	CargadorModelo3D(Modelo3D* modelo, std::string filename){
		a_cargar = modelo;
		nombre_de_archivo = filename;
	};
	virtual ~CargadorModelo3D() {};
	void cargarDatos() {
		loadData(a_cargar->_mesh, a_cargar->materiales);
		verificarIntegridad();
		finishLoad();
		a_cargar->definirVariablesInternas();
	};
	
/* abstract methods */
protected:
	virtual void loadData(ZahirMesh& mesh, std::vector<Material*> &materiales)=0;
	
/* non-abstract methods */
protected:
	virtual void finishLoad() {};
	virtual void verificarIntegridad() {};
	void setTexCoord(ZahirMesh::FaceHandle& face, int vertex_index[], int vertex_texture[], const std::vector<ZahirMesh::TexCoord2D>& texcoords) {
		assert(vertex_index[0] >= 0 && vertex_index[1] >= 0 && vertex_index[2] >= 0);
		assert(vertex_texture[0] >= 0 && vertex_texture[1] >= 0 && vertex_texture[2] >= 0);
		
		ZahirMesh::FaceHalfedgeIter he_it = a_cargar->_mesh.fh_iter(face);
		int he_vt = 0;
		for (int i = 0; i < 3; i++) {
			// aquí debo encontrar el vértice que le corresponde a esta half edge. luego meto la propiedad en la malla
			
			// using: VertexHandle from_vertex_handle(HalfedgeHandle _heh) const (from ArrayKernel)
			//mesh.from_vertex_handle( he_it.handle()).idx() --> gives me the index of the starting vertex of the halfedge
			
			if (vertex_index[0] == a_cargar->_mesh.from_vertex_handle( he_it.handle()).idx())
				he_vt = vertex_texture[0];
			else if (vertex_index[1] == a_cargar->_mesh.from_vertex_handle( he_it.handle() ).idx())
				he_vt = vertex_texture[1];
			else
				he_vt = vertex_texture[2];
			
			assert(((unsigned int) he_vt) < texcoords.size());
			
			a_cargar->_mesh.property(a_cargar->_half_edge_texcoords, he_it) = texcoords[he_vt];
			++he_it;
		}
	};
	
	Modelo3D* a_cargar;
	std::string nombre_de_archivo;
};

#endif /*CARGADORMODELO3D_H_*/
