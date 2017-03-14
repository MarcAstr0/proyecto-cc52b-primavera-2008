#include "Modelo3D.h"
#include <iostream>
#include "Cargadores/CargadorOBJ.h"
//#include "Algoritmos/EstimadorCurvatura.h"
//#include "Algoritmos/EstimadorDeNormales.h"

#include "Operations/CurvatureEstimator.h"

#include "../Visualization/Texturas/Textura.h"


Modelo3D::Modelo3D() {
	nombre_de_archivo = "";	
	radio_esfera = 3.0f;
	largo_promedio_arista = largo_caracteristico = 1.0f;
	normales_suavizadas = false;
	esfera_calculada = true;
	
	//curvatura_estimada = derivada_curvatura_estimada = false;

	materiales = std::vector<Material*>();
	
	_mesh.request_face_normals();
	_mesh.request_vertex_normals();
	_mesh.add_property(_half_edge_texcoords,  "vertex_texcoords");
	
	has_curvature = false;

}

void Modelo3D::definirVariablesInternas() {
	if (materiales.size() == 0){
		materiales.resize(1);
		materiales[0] = new Material();
	}
	
	normales_suavizadas = false;
	_mesh.update_face_normals();
	// Calculamos datos auxiliares
	esfera_calculada = false;
	calcularEsferaContenedora();
	
	//curvatura_estimada = derivada_curvatura_estimada = false;
	// TODO: rehacer la clase 1-Ring algorithm. por el momento ...
	_mesh.update_vertex_normals();
	
	requestCurvature();
	
}

Modelo3D::~Modelo3D() {
	for (unsigned int i = 0; i < materiales.size(); i++)
		delete materiales.at(i);
}

const OpenMesh::Vec3f& Modelo3D::centroid() const {
	return _centroid;
}

float Modelo3D::boundingSphereRatio() const {
	return radio_esfera;
}

void Modelo3D::requestCurvature() {
	if (!has_curvature) {
		_mesh.add_property(_vertex_curvature_1, "vertex_curvature_1");
		_mesh.add_property(_vertex_curvature_2, "vertex_curvature_2");
		_mesh.add_property(_vertex_pdir_1, "vertex_pdir_1");
		_mesh.add_property(_vertex_pdir_2, "vertex_pdir_2");
		_mesh.add_property(_vertex_curv_deriv, "vertex_curvature_derivatives");
		
		CurvatureEstimator ce(_mesh, _vertex_curvature_1, _vertex_curvature_2, _vertex_pdir_1, _vertex_pdir_2, _vertex_curv_deriv);
		ce.calculate();
		has_curvature = true;
	}
} 

float Modelo3D::cv1(const ZahirMesh::VertexIter& vi) const {
	assert(has_curvature);
	return _mesh.property(_vertex_curvature_1, vi);
}

float Modelo3D::cv2(const ZahirMesh::VertexIter& vi) const {
	assert(has_curvature);
	return _mesh.property(_vertex_curvature_2, vi);
}

const OpenMesh::Vec3f& Modelo3D::pdir1(const ZahirMesh::VertexIter& vi) const {
	assert(has_curvature);
	return _mesh.property(_vertex_pdir_1, vi);	
}

const OpenMesh::Vec3f& Modelo3D::pdir2(const ZahirMesh::VertexIter& vi) const {
	assert(has_curvature);
	return _mesh.property(_vertex_pdir_2, vi);	
}

void Modelo3D::calcularEsferaContenedora(){

	ZahirMesh::VertexIter	v_it, v_end(_mesh.vertices_end());
	
	_centroid = OpenMesh::Vec3f(0.0f, 0.0f, 0.0f);
	
	for (v_it = _mesh.vertices_begin(); v_it != v_end; ++v_it)
		_centroid += _mesh.point(v_it);	
	
	_centroid *= (1.0f/_mesh.n_vertices());
	
	radio_esfera = -1.0f; 

	float distance;
	for (v_it = _mesh.vertices_begin(); v_it != v_end; ++v_it) {
		OpenMesh::Vec3f pos = _centroid - _mesh.point(v_it);

		distance = sqrt(pos | pos);
		if (distance > radio_esfera) 
			radio_esfera = distance;
	}
}

void Modelo3D::calcularLargoCaracteristico(){
	// TODO: por ahora tiene un valor cualquiera, hay que definir esto.
	largo_caracteristico = radio_esfera * 0.1f;
}

float Modelo3D::largoCaracteristico() const {
//	std::cout << "largo caracteristico: " << largo_caracteristico << "\n";
	return largo_caracteristico;
}

unsigned int Modelo3D::numeroDePuntos() const { 
	return _mesh.n_vertices(); 
}

unsigned int Modelo3D::numeroDeTriangulos() const {  
	return _mesh.n_faces();
}


//void Modelo3D::colorize(MeshColorizer* colorizer) {
//	assert(colorizer != NULL);
//	if (!has_vertex_colors)
//		_mesh.add_property(_vertex_colors, "vertex colors");
//	//TODO: MeshColorizer :D
//}

bool Modelo3D::hasTexture() const {
	return materiales[0]->definidaTextura();
}
