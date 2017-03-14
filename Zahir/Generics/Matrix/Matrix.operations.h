#ifndef MATRIX_OPERATIONS_H_
#define MATRIX_OPERATIONS_H_


template <class t, class t2, int n, int m, int q>
Matriz<t2, n, q> operator*(const Matriz<t, n, m> &matriz1, const Matriz<t2, m, q> &matriz2){
	Matriz<t2,n,q> nueva_matriz;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < q; j++){
			t2 r = matriz1.valores[i] * matriz2.valores[j * n];
			for (int k = 1; k < m; k++){
				r = r + matriz1.valores[i + k * n] * matriz2.valores[k + j * n];
			}
			nueva_matriz.at(i,j) = r; 
		}
	}
	return nueva_matriz;
};


template <class t, int n, int m>
Matriz<t, n, m> operator-(Matriz<t, n, m> &matriz1, Matriz<t, n, m> &matriz2){
	Matriz<t,n,m> nueva_matriz;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			nueva_matriz.at(i,j) = matriz1.at(i,j) - matriz2.at(i,j); 
		}
	}
	return nueva_matriz;
};

template <class t, int n, int m>
Matriz<t, n, m> operator+(Matriz<t, n, m> &matriz1, Matriz<t, n, m> &matriz2){
	Matriz<t,n,m> nueva_matriz;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			nueva_matriz.at(i,j) = matriz1.at(i,j) + matriz2.at(i,j); 
		}
	}
	return nueva_matriz;
};

template <class t, int n, int m>
Matriz<t, n, m> operator/(Matriz<t, n, m> &matriz, t &escalar){
	Matriz<t,n,m> nueva_matriz;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			nueva_matriz.at(i,j) = matriz.at(i,j) / escalar; 
		}
	}
	return nueva_matriz;
};

template <class t, int n, int m>
Matriz<t, n, m> operator*(t escalar, Matriz<t, n, m> &matriz){
	Matriz<t,n,m> nueva_matriz;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			nueva_matriz.at(i,j) = escalar * matriz.at(i,j); 
		}
	}
	return nueva_matriz;
};

template <class t, int n, int m>
std::ostream& operator<<(std::ostream& stream, Matriz<t, n, m> &matriz){
	for (int i = 0; i < n; i++){
		if (i == 0) stream << "----------\n";
		for (int j = 0; j < m; j++){
			stream << matriz.at(i,j) << " ";
		}
		stream << "\n";
	}
	stream << "...........\n";
	return stream;
}


template <class T1, class T2, int N>
T2 operator*(const Matriz<T1, 1, N> &m1, const Matriz<T2, N, 1> &m2){
	T2 r = m1.valores[0] * m2.valores[0];
	for (int i = 1; i < N; i++)
		r = m1.valores[i] * m2.valores[i] + r;
	return r;
};


#endif /*MATRIX_OPERATIONS_H_*/
