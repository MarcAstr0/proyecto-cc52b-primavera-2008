#ifndef DEPTHMAP_H_
#define DEPTHMAP_H_

#include "../CgSurfaceRenderer.h"
#include "../../../Color.h"

// TODO: 

class DepthMap : public CgSurfaceRenderer
{
    public:
        DepthMap();
        virtual ~DepthMap();
    protected:
        void preparar(Modelo3D*, Material*);
        float depthScale;
};

#endif /*DEPTHMAP_H_*/
