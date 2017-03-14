#ifndef ESTIMADORDENORMALES_H_
#define ESTIMADORDENORMALES_H_

#include "Algoritmo.h"
#include "../Modelo3D.h"

class EstimadorDeNormales: public Algorithm1Ring
{
public:
	EstimadorDeNormales();
//	Vector calculateFor(const Punto& p, std::list<const Triangulo*>& ts);
	virtual ~EstimadorDeNormales();
};

#endif /*ESTIMADORDENORMALES_H_*/
