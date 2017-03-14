#include <OpenMesh/Core/IO/MeshIO.hh>
#include "CargadorOBJ.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../External/StringUtils.h"
#include "../../Visualization/Material.h"

CargadorOBJ::CargadorOBJ(Modelo3D* modelo, std::string filename):CargadorModelo3D(modelo, filename){}

CargadorOBJ::~CargadorOBJ(){}

void CargadorOBJ::loadData(ZahirMesh& mesh, std::vector<Material*>& materiales) {
	
	std::fstream archivo_obj;
	archivo_obj.open(nombre_de_archivo.c_str(), std::ios::in);
	std::string linea;
	std::vector<std::string> datos;
	
//	std::vector<Position3f> 			vertices;
	std::vector<ZahirMesh::TexCoord2D> 	texcoords;
//	std::vector<Vector> 				normals;
	std::vector<ZahirMesh::Normal>		normals;
	std::vector<ZahirMesh::FaceHandle> 	faces;
	
	OpenMesh::IO::BaseImporter::VHandles vhandles;

	unsigned int total_puntos = 0;
	unsigned int total_normales = 0;
	unsigned int total_texcoords = 0;
	
	// veamos si hay una textura por cargar

	Material* material_actual;
	std::istringstream reader;
	while (archivo_obj.good()){
		std::getline(archivo_obj, linea);
		datos.clear();
		int tokens = StringUtils::SplitString(linea, " ", datos, false);
		if (tokens > 0){
			std::string tipo = datos.at(0);
			if (tipo[0] == '#' || tipo == "g" || tipo == "p" || tipo == "vp") 
				continue; 
			else if (tipo == "mtllib"){ // un archivo de material
				std::string archivo_material = nombre_de_archivo.substr(0, nombre_de_archivo.find_last_of('/')) + "/" + datos.at(1);
				if (materiales.size() == 0)
					loadMTL(archivo_material, materiales);
			}
			else if (tipo == "usemtl" && materiales.size() > 0){
				continue;
				//TODO: considerar varios materiales.
//				std::string nombre_material = datos.at(1);
//				assert(nombre_material.length() >= 1); // que al menos tenga un nombre válido el material
//				material_actual = materiales[nombre_material];
//				assert(material_actual != NULL); // no queremos materiales null
//				asociacion_materiales[indice_triangulo_actual] = material_actual;
			}
			else if (tipo == "v"){ // posicion
				total_puntos++;
				mesh.add_vertex(ZahirMesh::Point(atof(datos[1].c_str()), atof(datos[2].c_str()), atof(datos[3].c_str())));
			}
			else if (tipo == "vt"){ // textura
				ZahirMesh::TexCoord2D tpos;
				tpos[0] = atof(datos[1].c_str());
				tpos[1] = atof(datos[2].c_str());
				texcoords.push_back(tpos);
				total_texcoords++;
			}
			else if (tipo == "vn"){ // normal
				continue;
//				Position3f punto_temporal;
//				punto_temporal[0] = atof(datos.at(1).c_str());
//				punto_temporal[1] = atof(datos.at(2).c_str());
//				punto_temporal[2] = atof(datos.at(3).c_str());
//				normales.push_back(Vector(punto_temporal));
//				total_normales++;
			}
			else if (tipo == "f"){ // cara
				//TODO: usar un número arbitrario de vértices para las caras. después de todo, la triangulación es trivial.
				assert(datos.size() == 4 || datos.size() == 5); // sólo con triángulos y quads
				std::vector<std::string> datos_cara;
				
				// usaremos 3 o 4 vértices, por lo que necesito 4 índices de posición, 4 de normal y 4 de textura, y por supuesto cuatro punteros.
				
				int v[4], vn[4], vt[4];
				unsigned int iv[4];
				bool has_vt = false;
				
				vhandles.clear();
				
				for (int i = 0; i < 4; i++){
					v[i] = vn[i] = vt[i] = -1;
				}
				
				for (int i = 0; (unsigned int) i < (datos.size() - 1); i++){ //ciclo para cada vértice
					StringUtils::SplitString(datos.at(i + 1), "/", datos_cara, true); // el formato es v/vt/vn
					if (datos_cara.size() == 3){ // v/vt/vn. quizás vt sea nulo (v//vn).
						v[i] = atoi(datos_cara.at(0).c_str()) - 1;
						vn[i] = atoi(datos_cara.at(2).c_str()) - 1;
						if (v[i] < 0)	v[i] = total_puntos + 1 + v[i];
						if (vn[i] < 0) vn[i] = normals.size() + 1 + vn[i];
						if (datos_cara.at(1).length() > 0){ // tiene textura
							vt[i] = atoi(datos_cara.at(1).c_str()) - 1;
							if (vt[i] < 0) 
								vt[i] = texcoords.size() + 1 + vt[i];
							has_vt = true;
						} 
					} // end if datos_cara.size == 3
					else if (datos_cara.size() == 2){ // v/vt . en este caso vt siempre existe. no tiene normal
						v[i] = atoi(datos_cara.at(0).c_str()) - 1;
						vt[i] = atoi(datos_cara.at(1).c_str()) - 1;
						if (v[i] < 0)	
							v[i] = total_puntos + 1 + v[i];
						if (vt[i] < 0) 
							vt[i] = texcoords.size() + 1 + vt[i];					
						has_vt = true;
					}
					else if (datos_cara.size() == 1){ // aquí no tiene textura ni normal
						v[i] = atoi(datos_cara.at(0).c_str()) - 1;
						if (v[i] < 0)	
							v[i] = total_puntos + 1 + v[i];
					}
					else{ // indefinido
						assert(false);
					}
					
					vhandles.push_back(OpenMesh::VertexHandle(v[i]));
					
					datos_cara.clear();
				} // endfor
				
				// ahora tengo los vértices. debo construir las aristas y los triángulos.
					
				faces.push_back(mesh.add_face(vhandles));
				
				int he_vt = 0;
				if (faces.back().is_valid() && has_vt)
					setTexCoord(faces.back(), v, vt, texcoords);
								
				// ahora veamos qué hacer en caso de tener un cuadrilátero
				
				if (datos.size() == 5){ //quad. nos falta un triángulo por procesar.
					vhandles.clear();
					vhandles.push_back(OpenMesh::VertexHandle(v[2]));
					vhandles.push_back(OpenMesh::VertexHandle(v[3]));
					vhandles.push_back(OpenMesh::VertexHandle(v[0]));
					faces.push_back(mesh.add_face(vhandles));
					
					if (faces.back().is_valid() && has_vt) {
						// si tiene texcoords debo recuperar las half edge de las caras
						int face_vt[] = { vt[2], vt[3], vt[0] };
						int face_v[] = { v[2], v[3], v[0] };
						setTexCoord(faces.back(), face_vt, face_v, texcoords);
					}
					
				} // if datos.size() == 5	
				vhandles.clear();
			} // if tipo == f
		}
		
		if (archivo_obj.eof()) 
			break;
	} // endwhile
	
	archivo_obj.close();
	
}

