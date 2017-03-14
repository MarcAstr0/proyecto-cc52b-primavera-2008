#include "Camara.h"

Camara::Camara() {
	definido_modelo = false;
}

Camara::~Camara() {}

OpenMesh::Vec4f& Camara::obtenerPosicion() {
	return posicion_camara;
}

void Camara::definirPosicionTransformada(OpenMesh::Vec4f& p) {
	posicion_transformada = p;
}

OpenMesh::Vec4f& Camara::obtenerPosicionTransformada() {
	return posicion_transformada;
}

void Camara::asignarModelo3D(Modelo3D* modelo) {
	modelo_enfocado = modelo;
	definido_modelo = true;
	posicion_camara[0] = modelo->centroid()[0];
	posicion_camara[1] = modelo->centroid()[1];
	posicion_camara[2] = modelo->centroid()[2] + 2.5f * modelo->boundingSphereRatio();
	posicion_camara[3] = 1.0f;
}
