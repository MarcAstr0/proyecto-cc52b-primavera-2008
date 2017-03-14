#ifndef COLOR_H_
#define COLOR_H_

#include <OpenMesh/Core/Geometry/VectorT.hh>

class Color
{
public:
	Color();
	Color(float r, float g, float b, float alpha = 1.0f);
	Color(const Color& c);
	Color& operator=(const Color& c);
	const float* dataPointer() const;
	
	friend Color operator+(const Color& c1, const Color& c2);
	friend Color operator*(float sc, const Color& c);
	
	virtual ~Color();
	
private:
	OpenMesh::Vec4f rgba_position;
};

#endif /*COLOR_H_*/
