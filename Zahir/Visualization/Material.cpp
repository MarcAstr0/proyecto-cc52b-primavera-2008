#include "Material.h"

Material::Material()
{
	definido_difuso = definido_ambiental = definido_especular = definida_textura = false;
	definirAmbiental(0.0f, 0.0f, 0.0f);
	definirDifuso(1.0f, 1.0f, 1.0f);
	definirEspecular(1.0f, 1.0f, 1.0f);
	definirBrillosidad(32.0f);
}


Material::~Material() {
	if (textura.estaAsignada())
		textura.eliminarTextura();
}

void Material::definirDifuso(float r, float g, float b){
//	if (definido_difuso) delete K_d;
	K_d = Color(r, g, b);
	definido_difuso = true;
}

void Material::definirAmbiental(float r, float g, float b){
//	if (definido_ambiental) delete K_a;
	K_a = Color(r, g, b);
	definido_ambiental = true;	
}

void Material::definirEspecular(float r, float g, float b){
//	if (definido_especular) delete K_s;
	K_s = Color(r, g, b);
	definido_especular = true;	
}

void Material::definirBrillosidad(float alpha){
	n = alpha;
}

void Material::definirTextura(const Textura &t){
	textura = t;
	definida_textura = true;
}

bool Material::definidoEspecular() const { return definido_especular; }

bool Material::definidoAmbiental() const { return definido_ambiental; }

bool Material::definidoDifuso() const { return definido_difuso; }

bool Material::definidaTextura() const { return definida_textura; }

const Color& Material::obtenerKa() const{
	return K_a;
}

const Color& Material::obtenerKd() const{
	return K_d;
}

const Color& Material::obtenerKs() const{
	return K_s;
}

float Material::obtenerBrillosidad() const{
	return n;
}

const Textura &Material::obtenerTextura() const {
	return textura;
}
