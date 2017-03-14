#ifndef SURFACECURVATURE_H_
#define SURFACECURVATURE_H_

#include "SurfaceRenderer.h"

class SurfaceCurvature : public SurfaceRenderer
{
public:
	SurfaceCurvature();
	virtual ~SurfaceCurvature();
protected:
	void ejecutar(Modelo3D*, Material*);
	void colorCoding(Modelo3D* modelo, ZahirMesh& mesh);
	
	OpenMesh::MPropHandleT< float > _max_curvature;
	OpenMesh::MPropHandleT< float > _min_curvature;
	OpenMesh::VPropHandleT< ZahirMesh::Point > _curv_color;
	bool added_curvature;
};

#endif /*SURFACECURVATURE_H_*/
