#include "EstimadorDeNormales.h"
#include <list>

EstimadorDeNormales::EstimadorDeNormales()
{
}

EstimadorDeNormales::~EstimadorDeNormales()
{
}

//Vector EstimadorDeNormales::calculateFor(const Punto& p, std::list<const Triangulo*>& adjacent_triangles){
//	assert(adjacent_triangles.size() > 0);
//	float weight;
//	Position3f average;		
//	for (std::list<const Triangulo*>::iterator it = adjacent_triangles.begin(); it != adjacent_triangles.end(); it++){
//		weight = 1.0f;
//		
//		for (int k = 0; k < 3; k++){
//			Arista& a = (*it)->obtenerArista(k);
//			if (a.esDeExtremo(&p)) {
//				assert(a.largo() > 0);
//				weight /= a.largo() * a.largo();
//			}
//		}
//		
//		weight *= (*it)->areaDoble();  
//		average += weight * (*it)->obtenerNormal().obtenerDireccion();
//	}
//
//	return Vector(average);
//}
