#ifndef INTERPOLADOR_H_
#define INTERPOLADOR_H_

template<class T>
class Interpolador
{
public:
	Interpolador<T>(){};
	virtual void definirPuntoDeControl(unsigned int indice, T &punto)=0;
	virtual T interpolar(float parametro)=0;
	virtual ~Interpolador(){};
};

#endif /*INTERPOLADOR_H_*/
