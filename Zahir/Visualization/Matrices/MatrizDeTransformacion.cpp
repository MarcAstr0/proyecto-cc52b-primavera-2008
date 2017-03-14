#include "MatrizDeTransformacion.h"
#include "../../Geometry/definiciones.h"

MatrizDeTransformacion::MatrizDeTransformacion():Matriz<float,4,4>(){
	hacerIdentidad();
}

MatrizDeTransformacion::MatrizDeTransformacion(const MatrizDeTransformacion &m):Matriz<float,4,4>(){
	for (int i = 0; i < 16; i++)
		valores[i] = m.valores[i];	
}

MatrizDeTransformacion &MatrizDeTransformacion::operator=(const MatrizDeTransformacion &m){
	if (this != &m){
		for (int i = 0; i < 16; i++)
			valores[i] = m.valores[i];	
	}
	return *this;
}

MatrizDeTransformacion::~MatrizDeTransformacion(){
	
}

void MatrizDeTransformacion::hacerIdentidad(){
	for (int i = 1; i < 16; i++)
		valores[i] = 0.0f;
	valores[0] = 1.0f;
	valores[5] = 1.0f;
	valores[10] = 1.0f;
	valores[15] = 1.0f;
}

MatrizDeTransformacion MatrizDeTransformacion::obtenerInversa(){
	MatrizDeTransformacion inv;
	
	inv.valores[0] = valores[0];
	inv.valores[1] = valores[4];
	inv.valores[2] = valores[8];
	inv.valores[4] = valores[1];
	inv.valores[5] = valores[5];
	inv.valores[6] = valores[9];
	inv.valores[8] = valores[2];
	inv.valores[9] = valores[6];
	inv.valores[10] = valores[10];

	inv.valores[12] = inv.valores[0]*-valores[12]+inv.valores[4]*-valores[13]+inv.valores[8]*-valores[14];
	inv.valores[13] = inv.valores[1]*-valores[12]+inv.valores[5]*-valores[13]+inv.valores[9]*-valores[14];
	inv.valores[14] = inv.valores[2]*-valores[12]+inv.valores[6]*-valores[13]+inv.valores[10]*-valores[14];

	inv.valores[3] = 0.0f;
	inv.valores[7] = 0.0f;
	inv.valores[11] = 0.0f;
	inv.valores[15] = 1.0f;
	
	return inv;
}

// operadores

MatrizDeTransformacion operator*(const MatrizDeTransformacion &m1, const MatrizDeTransformacion &m2){
	MatrizDeTransformacion m;
	//TODO: no sé cómo aprovechar el operador* de la clase madre, así que por el momento replicaré el código
	int i1, i2, i3, i4, i;
	for (int col = 0; col < 4; col++){
		i = 4*col;
		i1 = i; i2 = i + 1; i3 = i + 2; i4 = i + 3;
		m.valores[i1] = m1.valores[0]*m2.valores[i1] + m1.valores[4]*m2.valores[i2] + m1.valores[ 8]*m2.valores[i3] + m1.valores[12]*m2.valores[i4];
		m.valores[i2] = m1.valores[1]*m2.valores[i1] + m1.valores[5]*m2.valores[i2] + m1.valores[ 9]*m2.valores[i3] + m1.valores[13]*m2.valores[i4];
		m.valores[i3] = m1.valores[2]*m2.valores[i1] + m1.valores[6]*m2.valores[i2] + m1.valores[10]*m2.valores[i3] + m1.valores[14]*m2.valores[i4];
		m.valores[i4] = m1.valores[3]*m2.valores[i1] + m1.valores[7]*m2.valores[i2] + m1.valores[11]*m2.valores[i3] + m1.valores[15]*m2.valores[i4];
	}
	return m;
}

OpenMesh::Vec4f operator*(const MatrizDeTransformacion& m1, const OpenMesh::Vec4f& p){
	Matriz<float, 4, 1> vector_posicion;
	vector_posicion[0] = p[0]; 
	vector_posicion[1] = p[1]; 
	vector_posicion[2] = p[2]; 
	vector_posicion[3] = p[3];
	Matriz<float, 4, 1> resultado = m1 * vector_posicion;
	OpenMesh::Vec4f res;
	res[0] = resultado[0];
	res[1] = resultado[1];
	res[2] = resultado[2];
	res[3] = resultado[3];
	return res;
}

OpenMesh::Vec4f operator*(const MatrizDeTransformacion& m1, const OpenMesh::Vec3f& p){
	Matriz<float, 4, 1> vector_posicion;
	vector_posicion[0] = p[0]; 
	vector_posicion[1] = p[1]; 
	vector_posicion[2] = p[2]; 
	vector_posicion[3] = 1.0f;
	Matriz<float, 4, 1> resultado = m1 * vector_posicion;
	OpenMesh::Vec4f res;
	res[0] = resultado[0];
	res[1] = resultado[1];
	res[2] = resultado[2];
	res[3] = resultado[3];
	return res;
}


//Vector operator*(const MatrizDeTransformacion& m1, const Vector& v){
//	Position4f dir;
//	dir[0] = v.obtenerDireccion().at(0);
//	dir[1] = v.obtenerDireccion().at(1);
//	dir[2] = v.obtenerDireccion().at(2);
//	dir[3] = 1.0f;
//	Posicion<float,4> res = m1 * dir;
//	return	Vector(res);
//}

std::ostream& operator<<(std::ostream& stream, MatrizDeTransformacion &matriz){
	for (int i = 0; i < 4; i++){
		if (i == 0) stream << "----------\n";
		for (int j = 0; j < 4; j++){
			stream << matriz.at(i,j) << " ";
		}
		stream << "\n";
	}
	stream << "...........\n";
	return stream;
}
