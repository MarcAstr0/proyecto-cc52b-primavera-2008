#ifndef TOON_H_
#define TOON_H_

#include "../CgSurfaceRenderer.h"

class Toon : public CgSurfaceRenderer
{
public:
	Toon();
	Toon(const Textura &difusa, const Textura &especular);
	virtual ~Toon();
protected:
	void preparar(Modelo3D*, Material*);
	void finalizar(Modelo3D*, Material*);
	
	Textura intensidad_difusa;
	Textura intensidad_especular;
	
};

#endif /*TOON_H_*/
