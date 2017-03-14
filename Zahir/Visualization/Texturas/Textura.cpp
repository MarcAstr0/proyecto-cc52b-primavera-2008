#include "Textura.h"
#include <cmath>
#include <assert.h>
#include <GL/gl.h>
#include <GL/glext.h>

Textura::Textura(int d, texture_type t)
{
	identificador = 0;
	width = 0;
	height = 0;
	asignada = false;
	type = t;
	dimension = d;
}

Textura::Textura(const Textura& t){
	identificador = t.identificador;
	asignada = t.asignada;
	dimension = t.dimension;
	width = t.width;
	height = t.height;	
	type = t.type;
}

Textura &Textura::operator=(const Textura& t){
	if (this != &t){
		identificador = t.identificador;
		asignada = t.asignada;
		dimension = t.dimension;
		width = t.width;
		height = t.height;	
	}
	return *this;
}

Textura::~Textura()
{
	//eliminarTextura();
}

void Textura::asignarIdentificador(unsigned int id){
//	std::cout << "asignando textura: " << identificador << std::endl;
	identificador = id;
	asignada = true;
}

void Textura::asignarDimension(int _width, int _height){
	width = _width;
	height = _height;
}

bool Textura::estaAsignada() const{
	return asignada;
}

//TODO: esto DEBE SALIR DE ESTA CLASE
void Textura::eliminarTextura(){
		glDeleteTextures(1, &identificador);
};
