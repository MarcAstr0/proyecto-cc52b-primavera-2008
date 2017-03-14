#ifndef MATRIZDETRANSFORMACION_H_
#define MATRIZDETRANSFORMACION_H_

#include "../../Generics/Matrix/Matriz.h"
//#include "../../Geometry/Vector.h"

#include <OpenMesh/Core/Geometry/VectorT.hh>

 
// TODO: reescribir esta clase.

#define MIN_ROTATION_ANGLE 0.001f
#define PI 3.1415f

inline 
float zahir_grad2rad(float grad){
	return grad * PI / 180.0f;
};

class MatrizDeTransformacion : public Matriz<float,4,4>
{
public:
	MatrizDeTransformacion();
	MatrizDeTransformacion(const MatrizDeTransformacion &m);
	MatrizDeTransformacion &operator=(const MatrizDeTransformacion &m);
	virtual ~MatrizDeTransformacion();
	
	// TODO: utilizar la inversa "for real", no la heurística para modelview
	MatrizDeTransformacion obtenerInversa();
	
	static MatrizDeTransformacion MatrizDeTraslacion(float tx, float ty, float tz){
		MatrizDeTransformacion m;
		m.valores[12] = tx;
		m.valores[13] = ty;
		m.valores[14] = tz;
		m.valores[15] = 1.0f;
		return m;
	};
	static MatrizDeTransformacion Escalamiento(float sx, float sy, float sz){
		MatrizDeTransformacion m;
		m.at(0,0) = sx;
		m.at(1,1) = sy;
		m.at(2,2) = sz;
		m.at(3,3) = 1.0f;
		return m;
	};
	static MatrizDeTransformacion MatrizDeTraslacion(const OpenMesh::Vec3f& p){
		MatrizDeTransformacion m;
		m.valores[12] = p[0];
		m.valores[13] = p[1];
		m.valores[14] = p[2];
		m.valores[15] = 1.0f;
		return m;
	};
	static MatrizDeTransformacion MatrizDeRotacion(float rx, float ry, float rz){
		// TODO: evitar gimbal lock
		MatrizDeTransformacion mrx, mry, mrz;
		
		float cos_angulo = cos(zahir_grad2rad(rx));
		float sen_angulo = sin(zahir_grad2rad(rx));
		
		mrx.valores[0] = cos_angulo;
		mrx.valores[1] = sen_angulo;
		mrx.valores[4] = -sen_angulo;
		mrx.valores[5] = cos_angulo;
		
		cos_angulo = cos(zahir_grad2rad(ry));
		sen_angulo = sin(zahir_grad2rad(ry));
		
		mry.valores[5] = cos_angulo;
		mry.valores[6] = sen_angulo;
		mry.valores[9] = -sen_angulo;
		mry.valores[10] = cos_angulo;
		
		cos_angulo = cos(zahir_grad2rad(rz));
		sen_angulo = sin(zahir_grad2rad(rz));
		
		mrz.valores[0] = cos_angulo;
		mrz.valores[8] = sen_angulo;
		mrz.valores[2] = -sen_angulo;
		mrz.valores[10] = cos_angulo;
		
		MatrizDeTransformacion final = mrz * mry * mrx;
		return final;
	};
	
	/*** implementation based on Real Time Rendering, Second Edition, Section 3.2.4, Equation 3.27 
	 * axis MUST be normalized*/
	static MatrizDeTransformacion MatrizDeRotacion(float angle, const OpenMesh::Vec3f& axis) {
		MatrizDeTransformacion m;
		if (abs(angle) < MIN_ROTATION_ANGLE)
			return m;
		if (axis.norm() < MIN_ROTATION_ANGLE)
			return m;
		
		float cos_phi = cos(angle);
		float sin_phi = sin(angle);
		
		float rx = axis[0];
		float ry = axis[1];
		float rz = axis[2];
		
		m.at(0,0) = cos_phi + (1.0f - cos_phi) * rx * rx;
		m.at(1,0) = (1.0f - cos_phi) * rx * ry + rz * sin_phi;
		m.at(2,0) = (1.0f - cos_phi) * rx * rz - ry * sin_phi;
		
		m.at(0,1) = (1.0f - cos_phi) * rx * ry - rz * sin_phi;
		m.at(1,1) = cos_phi + (1.0f - cos_phi) * ry * ry;
		m.at(2,1) = (1.0f - cos_phi) * ry * rz + rx * sin_phi;
		
		m.at(0,2) = (1.0f - cos_phi) * rx * rz + ry * sin_phi;
		m.at(1,2) = (1.0f - cos_phi) * ry * rz - rx * sin_phi;
		m.at(2,2) = cos_phi + (1.0f - cos_phi) * rz * rz;
		
		return m;
	};
	
	// TODO: operator *=
	friend MatrizDeTransformacion operator*(const MatrizDeTransformacion &m1, const MatrizDeTransformacion &m2);
	
	friend OpenMesh::Vec4f operator*(const MatrizDeTransformacion &m1, const OpenMesh::Vec4f& p);
	friend OpenMesh::Vec4f operator*(const MatrizDeTransformacion &m1, const OpenMesh::Vec3f& p);
//	friend Vector operator*(const MatrizDeTransformacion &m1, const Vector& p);
	friend std::ostream& operator<<(std::ostream& stream, MatrizDeTransformacion& m);

protected:
	void hacerIdentidad();
	
};

#undef MIN_ROTATION_ANGLE
#undef PI

#endif /*MATRIZDETRANSFORMACION_H_*/
