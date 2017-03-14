#include "../../../Texturas/CargadorTexturas.h"
#include "Toon.h"

Toon::Toon(): CgSurfaceRenderer() {
	
	std::string zahir_path = getenv("ZAHIR_PATH");
	
	vertex_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Toon/vertex_program.cg");
	fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Toon/fragment_program.cg");
	
	requiere_modelview_proj = true;
	nombre_modelview_proj = "modelViewProj";
	
	requiere_modelview = false;
	
	requiere_posicion_luz = true;
	nombre_posicion_luz = "lightPosition";
	
	requiere_posicion_vista = true;
	nombre_posicion_vista = "eyePosition";
	
	nombre_textura = "textura";
	
	float diffuse_ramp[] = {0.3f, 0.3f, 0.3f, 0.7f, 0.7f, 0.7f, 1.0f, 1.0f};
	float specular_ramp[] = {0.0f, 0.0f, 0.0f, 1.0f};
	// Notar que las texturas son potencias de dos
//	intensidad_difusa = Textura(diffuse_ramp, 8);
	// TODO: verificar que se carguen bien.
	intensidad_difusa.asignarIdentificador(CargadorTexturas::cargarTextura1D(diffuse_ramp, 8));
//	intensidad_especular = Textura(specular_ramp, 4);
	intensidad_especular.asignarIdentificador(CargadorTexturas::cargarTextura1D(specular_ramp, 4));
}

Toon::Toon(const Textura &difusa, const Textura &especular): CgSurfaceRenderer() {
	
	std::string zahir_path = getenv("ZAHIR_PATH");
		
	vertex_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Toon/vertex_program.cg");
	fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/Toon/fragment_program.cg");
	
	requiere_modelview_proj = true;
	nombre_modelview_proj = "modelViewProj";
	
	requiere_modelview = false;
	
	requiere_posicion_luz = true;
	nombre_posicion_luz = "lightPosition";
	
	requiere_posicion_vista = true;
	nombre_posicion_vista = "eyePosition";
	
	nombre_textura = "textura";
	
	intensidad_difusa = difusa;
	intensidad_especular = especular;
}

Toon::~Toon() {
	intensidad_difusa.eliminarTextura();
	intensidad_especular.eliminarTextura();
}

void Toon::preparar(Modelo3D *modelo, Material *material) {
	this->CgSurfaceRenderer::preparar(modelo, material);
	
	fragment_shader.bindTexture("diffuseRamp", intensidad_difusa);
	fragment_shader.bindTexture("specularRamp", intensidad_especular);
}

void Toon::finalizar(Modelo3D *modelo, Material *material){
	fragment_shader.unbindTexture("specularRamp");
	fragment_shader.unbindTexture("diffuseRamp");
	
	this->CgSurfaceRenderer::finalizar(modelo, material);
}


