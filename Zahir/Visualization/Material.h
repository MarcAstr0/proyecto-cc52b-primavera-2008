#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Geometry/definiciones.h"
#include "Color.h"
#include "Texturas/Textura.h"

class Material
{
public:
	Material();
	Material(const Material& m);
	Material& operator=(const Material& m);
	void definirDifuso(float r, float g, float b);
	void definirAmbiental(float r, float g, float b);
	void definirEspecular(float r, float g, float b);
	void definirBrillosidad(float brillosidad);
	void definirTextura(const Textura &t);
	bool definidoDifuso() const;
	bool definidoAmbiental() const;
	bool definidoEspecular() const;
	bool definidaTextura() const;
	virtual ~Material();
	const Color &obtenerKd() const;
	const Color &obtenerKs() const;
	const Color &obtenerKa() const;
	const Textura &obtenerTextura() const;
	float obtenerBrillosidad() const;
	
private:
	/// Coeficiente de reflectividad difusa
	Color K_d;  
	/// Coeficiente de reflectividad especular
	Color K_s; 
	/// Coeficiente de reflectividad ambiental
	Color K_a; 
	bool definido_difuso;
	bool definido_especular;
	bool definido_ambiental;
	bool definida_textura;
	/// Exponente para R dot V
	float n;
	/// Textura Difusa del material
	Textura textura;
};

#endif /*MATERIAL_H_*/
