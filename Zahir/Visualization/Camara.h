#ifndef CAMARA_H_
#define CAMARA_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include "../Models/Modelo3D.h"


// TODO: constructor copia, operator =
// TODO: esta clase debiese estar en otro lado, o ser una implementación para OpenGL de una interfaz de cámara.

class Camara
{
public:
	Camara();
	//TODO: mover esto al graficador.
	void posicionarEnEscena(){
		if (definido_modelo)
			gluLookAt( posicion_camara[0], posicion_camara[1], posicion_camara[2], modelo_enfocado->centroid()[0], modelo_enfocado->centroid()[1], modelo_enfocado->centroid()[2], 0.0, 1.0, 0.0);
		else 
			gluLookAt( 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	};
	void asignarModelo3D(Modelo3D* modelo);
	OpenMesh::Vec4f& obtenerPosicion();
	OpenMesh::Vec4f& obtenerPosicionTransformada();
	void definirPosicionTransformada(OpenMesh::Vec4f& p);
	virtual ~Camara();
	
private:
	OpenMesh::Vec4f posicion_camara;
	OpenMesh::Vec4f posicion_transformada;
	Modelo3D* modelo_enfocado;
	bool definido_modelo;
	
};

#endif /*CAMARA_H_*/