void CargadorOBJ::loadMTL(std::string filename, std::vector<Material*> &materiales){
	std::fstream archivo_mtl;
	archivo_mtl.open(filename.c_str(), std::ios::in);
	if (!archivo_mtl.good()) {
		std::cerr << "error en el archivo de material " << filename.c_str() <<  std::endl;
		return;
	}
	std::string linea;
	std::vector<std::string> datos;
	Material *actual;
	std::string nombre_material;
	unsigned int materiales_creados = 0;
	std::istringstream reader;
	while (archivo_mtl.good() && !archivo_mtl.eof()){
		std::getline(archivo_mtl, linea);
		datos.clear();
		int tokens = StringUtils::SplitString(linea, " ", datos, true);
		if (tokens > 0){
				std::string tipo = datos.at(0);
				if (tipo == "newmtl"){ // un archivo de material
					if (materiales_creados >= 1){ //había una definición de material antes, entonces lo metemos al mapa
						continue;
//						assert(nombre_material.length() >= 1); // si había un material, que tenga nombre válido.
//						materiales[nombre_material] = actual;
					}
					actual = new Material();
					nombre_material = datos.at(1);
					std::cout << "definido nuevo material: " << nombre_material << std::endl;
					materiales_creados++;
				}
				else if (tipo == "Ka"){ 
					float r,g,b;
					r = atof(datos[1].c_str());
					g = atof(datos[2].c_str());
					b = atof(datos[3].c_str());
					assert (r <= 1.0f && r >= 0.0f && g <= 1.0f && g >= 0.0f && b <= 1.0f && b >= 0.0f);
					actual->definirAmbiental(r, g, b);
				}
				else if (tipo == "Kd"){ 
					float r,g,b;
					r = atof(datos[1].c_str());
					g = atof(datos[2].c_str());
					b = atof(datos[3].c_str());
					assert (r <= 1.0f && r >= 0.0f && g <= 1.0f && g >= 0.0f && b <= 1.0f && b >= 0.0f);
					actual->definirDifuso(r, g, b);
				}
				else if (tipo == "Ks"){ 
					float r,g,b;
					r = atof(datos[1].c_str());
					g = atof(datos[2].c_str());
					b = atof(datos[3].c_str());
					assert (r <= 1.0f && r >= 0.0f && g <= 1.0f && g >= 0.0f && b <= 1.0f && b >= 0.0f);
					actual->definirEspecular(r, g, b);
				}
				else if (tipo == "Ns"){ 
					float glossyness;
					glossyness = atof(datos[1].c_str());
					assert(glossyness <= 1000.0f && glossyness >= 0.0f);
					actual->definirBrillosidad(glossyness);
				}
				else if (tipo == "map_Kd"){
					std::fstream archivo_textura;
					//TODO: posible error aquí
					std::string nombre_textura = nombre_de_archivo.substr(0, nombre_de_archivo.find_last_of('/')) + "/" + datos.at(1);
					archivo_textura.open(nombre_textura.c_str(), std::ios::in);
					if (archivo_textura.good()){
						std::cerr << "leyendo... " << nombre_textura << "\n";
						//unsigned int textura_modelo = CargadorTexturas::cargarTextura(filename.substr(0, filename.find_last_of('/')) + '/' + datos.at(1).c_str());
						unsigned int textura_modelo = CargadorTexturas::cargarTextura(nombre_textura);
						std::cerr << "se obtuvo: " << textura_modelo << "\n";
						Textura t;
						if (textura_modelo > 0) {
							t.asignarIdentificador(textura_modelo);
							actual->definirTextura(t);
							std::cerr << "textura asignada al material ;)\n";
						}
						archivo_textura.close();
					}
				}
				reader.clear();
		}
		if (archivo_mtl.eof()) 
			break;
	}
	archivo_mtl.close();
	if (actual != NULL && materiales_creados > materiales.size()) 
		//materiales[nombre_material] = actual;
		materiales.push_back(actual);
	std::cerr << "total materiales: " << materiales.size() << std::endl;	
}

