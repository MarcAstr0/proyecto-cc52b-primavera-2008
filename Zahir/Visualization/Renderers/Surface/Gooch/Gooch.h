#ifndef GOOCH_H_
#define GOOCH_H_

#include "../CgSurfaceRenderer.h"
#include "../../../Color.h"

// TODO: detectar número de luces

class Gooch : public CgSurfaceRenderer
{
public:
	Gooch();
	Gooch(const Color& k_c, const Color& k_w);
	virtual ~Gooch();
protected:
	void preparar(Modelo3D*, Material*);
	
	Color k_blue;
	Color k_yellow;
	
	float alpha;
	float beta;
};

#endif /*GOOCH_H_*/
