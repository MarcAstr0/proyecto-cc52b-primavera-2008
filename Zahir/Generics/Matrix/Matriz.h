#ifndef MATRIZ_H_
#define MATRIZ_H_

#include <assert.h>
#include <ostream>

template <class tipo, int N, int M>
class Matriz
{
public:
	Matriz<tipo,N,M>() {};
	Matriz<tipo,N,M>(const Matriz<tipo,N,M> &matriz) {
		for (int i = 0; i < N * M; i++)
			valores[i] = matriz.valores[i];	
	};
	Matriz<tipo,N,M> &operator=(const Matriz<tipo,N,M> &matriz){
		if (this != &matriz){
			for (int i = 0; i < N * M; i++)
					valores[i] = matriz.valores[i];
		}
		return *this;
	};
	tipo& at(const int i, const int j) {
		assert(i < N);
		assert(i >= 0);
		assert(j < M);
		assert(j >= 0);
		return valores[i + j * N];
	};
	tipo& operator[](const int indice) {
		assert(indice < M*N && indice >= 0);
		return valores[indice];
	};
	Matriz<tipo,M,N> obtenerTraspuesta() const {
		Matriz<tipo, M, N> nueva_matriz;
		for (int i = 1; i <= M; i++)
			for (int j = 1; i <= N; i++)
				nueva_matriz.at(i,j) = this->at(j,i);
		return nueva_matriz;
	};
	
	void rellenar(const tipo *arreglo){
		for (int i = 0; i < N*M; i++)
			valores[i] = arreglo[i];
	};
	
	virtual ~Matriz(){};
	
	// TODO: operator +=, *= ...
	
	template <class t, class t2, int n, int m, int q>
	friend Matriz<t2, n, q> operator*(const Matriz<t, n, m> &matriz1, const Matriz<t2, m, q> &matriz2);

	
	template <class t, int n, int m>
	friend Matriz<t, n, m> operator-(Matriz<t, n, m> &matriz1, Matriz<t, n, m> &matriz2);
	
	template <class t, int n, int m>
	friend Matriz<t, n, m> operator+(Matriz<t, n, m> &matriz1, Matriz<t, n, m> &matriz2);
	
	template <class t, int n, int m>
	friend Matriz<t, n, m> operator/(Matriz<t, n, m> &matriz, t &escalar);
	
	template <class t, int n, int m>
	friend Matriz<t, n, m> operator*(t escalar, Matriz<t, n, m> &matriz);

	template <class t, int n, int m>
	friend std::ostream& operator<<(std::ostream& stream, Matriz<t, n, m> &matriz);
	
	
	// especializaciones
//	template <class t>
//	friend Matriz<t, 4, 4> operator*(const Matriz<t, 4, 4> &m1, const Matriz<t, 4, 4> &m2);
//	
//	template <class t, class t2>
//	friend Matriz<t2, 4, 1> operator*(const Matriz<t, 4, 4> &matriz1, const Matriz<t2, 4, 1> &matriz2);
//	
//	template <class t, class t2>
//	friend Matriz<t2, 1, 4> operator*(const Matriz<t, 1, 4> &matriz1, const Matriz<t2, 4, 4> &matriz2);
//	
	template <class t, class t2, int n>
	friend t2 operator*(const Matriz<t, 1, n> &matriz1, const Matriz<t2, n, 1> &matriz2);

	
protected:
	tipo valores[N*M];
};

typedef Matriz<float, 4, 4> 	Matrix4f;
typedef Matriz<double, 4, 4> 	Matrix4d;
typedef Matriz<float, 3, 3> 	Matrix3f;
typedef Matriz<double, 3, 3> 	Matrix3d;

#include "Matrix.operations.h"

#endif /*MATRIZ_H_*/
