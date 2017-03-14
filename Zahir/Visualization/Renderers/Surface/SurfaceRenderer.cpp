#include "SurfaceRenderer.h"
#include <vector>
#include <map>
#include <GL/gl.h>
#include "../../Texturas/Textura.h"

SurfaceRenderer::SurfaceRenderer(){
	graficar_texturas = true;
	usar_materiales = true;
}

SurfaceRenderer::~SurfaceRenderer(){
}

void SurfaceRenderer::desactivarMateriales() {
	usar_materiales = false;
}

void SurfaceRenderer::preparar(Modelo3D*, Material* material){
	if (usar_materiales)
		this->graficarMaterial(material);
}

void SurfaceRenderer::finalizar(Modelo3D*, Material* material){

		if (graficar_texturas && material->definidaTextura()) {
			switch (material->obtenerTextura().obtenerDimension()) {
			case 1:
				glDisable(GL_TEXTURE_1D);
				break;
			case 2:
				glDisable(GL_TEXTURE_2D);
				break;
			case 3:
				glDisable(GL_TEXTURE_3D);
				break;
			}
		}
}

void SurfaceRenderer::ejecutar(Modelo3D* modelo, Material *material){	
	ZahirMesh::ConstFaceIter    	fIt(modelo->_mesh.faces_begin()), fEnd(modelo->_mesh.faces_end());
	ZahirMesh::ConstFaceVertexIter 	fvIt;
	ZahirMesh::FaceHalfedgeIter 	he_it;
	
	glBegin(GL_TRIANGLES);
	
	if (graficar_texturas && material->definidaTextura()) {
		for (; fIt != fEnd; ++fIt) {
			he_it = modelo->_mesh.fh_iter(fIt.handle());
			for (int i = 0; i < 3; i++) {
				glNormal3fv		( & modelo->_mesh.normal	( modelo->_mesh.from_vertex_handle( he_it.handle() ) )[0] );
				glTexCoord2fv	( & modelo->_mesh.property	( modelo->_half_edge_texcoords, he_it)[0] );
				glVertex3fv		( & modelo->_mesh.point		( modelo->_mesh.from_vertex_handle( he_it.handle() ) )[0] );
				++he_it;
			}
		}   		
	} else {
		for (; fIt != fEnd; ++fIt) {
		      fvIt = modelo->_mesh.cfv_iter(fIt.handle());
		      glNormal3fv( &modelo->_mesh.normal(fvIt)[0] );
		      glVertex3fv( &modelo->_mesh.point(fvIt)[0] );
		      ++fvIt;
		      glNormal3fv( &modelo->_mesh.normal(fvIt)[0] );
		      glVertex3fv( &modelo->_mesh.point(fvIt)[0] );
		      ++fvIt;
		      glNormal3fv( &modelo->_mesh.normal(fvIt)[0] );
		      glVertex3fv( &modelo->_mesh.point(fvIt)[0] );      
		}
	}

	glEnd();
}

void SurfaceRenderer::graficar(Modelo3D *modelo){
	this->preparar(modelo, modelo->materiales[0]);
	this->ejecutar(modelo, modelo->materiales[0]);
	this->finalizar(modelo, modelo->materiales[0]);
}

void SurfaceRenderer::graficarMaterial(Material* material){
	assert(material != NULL);
	if (material->definidoAmbiental())
		glMaterialfv(GL_FRONT, GL_AMBIENT, material->obtenerKa().dataPointer());

	if (material->definidoDifuso())
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material->obtenerKd().dataPointer());

	if (material->definidoEspecular()) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, material->obtenerKs().dataPointer());
		static float shininess = material->obtenerBrillosidad();
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	}

	if (graficar_texturas && material->definidaTextura()) {
		switch (material->obtenerTextura().obtenerDimension()) {
		case 1:
			glEnable(GL_TEXTURE_1D);
			glBindTexture(GL_TEXTURE_1D, material->obtenerTextura().obtenerIdentificador());
			break;
		case 2:
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, material->obtenerTextura().obtenerIdentificador());
			break;
		case 3:
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, material->obtenerTextura().obtenerIdentificador());
			break;
		}
	}
}

