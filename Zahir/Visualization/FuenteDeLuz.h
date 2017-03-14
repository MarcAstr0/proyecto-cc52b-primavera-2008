#ifndef FUENTEDELUZ_H_
#define FUENTEDELUZ_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include "Color.h"
#include <OpenMesh/Core/Geometry/VectorT.hh>

//TODO: rehacer completamente esta clase.

class FuenteDeLuz
{
public:
	FuenteDeLuz(GLenum id = GL_LIGHT0);
	void entregarValores();
	void entregarIntensidades();
	void prender();
	void apagar();
	void definirPosicion(OpenMesh::Vec4f& p);
	void definirSpotlight(OpenMesh::Vec3f& d, float cutoff);
	void definirIntensidadAmbiental(float r, float g, float b);
	void definirIntensidadDifusa(float r, float g, float b);
	void definirIntensidadEspecular(float r, float g, float b);
	const OpenMesh::Vec4f& obtenerPosicion() const;
	const OpenMesh::Vec4f& obtenerPosicionTransformada() const;
	void definirPosicionTransformada(OpenMesh::Vec4f& p);
	virtual ~FuenteDeLuz();
	
private:
	GLenum identificador;
	OpenMesh::Vec4f posicion;
	OpenMesh::Vec4f posicion_transformada;
	OpenMesh::Vec3f direction;
	Color* I_a;
	Color* I_d;
	Color* I_s;
	float angulo; /* en caso de ser una linterna */
	bool es_spotlight;
	bool definido_ambiental;
	bool definido_difuso;
	bool definido_especular;
	bool tiene_direccion;
	bool tiene_posicion;
	
};

#endif /*FUENTEDELUZ_H_*/
