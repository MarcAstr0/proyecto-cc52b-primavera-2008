#ifndef CARGADOROBJ_H_
#define CARGADOROBJ_H_

#include "CargadorModelo3D.h"

class CargadorOBJ : public CargadorModelo3D
{
public:
	CargadorOBJ(Modelo3D *modelo, std::string filename);
	virtual ~CargadorOBJ();
	void loadData(ZahirMesh& mesh, std::vector<Material*> &materiales);
	void loadMTL(std::string filename, std::vector<Material* > &materiales);
};

#endif /*CARGADOROBJ_H_*/
