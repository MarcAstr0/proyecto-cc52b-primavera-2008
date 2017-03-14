#ifndef HELPERRENDERERS_H
#define HELPERRENDERERS_H

#include "SurfaceRenderer.h"

class SurfaceVectors: public SurfaceRenderer {
public:
	SurfaceVectors();
	virtual ~SurfaceVectors();
	void ejecutar(Modelo3D*, Material*);
protected:
	std::string rendered_vectors_identifier; // examples: "normals", "principal directions"
};

class WireFrameRenderer: public SurfaceRenderer {
	WireFrameRenderer();
	virtual ~WireFrameRenderer();
	void preparar();
	void finalizar();
};

class DepthBufferOnlyRenderer: public SurfaceRenderer {
	DepthBufferOnlyRenderer();
	virtual ~DepthBufferOnlyRenderer();
	void preparar();
	void finalizar();
};

#endif /*HELPERRENDERERS_H*/