#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "../Generics/Matrix/Matriz.h"
#include <OpenMesh/Core/Geometry/VectorT.hh>

#define DOT |
#define CROSS %

template <class T>
class QuaternionT
{
public:
	QuaternionT<T>(T i = 0.0f, T j = 0.0f, T k = 0.0f, T w = 1.0f): q_v(i, j, k), q_w(w) {};
	QuaternionT<T>(const Vector& v, T w = 1.0f): q_v(v), q_w(w) {};
	QuaternionT<T>(const Position4f& p): q_v(p), q_w(1.0f) {};
	QuaternionT<T>(const Position3f& p): q_v(p), q_w(1.0f) {};
	QuaternionT<T>(const QuaternionT<T>& q): q_v(q.q_v), q_w(q.q_w) {};
	virtual ~QuaternionT<T>(){};
	
	static QuaternionT<T> rotation(T angle, const Vector& axis) {
		assert(axis.estaNormalizado());
		angle /= 2.0f;
	//	std::cout << "angulo (mitad): " << angle << ", coseno: " << cos(angle) << ", seno: " << sin(angle) << ", axis: " << axis << "\n";
//		std::cout << "rot. quat.: " << sin(angle) * axis << "\n";
		return QuaternionT<T>(sin(angle) * axis, cos(angle));
	};
	
	QuaternionT<T>& operator=(const QuaternionT<T>& q) {
		if (this != &q) {
			q_v = q.q_v;
			q_w = q.q_w;
		}
		return *this;
	};
	
	QuaternionT<T>& operator+=(const QuaternionT<T>& q) {
		q_v += r.q_v;
		q_w += r.q_w;
		return *this;
	};
	
	QuaternionT<T>& operator*=(const QuaternionT<T>& q) {
		float w = q_w;
		q_w = w * r.q_w - q_v DOT r.q_v;
		q_v = ((q_v CROSS r.q_v) + (r.q_w * q_v)) + (w * r.q_v);
		return *this;
	}
;
	
	QuaternionT<T>& operator*=(T scale) {
		q_v *= scale;
		q_w *= scale;
		return *this;
	};
	
	const QuaternionT<T> operator+(const QuaternionT<T>& q) const {
		return QuaternionT<T>(*this) += q;
	};
	const QuaternionT<T> operator*(const QuaternionT<T>& q) const {
		return QuaternionT<T>(*this) *= q;
	};
	const QuaternionT<T> operator*(T scale) const {
		return QuaternionT<T>(*this) *= scale;
	};
	
	T norm() const {
		return sqrt(q_v DOT q_v + q_w * q_w);
	};
	QuaternionT<T> conjugate() const {
		return QuaternionT<T>(-q_v, q_w);
	};
	QuaternionT<T> inverse() const {
		return conjugate() *= (1.0f / (q_v DOT q_v + q_w * q_w));
	};
	QuaternionT<T> unitInverse() const {
		return conjugate();
	};
	
	// for unit quaternions
	QuaternionT<T> pow(T t) const {
		float angle = acos(q_w);
		float angle_t = angle * t;
		return QuaternionT<T>((sin(angle_t) / sin(angle)) * q_v, cos(angle_t));
	};
	QuaternionT<T> log() const {
		float angle = acos(q_w);
		return QuaternionT<T>((angle / sin(angle)) * q_v);
	};
	
	// rotations
//	Matriz<float, 4, 4> getRotationMatrix4() const;
//	Matriz<float, 3, 3> getRotationMatrix3() const;
	OpenMesh::VectorT<T> rotate(const OpenMesh::VectorT<T>& v) {
			QuaternionT<T> q = (*this * QuaternionT<T>(v, 0.0f)) * unitInverse();
			return q.q_v;
	};
	
	Matriz<T, 4, 4> asRotationMatrix4() const {
		T q_x = q_v[0], q_xx = q_x * q_x;
		T q_y = q_v[0], q_yy = q_y * q_y; 
		T q_z = q_v[0], q_zz = q_z * q_z;
		T q_xy = q_x * q_y, q_xz = q_x * q_z, q_yz = q_y * q_z;
		T q_xw = q_x * q_w, q_yw = q_y * q_w, q_zw = q_z * q_w;
		Matriz<T, 4, 4> m;
		m.at(0,0) = 1 - 2(q_yy + q_zz);
		m.at(1,0) = 2(q_xy + q_zw);
		m.at(2,0) = 2(q_xz - q_yw);
		m.at(3,0) = 0;
		m.at(0,1) = 2(q_xy - q_zw);
		m.at(1,1) = 1 - 2(q_xx + q_zz);
		m.at(2,1) = 2(q_yz + q_xw);
		m.at(3,1) = 0;
		m.at(0,2) = 2(q_xz + q_yw);
		m.at(1,2) = 2(q_yz - q_xw);
		m.at(2,2) = 1 - 2(q_xx + q_yy);
		m.at(3,2) = 0;
		m.at(0,3) = 0;
		m.at(1,3) = 0;
		m.at(2,3) = 0;
		m.at(3,3) = 1;
		return m;
	};
	
//	Matriz<T, 3, 3> asRotationMatrix3() const {
//		
//	};
	
protected:
	OpenMesh::VectorT<T, 3> q_v;
	T q_w;
};

typedef QuaternionT<float> 	Quaternionf;
typedef QuaternionT<double> Quaterniond;

#undef DOT
#undef CROSS

#endif /*QUATERNION_H_*/
