#ifndef PHONG_H_
#define PHONG_H_

#include "../CgSurfaceRenderer.h"

// TODO: detectar texturas y número de luces
class Phong : public CgSurfaceRenderer
{
public:
	Phong(): CgSurfaceRenderer() {
		
		std::string zahir_path = getenv("ZAHIR_PATH"); 
		
		vertex_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Phong/vertex_program.cg");
		fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Phong/fragment_program.cg");
		
		requiere_posicion_luz = true;
		nombre_posicion_luz = "lightPosition";
		
		requiere_posicion_vista = true;
		nombre_posicion_vista = "eyePosition";
		
		requiere_modelview_proj = true;
		nombre_modelview_proj = "modelViewProj";
	};
	virtual ~Phong(){};
	
};

#endif /*PHONG_H_*/
