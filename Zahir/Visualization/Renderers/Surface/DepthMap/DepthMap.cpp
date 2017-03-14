#include "DepthMap.h"
#include <cstdlib>

DepthMap::DepthMap(): 
    CgSurfaceRenderer() {
	
    std::string zahir_path = getenv("ZAHIR_PATH"); 
    
    vertex_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/DepthMap/vertex_program.cg");
    fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/DepthMap/fragment_program.cg");
    
    assert(vertex_shader.isLoaded());
    
    requiere_modelview_proj = true;
    nombre_modelview_proj = "modelViewProj";
    
    requiere_modelview = false;
    
    requiere_posicion_luz = false;
    
    requiere_posicion_vista = false;
    
    graficar_texturas = false;
    usar_materiales = false;

    depthScale = 0.004800;
}

DepthMap::~DepthMap() {}

void DepthMap::preparar(Modelo3D *modelo, Material *material) {
    this->CgSurfaceRenderer::preparar(modelo, material);
    vertex_shader.setFloatParameter("depthScale", depthScale);
}

