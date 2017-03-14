#include <GL/gl.h>
#include <GL/glu.h>
#include "SurfaceCurvature.h"

SurfaceCurvature::SurfaceCurvature() {
	usar_materiales = false;
	graficar_texturas = false;
	added_curvature = false;
}

SurfaceCurvature::~SurfaceCurvature() {
}

void SurfaceCurvature::ejecutar(Modelo3D* modelo, Material *material){	
	ZahirMesh::ConstFaceIter    	fIt(modelo->_mesh.faces_begin()), fEnd(modelo->_mesh.faces_end());
	ZahirMesh::ConstFaceVertexIter 	fvIt;
	
	if (!added_curvature) {
		colorCoding(modelo, modelo->_mesh);
		added_curvature = true;
	}
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);
	
	for (; fIt != fEnd; ++fIt) {
	      fvIt = modelo->_mesh.cfv_iter(fIt.handle());
	      glColor3fv ( modelo->_mesh.property(_curv_color, fvIt).data() );
	      glVertex3fv( &modelo->_mesh.point(fvIt)[0] );
	      ++fvIt;
	      glColor3fv ( modelo->_mesh.property(_curv_color, fvIt).data() );
	      glVertex3fv( &modelo->_mesh.point(fvIt)[0] );
	      ++fvIt;
	      glColor3fv ( modelo->_mesh.property(_curv_color, fvIt).data() );
	      glVertex3fv( &modelo->_mesh.point(fvIt)[0] );      
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

/* from ... */
//TODO: mover esto de aquí a otro lado, quizás una clase "colorizer"
void SurfaceCurvature::colorCoding(Modelo3D* modelo, ZahirMesh& mesh) {
	mesh.add_property(_curv_color, "curvature color");
	
	ZahirMesh::VertexIter  v_it, v_end(mesh.vertices_end());
	ZahirMesh::Scalar      curv, min_curv(FLT_MAX), max_curv(-FLT_MAX);
	ZahirMesh::Point       col;
	
	// put all curvature values into one array
	std::vector<ZahirMesh::Scalar> curv_values;
	curv_values.reserve(mesh.n_vertices());
	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
		curv_values.push_back(modelo->cv1(v_it) * modelo->cv2(v_it));
	
	// discard upper and lower 5%
	unsigned int n = curv_values.size()-1;
	unsigned int i = n / 20;
	std::sort(curv_values.begin(), curv_values.end());
	min_curv = curv_values[i];
	max_curv = curv_values[n-1-i];
	
	// define uniform color intervalls [v0,v1,v2,v3,v4]
	ZahirMesh::Scalar v0, v1, v2, v3, v4;
	v0 = min_curv + 0.0/4.0 * (max_curv - min_curv);
	v1 = min_curv + 1.0/4.0 * (max_curv - min_curv);
	v2 = min_curv + 2.0/4.0 * (max_curv - min_curv);
	v3 = min_curv + 3.0/4.0 * (max_curv - min_curv);
	v4 = min_curv + 4.0/4.0 * (max_curv - min_curv);
	
	// map curvatures to colors
	
	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) {
		curv = modelo->cv1(v_it) * modelo->cv2(v_it);
		col = ZahirMesh::Point(255,255,255);
		
	    unsigned char u;
	    
	    if (curv < v0)
	    	col = ZahirMesh::Point(0, 0, 255);
	    else if (curv > v4) 
	    	col = ZahirMesh::Point(255, 0, 0);
	    else if (curv <= v2) {
	    	if (curv <= v1) { // [v0, v1]
	    		u = (unsigned char) (255.0 * (curv - v0) / (v1 - v0));
	    		col = ZahirMesh::Point(0, u, 255);
	    	} else { // ]v1, v2]
	    		u = (unsigned char) (255.0 * (curv - v1) / (v2 - v1));
	    		col = ZahirMesh::Point(0, 255, 255-u);
	    	}
	    } else {
	    	if (curv <= v3) { // ]v2, v3]
	    		u = (unsigned char) (255.0 * (curv - v2) / (v3 - v2));
	    		col = ZahirMesh::Point(u, 255, 0);
	    	} else { // ]v3, v4]
	    		u = (unsigned char) (255.0 * (curv - v3) / (v4 - v3));
	    		col = ZahirMesh::Point(255, 255-u, 0);
	    	}
	    }
		
	    mesh.property(_curv_color, v_it) = (col *= 1.0f/255.0f);
	}
}
