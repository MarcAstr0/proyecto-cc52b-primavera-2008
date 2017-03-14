#ifndef MODELO3D_H_
#define MODELO3D_H_

#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <list>

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> ZahirMesh;

#include "../Visualization/Material.h"
#include "../Visualization/Texturas/Textura.h"

/* declarations for friend classes */

class CargadorModelo3D;
class SurfaceRenderer;
class ModelFactory;
class SurfaceCurvature; //TODO: quitar esta clase de aquí

//class MeshColorizer;


class Modelo3D {
public:
	bool hasTexture() const;
	
	const OpenMesh::Vec3f& centroid() const;
	
	float boundingSphereRatio() const;
	
	float radioEsferaContenedora();

	float largoPromedioAristas() const;
	float largoCaracteristico() const;
	
	unsigned int numeroDePuntos() const;
	unsigned int numeroDeTriangulos() const;
	virtual ~Modelo3D();
	
//	virtual void colorize(MeshColorizer* colorizer);
	
	virtual void requestCurvature();
	virtual float cv1(const ZahirMesh::VertexIter& vi) const;
	virtual float cv2(const ZahirMesh::VertexIter& vi) const;
	const OpenMesh::Vec3f& pdir1(const ZahirMesh::VertexIter& vi) const;
	const OpenMesh::Vec3f& pdir2(const ZahirMesh::VertexIter& vi) const;

	friend class CargadorModelo3D;
	friend class SurfaceRenderer;
	friend class SurfaceCurvature;
	friend class ModelFactory;

protected:
	Modelo3D();
	void calcularEsferaContenedora();
	void calcularLargoCaracteristico();
	virtual void definirVariablesInternas();

	OpenMesh::Vec3f _centroid;
	
	float radio_esfera;

	float largo_caracteristico;
	float largo_promedio_arista;

	bool normales_suavizadas;
	bool esfera_calculada;
	bool has_curvature;

	// Elementos base

	std::string nombre_de_archivo;

	/// Materials
	std::vector<Material*> materiales;

	// información de vecindad y asociación

	ZahirMesh _mesh;
	OpenMesh::HPropHandleT< ZahirMesh::TexCoord2D > _half_edge_texcoords;
	OpenMesh::VPropHandleT< float > _vertex_curvature_1;
	OpenMesh::VPropHandleT< float > _vertex_curvature_2;
	OpenMesh::VPropHandleT< OpenMesh::Vec3f > _vertex_pdir_1;
	OpenMesh::VPropHandleT< OpenMesh::Vec3f > _vertex_pdir_2;
	OpenMesh::VPropHandleT< OpenMesh::Vec4f > _vertex_curv_deriv;
	
	// optional mesh attributes (can be handled by other classes)
	
//	bool has_vertex_colors;
//	OpenMesh::VPropHandleT< OpenMesh::Vec3f > _vertex_colors;
	
	bool has_vertex_texture;
	bool has_vertex_colors;
	bool has_vertex_normals;
	bool has_vertex_curvature;
	bool has_vertex_principal_directions;
	bool has_vertex_curvature_derivatives;
	
};

#endif /*MODELO3D_H_*/
