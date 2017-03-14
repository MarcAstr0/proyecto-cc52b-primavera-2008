#include "FuenteDeLuz.h"

FuenteDeLuz::FuenteDeLuz(GLenum id){
	identificador = id;
	definido_ambiental = definido_difuso = definido_especular = false;
	es_spotlight = false;
	angulo = 180.0f;
	tiene_posicion = false;
}

void FuenteDeLuz::entregarValores(){
	if (tiene_posicion) 
		glLightfv(identificador, GL_POSITION, posicion.data());
	if (es_spotlight && tiene_direccion) {
		glLightfv(identificador, GL_SPOT_DIRECTION , direction.data());
		glLightfv(identificador, GL_SPOT_CUTOFF, &angulo);
	}
}

void FuenteDeLuz::entregarIntensidades(){
	if (definido_especular) 
		glLightfv(identificador, GL_SPECULAR, I_s->dataPointer());
	if (definido_ambiental) 
		glLightfv(identificador, GL_AMBIENT, I_a->dataPointer());			
	if (definido_difuso) 
		glLightfv(identificador, GL_DIFFUSE, I_d->dataPointer());	
}

void FuenteDeLuz::prender(){
	glEnable(identificador);
}

void FuenteDeLuz::apagar(){
	glDisable(identificador);
}

void FuenteDeLuz::definirIntensidadAmbiental(float r, float g, float b){
	if (definido_ambiental) delete I_a;
	I_a = new Color(r, g, b);
	definido_ambiental = true;
}

void FuenteDeLuz::definirPosicion(OpenMesh::Vec4f& p){
	posicion = p;
	tiene_posicion = true;
}

void FuenteDeLuz::definirSpotlight(OpenMesh::Vec3f& v, float cutoff){
	direction = v;
	tiene_direccion = true;
	es_spotlight = true;
	angulo = cutoff;
}

void FuenteDeLuz::definirIntensidadDifusa(float r, float g, float b){
	if (definido_difuso) 
		delete I_d;
	I_d = new Color(r, g, b);
	definido_difuso = true;
}

void FuenteDeLuz::definirIntensidadEspecular(float r, float g, float b){
	if (definido_especular) 
		delete I_s;
	I_s = new Color(r, g, b);
	definido_especular = true;
}

FuenteDeLuz::~FuenteDeLuz(){
	if (definido_especular) 
		delete I_s;
	if (definido_difuso) 
		delete I_d;
	if (definido_ambiental) 
		delete I_a;
}

const OpenMesh::Vec4f& FuenteDeLuz::obtenerPosicion() const{
	return posicion;
}
	
const OpenMesh::Vec4f& FuenteDeLuz::obtenerPosicionTransformada() const{
	return posicion_transformada;
}

void FuenteDeLuz::definirPosicionTransformada(OpenMesh::Vec4f& p){
	posicion_transformada = p;
}

