#ifndef POLILINEA_H_
#define POLILINEA_H_

//TODO: convertirla en una clase con template

/**
 * cosas que debe considerar la polilínea en el futuro:
 * - utilizar un tipo de storage de acuerdo al uso que se le dará a la polilínea
 * - utilizar un distinto tipo de "punto" de la polilínea
 */

//#include "definiciones.h"
//#include <list>
//#include "../Generics/Position/Posicion.h"
//
//class Polilinea
//{
//public:
//	Polilinea();
//	Polilinea(std::list<Position3f>&);
//	Polilinea(std::list<Position3f>::iterator inicio, std::list<Position3f>::iterator fin);
//	
//	Polilinea(const Polilinea&);
//	Polilinea& operator=(const Polilinea&);
//	
//	void insertarPunto(Position3f&);
//	bool estaCerrada() const;
//	virtual ~Polilinea();
//	void finalizar(bool cerrar = false);
//	unsigned int numeroDePuntos() const;
//	inline std::list<Position3f>::iterator inicioLinea() { return puntos.begin();};
//	inline std::list<Position3f>::iterator finLinea() { return puntos.end();};
//	inline void limpiar() { puntos.clear(); };
//	
//private:
//	//TODO: cada punto debiera tener cierta información asociada a él, que debería ser añadida al construir la polilínea
//	std::list<Position3f> puntos;
//	bool cerrada;
//};
//
//Polilinea::Polilinea(){
//	cerrada = false;
//}
//
//
//Polilinea::Polilinea(std::list<Position3f>& puntos_a_insertar){
//	//puntos.clear();
//	//puntos = puntos_a_insertar;
//	cerrada = false;
//	puntos.assign(puntos_a_insertar.begin(), puntos_a_insertar.end());
//}
//
//Polilinea::Polilinea(std::list<Position3f>::iterator inicio, std::list<Position3f>::iterator fin){
//	puntos.assign(inicio, fin);
//	cerrada = false;
//}
//
//Polilinea::Polilinea(const Polilinea& pol){
//	cerrada = pol.cerrada;
//	puntos.assign(pol.puntos.begin(), pol.puntos.end());
//}
//
//Polilinea& Polilinea::operator=(const Polilinea& pol){
//	if (this != &pol){
//		cerrada = pol.cerrada;
//		puntos.assign(pol.puntos.begin(), pol.puntos.end());
//	}
//	return *this;
//}
//
//void Polilinea::insertarPunto(Position3f &p){
//	puntos.push_back(p);
//}
//
//Polilinea::~Polilinea(){
//	puntos.clear();
//}
//
//bool Polilinea::estaCerrada() const {
//	return cerrada;
//}
//
//void Polilinea::finalizar(bool cerrar){
//	cerrada = cerrar;
//}
//
//unsigned int Polilinea::numeroDePuntos() const {
//	return puntos.size();
//}

#endif /*POLILINEA_H_*/
