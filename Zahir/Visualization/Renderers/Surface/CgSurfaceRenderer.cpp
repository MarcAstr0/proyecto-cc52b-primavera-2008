#include "CgSurfaceRenderer.h"

//TODO: hacer este constructor (y considerar el error handling del siguiente constructor).

CgSurfaceRenderer::CgSurfaceRenderer():
	SurfaceRenderer(), 
	vertex_shader(CgProgram::VERTEX_PROGRAM),
	fragment_shader(CgProgram::FRAGMENT_PROGRAM) {
	
	requiere_modelview_proj = true;
	requiere_modelview = false;
	requiere_posicion_luz = true;
	requiere_posicion_vista = true;
	
	nombre_posicion_luz = "lightPos";
	nombre_posicion_vista = "eyePos";
	nombre_modelview_proj = "modelViewProj";
	nombre_modelview = "modelView";
	
	nombre_textura = "textura";
	
	camara = NULL;
	fuente_de_luz = NULL;
	
} 

CgSurfaceRenderer::CgSurfaceRenderer(std::string archivo_vertex_program, std::string archivo_fragment_program):
	SurfaceRenderer(), 
	vertex_shader(CgProgram::VERTEX_PROGRAM),
	fragment_shader(CgProgram::FRAGMENT_PROGRAM) {
	
	vertex_shader.loadFromFile(archivo_vertex_program);
	fragment_shader.loadFromFile(archivo_fragment_program);
	
	//TODO: algún tipo de error handling
	
	requiere_modelview_proj = true;
	requiere_modelview = false;
	requiere_posicion_luz = true;
	requiere_posicion_vista = true;
	
	nombre_posicion_luz = "lightPos";
	nombre_posicion_vista = "eyePos";
	nombre_modelview_proj = "modelViewProj";
	nombre_modelview = "modelView";
	
	nombre_textura = "textura";
	
	camara = NULL;
	fuente_de_luz = NULL;
}

CgSurfaceRenderer::~CgSurfaceRenderer() {}

void CgSurfaceRenderer::asignarCamara(Camara &c) {
	camara = &c;
}

void CgSurfaceRenderer::asignarFuenteDeLuz(FuenteDeLuz &f) {
	fuente_de_luz = &f;
}

void CgSurfaceRenderer::preparar(Modelo3D*, Material *material) {
	vertex_shader.bind();
	fragment_shader.bind();
	
	// Parámetros típicos
	
	if (requiere_modelview)
		vertex_shader.setMatrixParameter(nombre_modelview, CgProgram::MODELVIEW_MATRIX);
	
	if (requiere_modelview_proj)
		vertex_shader.setMatrixParameter(nombre_modelview_proj, CgProgram::MODELVIEW_PROJECTION_MATRIX);
	
	if (requiere_posicion_luz)
		fragment_shader.setLightSourceParameter(nombre_posicion_luz, *fuente_de_luz);
	
	if (requiere_posicion_vista)
		fragment_shader.setVec4fParameter(nombre_posicion_vista, camara->obtenerPosicionTransformada());

	if (usar_materiales) 
		this->graficarMaterial(material);
}

void CgSurfaceRenderer::finalizar(Modelo3D*,Material *material) {
    if (usar_materiales && graficar_texturas && material->definidaTextura()) 
    	fragment_shader.unbindTexture(nombre_textura);

    fragment_shader.unBind();
    vertex_shader.unBind();
}

void CgSurfaceRenderer::graficarMaterial(Material* material) {
	fragment_shader.setMaterialParameter(*material);
	
	if (graficar_texturas && material->definidaTextura()){
		fragment_shader.bindTexture(nombre_textura, material->obtenerTextura());
		fragment_shader.setBoolParameter("tiene_textura", true);
	}
	else
		fragment_shader.setBoolParameter("tiene_textura", false);
	
}

