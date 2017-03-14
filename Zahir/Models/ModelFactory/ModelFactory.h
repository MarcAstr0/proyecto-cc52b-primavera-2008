#ifndef MODELFACTORY_H_
#define MODELFACTORY_H_

#include "../Cargadores/CargadorOBJ.h"
#include "../Modelo3D.h"


class ModelFactory {

    public:
    static Modelo3D* loadFromFile(std::string filename) {
    
      CargadorModelo3D *cargador;
      Modelo3D *model;
      if (filename.substr(filename.length() - 4) == ".obj"){
		  model = new Modelo3D();
		  cargador = new CargadorOBJ(model, filename); 
      } else {
    	  return NULL;
      }

      cargador->cargarDatos();
      delete cargador;
      return model;

    };
};


#endif /*MODELFACTORY_H_*/
