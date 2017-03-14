#ifndef INTERPOLADORLINEAL_H_
#define INTERPOLADORLINEAL_H_

#include <vector>
#include "Interpolador.h"

template <class T>
class InterpoladorLineal: public Interpolador<T>
{
public:
	InterpoladorLineal<T>():Interpolador<T>(){
		total_puntos_de_control = 2;
		puntos_de_control.resize(total_puntos_de_control);
	};
	void definirPuntoDeControl(unsigned int indice, T& punto){
		assert(indice < puntos_de_control.size());
		puntos_de_control[indice] = punto;
	};
	void definirPuntosDeControl(T& p0, T& p1){
		puntos_de_control[0] = p0;
		puntos_de_control[1] = p1;
	};
	virtual ~InterpoladorLineal(){};
	T interpolar(float parametro) {
		return (1.0f - parametro) * puntos_de_control[0] + parametro * puntos_de_control[1]; 
	};
	void interpolarEn(const T &p0, const T &p1, float parametro, T &resultado){
		resultado = (1.0f - parametro) * p0 + parametro * p1;
	}
protected:
	std::vector<T> puntos_de_control;
	short total_puntos_de_control;
};

#endif /*INTERPOLADORLINEAL_H_*/
