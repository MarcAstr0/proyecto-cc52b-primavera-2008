#ifndef INTERPOLADORCUBICO_H_
#define INTERPOLADORCUBICO_H_

//TODO: realizar optimizaciones. por ejemplo, una vez que se definieron los puntos de control, el producto de factores * puntos de control siempre es el mismo.

#include "Interpolador.h"
#include "../Matrix/Matriz.h"

template <class T>
class InterpoladorCubico: public Interpolador<T>
{
public:
	InterpoladorCubico<T>():Interpolador<T>(){};
	virtual void inicializarFactores()=0;
	void definirPuntoDeControl(unsigned int indice, T& punto){
		assert(indice < 4);
		this->puntos_de_control[indice] = punto;
	};
	void definirPuntosDeControl(T& p0, T& p1, T& p2, T& p3){
		this->puntos_de_control[0] = p0;
		this->puntos_de_control[1] = p1;
		this->puntos_de_control[2] = p2;
		this->puntos_de_control[3] = p3;
	};
	virtual ~InterpoladorCubico<T>(){};
	T interpolar(float parametro){
		this->parametros[3] = 1;
		this->parametros[2] = parametro;
		this->parametros[1] = parametro * this->parametros[2];
		this->parametros[0] = parametro * this->parametros[1];
		T resultado = this->parametros * (this->factores * this->puntos_de_control);	
		return resultado;
	};
	const Matriz<float,4,4>& obtenerMatriz(){
		return parametros;
	};
protected:
	Matriz<float,1,4> parametros;
	Matriz<float,4,4> factores;
	Matriz<T,4,1> puntos_de_control;
};

template <class T>
class CatmullRom: public InterpoladorCubico<T>{
public:
	CatmullRom<T>():InterpoladorCubico<T>(){ tension = 0.5f; this->inicializarFactores();};
	~CatmullRom<T>(){};
	void inicializarFactores(){
		this->factores[0] = -tension;       this->factores[4] = 2.0f - tension; this->factores[ 8] = tension - 2.0f;        this->factores[12] = tension;
		this->factores[1] = 2.0f * tension; this->factores[5] = tension - 3.0f; this->factores[ 9] = 3.0f - 2.0f * tension; this->factores[13] = -tension;
		this->factores[2] = -tension;       this->factores[6] = 0.0f;           this->factores[10] = tension;               this->factores[14] = 0.0f;
		this->factores[3] = 0.0f;           this->factores[7] = 1.0f;           this->factores[11] = 0.0f;                  this->factores[15] = 0.0f;
	}
	void definirTension(float t) { tension = t; this->inicializarFactores(); };
protected:
	float tension;
};

// funciona

template <class T>
class Bezier: public InterpoladorCubico<T>{
public:
	Bezier<T>():InterpoladorCubico<T>(){ this->inicializarFactores(); std::cout << "factores:" << this->factores; };
	~Bezier<T>(){};
	void inicializarFactores(){
		this->factores[ 0] = -1.0f; this->factores[ 4] =  3.0f; this->factores[ 8] = -3.0f; this->factores[12] = 1.0f;
		this->factores[ 1] =  3.0f; this->factores[ 5] = -6.0f; this->factores[ 9] =  3.0f; this->factores[13] = 0.0f;
		this->factores[ 2] = -3.0f; this->factores[ 6] =  3.0f; this->factores[10] =  0.0f; this->factores[14] = 0.0f;
		this->factores[ 3] =  1.0f; this->factores[ 7] =  0.0f; this->factores[11] =  0.0f; this->factores[15] = 0.0f;
	}
};

template <class T>
class BSpline: public InterpoladorCubico<T>{
public:
	BSpline<T>():InterpoladorCubico<T>(){ this->inicializarFactores(); };
	~BSpline<T>(){};
	void inicializarFactores(){
		this->factores[ 0] = -1.0f; this->factores[ 4] =  3.0f; this->factores[ 8] = -3.0f; this->factores[12] = 1.0f;
		this->factores[ 1] =  3.0f; this->factores[ 5] = -6.0f; this->factores[ 9] =  3.0f; this->factores[13] = 0.0f;
		this->factores[ 2] = -3.0f; this->factores[ 6] =  0.0f; this->factores[10] =  3.0f; this->factores[14] = 0.0f;
		this->factores[ 3] =  1.0f; this->factores[ 7] =  4.0f; this->factores[11] =  1.0f; this->factores[15] = 0.0f;
		this->factores = (1.0f/6.0f) * this->factores; 
	}
};

#endif /*INTERPOLADORCUBICO_H_*/
