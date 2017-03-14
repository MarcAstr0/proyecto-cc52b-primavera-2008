#include "Color.h"

Color::Color(): rgba_position(1.0f, 1.0f, 1.0f, 1.0f) {}

Color::Color(float r, float g, float b, float alpha): rgba_position(r, g, b, alpha) {}

Color::Color(const Color& c): rgba_position(c.rgba_position) {}

Color& Color::operator=(const Color& c){
	if (this != &c){
		rgba_position = c.rgba_position;
	}
	return *this;
}

Color::~Color(){}

const float* Color::dataPointer() const {
	return rgba_position.data();
}

//TODO: ojo, se asume que alpha es 1.0 en los dos... no debería ser así.
Color operator+(const Color& c1, const Color &c2){
	Color c;
	for (int i = 0; i < 3; i ++){
		c.rgba_position[i] = c1.rgba_position[i] + c2.rgba_position[i];
		if (c.rgba_position[i] > 1.0f) c.rgba_position[i] = 1.0f;
	}
	c.rgba_position[3] = 1.0f;
	return c;
}

Color operator*(float sc, const Color &c1){
	Color c;
	for (int i = 0; i < 3; i ++){
		c.rgba_position[i] = sc * c1.rgba_position[i];
		if (c.rgba_position[i] > 1.0f) c.rgba_position[i] = 1.0f;
	}
	c.rgba_position[3] = c1.rgba_position[3];
	return c;
}
