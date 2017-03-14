#ifndef DEFINICIONES_H_
#define DEFINICIONES_H_

#include <assert.h>
#include <iostream>
#include <ostream>
#include <math.h>
#include <list>


#define PI 3.1415f


// desde LSCM
template <class T> inline T zahir_min(T x, T y) { return x < y ? x : y ; };
template <class T> inline T zahir_max(T x, T y) { return x > y ? x : y ; };

class Utilidades {
public:
	/*
	 * La siguiente función asume que el primer valor es el asociado a 0 en una ecuación paramétrica.
	 * El segundo valor es el asociado a 1, y el tercero es el valor que queremos encontrar.
	 * Ejemplo: en P1(t = 0) x vale -5, en P2(t = 1) x vale 3. Quiero obtener el t 
	 * para el cual x vale 0.
	 */ 
	static inline float calcularParametroInterpolacionLineal(float valor_asociado_a_0, float valor_asociado_a_1, float valor_intermedio){
		float resultado = (valor_intermedio - valor_asociado_a_0) / (valor_asociado_a_1 - valor_asociado_a_0);
		return resultado;
	};
	static inline float gradosARadianes(float grados){
		return grados * PI / 180.0f;
	};
};

#endif /*DEFINICIONES_H_*/
