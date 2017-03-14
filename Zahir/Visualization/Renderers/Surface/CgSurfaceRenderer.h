#ifndef CGSURFACERENDERER_H_
#define CGSURFACERENDERER_H_

#include <string>
#include "../../GPU/CgWrapper.h"
#include "SurfaceRenderer.h"
#include "../../Matrices/MatrizDeTransformacion.h"
#include "../../FuenteDeLuz.h"
#include "../../Camara.h"

class CgSurfaceRenderer : public SurfaceRenderer
{
public:
	CgSurfaceRenderer();
	CgSurfaceRenderer(std::string archivo_vertex_program, std::string archivo_fragment_program);
	virtual ~CgSurfaceRenderer();
	void asignarCamara(Camara&);
	void asignarFuenteDeLuz(FuenteDeLuz&);
protected:
	virtual void preparar(Modelo3D*, Material*);
	virtual void finalizar(Modelo3D*,Material*);
	void graficarMaterial(Material*);
	
	CgProgram vertex_shader;
	CgProgram fragment_shader;
//	CgProgram geometry_shader;
	
	bool requiere_posicion_luz;
	bool requiere_posicion_vista;
	bool requiere_modelview_proj;
	bool requiere_modelview;
	std::string nombre_posicion_luz;
	std::string nombre_posicion_vista;
	std::string nombre_modelview_proj;
	std::string nombre_modelview;
	
	std::string nombre_textura;
	
	FuenteDeLuz *fuente_de_luz;
	Camara *camara;
};

#endif /*CGSURFACERENDERER_H_*/
