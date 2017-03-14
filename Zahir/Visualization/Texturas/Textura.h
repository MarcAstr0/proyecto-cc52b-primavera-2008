#ifndef TEXTURA_H_
#define TEXTURA_H_

class Textura
{
public:
	enum texture_type {DIFFUSE_MAP, SPECULAR_MAP, EMISSIVE_MAP, NORMAL_MAP, LIGHT_MAP, DEPTH_MAP, OTHER_MAP};
	Textura(int d = 2, texture_type = DIFFUSE_MAP);
	Textura(const Textura &t);
	Textura& operator=(const Textura &t);
	void asignarIdentificador(unsigned int id);
	void asignarDimension(int _width, int _height);
	int obtenerIdentificador() const { return identificador; };
	int obtenerDimension() const { return dimension; };
	int getWidth() const { return width; };
	int getHeight() const { return height; };
	texture_type getType() const { return type; };
	void setType(texture_type t) { type = t; };

	void eliminarTextura();
	bool estaAsignada() const;
	virtual ~Textura();
	
protected:
	//TODO: usar un shared ptr de un unsigned int
	unsigned int identificador;
	int width;
	int height;
	
	bool asignada;
	int dimension;
	
	texture_type type;
};

#endif /*TEXTURA_H_*/
