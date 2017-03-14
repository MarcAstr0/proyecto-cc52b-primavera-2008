#include "Gooch.h"
#include <cstdlib>

Gooch::Gooch(): 
	CgSurfaceRenderer() {
	
	std::string zahir_path = getenv("ZAHIR_PATH"); 
	
	vertex_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Gooch/vertex_program.cg");
	fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Gooch/fragment_program.cg");
	
	assert(vertex_shader.isLoaded());
	
	requiere_posicion_luz = true;
	nombre_posicion_luz = "lightPosition";
	
	requiere_posicion_vista = true;
	nombre_posicion_vista = "eyePosition";
	
	graficar_texturas = false;
	usar_materiales = false;
	k_blue = Color(0.0f, 0.0f, 0.5f, 1.0f);
	k_yellow = Color(0.5f, 0.5f, 0.0f, 1.0f);
	
	alpha = 0.2f;
	beta = 0.6f;
	
	
}

Gooch::Gooch(const Color& k_b, const Color& k_y): 
	CgSurfaceRenderer() {
	
	std::string zahir_path = getenv("ZAHIR_PATH");
	
	vertex_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Gooch/vertex_program.cg");
	fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Gooch/fragment_program.cg");
	
	assert(vertex_shader.isLoaded());
	
	graficar_texturas = false;
	usar_materiales = false;
	k_blue = k_b;
	k_yellow = k_y;
	
	alpha = 0.3f;
	beta = 0.7f;
}

Gooch::~Gooch() {}

void Gooch::preparar(Modelo3D *modelo, Material *material) {
	this->CgSurfaceRenderer::preparar(modelo, material);
	
	Color k_cool = k_blue + alpha * material->obtenerKd();
	Color k_warm = k_yellow + beta * material->obtenerKd();
	
	fragment_shader.setColorParameter("k_warm", k_warm);
	fragment_shader.setColorParameter("k_cool", k_cool);
}

