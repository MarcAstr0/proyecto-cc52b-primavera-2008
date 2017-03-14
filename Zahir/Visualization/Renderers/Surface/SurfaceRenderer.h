#ifndef SURFACERENDERER_H_
#define SURFACERENDERER_H_

#include "../../../Models/Modelo3D.h"

/**
 * Esta clase grafica el interior de un objeto. Sus funciones son virtuales para que otras clases, que hereden de ella, puedan redefinir
 * sus métodos.
 */

class SurfaceRenderer
{
public:
	SurfaceRenderer();
	virtual void graficar(Modelo3D*);
	virtual ~SurfaceRenderer();
	void desactivarMateriales();
protected:
	virtual void preparar(Modelo3D*, Material*);
	virtual void ejecutar(Modelo3D*, Material*);
	virtual void finalizar(Modelo3D*,Material*);
	virtual void graficarMaterial(Material*);

	bool graficar_texturas;
	bool usar_materiales;
	
	//TODO: use the following booleans
	bool cull_back_faces;
	bool draw_vertex_normals;
	bool draw_face_normals;
	bool draw_textures;
	bool draw_colors;
	bool use_lightning;
	
	
};

#endif /*SURFACERENDERER_H_*/
