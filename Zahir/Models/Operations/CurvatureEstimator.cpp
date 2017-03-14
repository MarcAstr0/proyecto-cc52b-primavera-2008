#include "CurvatureEstimator.h"
#include "../../External/lineqn.h"

/**
 * Based on code by Szymon Rusinkiewicz :
 * Rusinkiewicz, Szymon.
 "Estimating Curvatures and Their Derivatives on Triangle Meshes,"
 Proc. 3DPVT, 2004.
 * 
 *  
 * //   Example code for the full-day course
//
//   M. Botsch, M. Pauly, L. Kobbelt, P. Alliez, B. Levy,
//   "Geometric Modeling Based on Polygonal Meshes"
//   held at SIGGRAPH 2007, San Diego, and Eurographics 2008, Crete.
//
//   Copyright (C) 2007 by  Computer Graphics Laboratory, ETH Zurich, 
//                      and Computer Graphics Group,      RWTH Aachen
 * */

/*-------------------------- 
Szymon Rusinkiewicz
Princeton University

from TriMesh_curvature.cc
*/


// i+1 and i-1 modulo 3
// This way of computing it tends to be faster than using %
//#define NEXT(i) ((i)<2 ? (i)+1 : (i)-2)
//#define PREV(i) ((i)>0 ? (i)-1 : (i)+2)


// Rotate a coordinate system to be perpendicular to the given normal
template <class vec>
static void rot_coord_sys(const vec &old_u, const vec &old_v,
			  const vec &new_norm,
			  vec &new_u, vec &new_v)
{
	new_u = old_u;
	new_v = old_v;
	vec old_norm = old_u % old_v;
	float ndot = old_norm | new_norm;
	if (unlikely(ndot <= -1.0f)) {
		new_u = -new_u;
		new_v = -new_v;
		return;
	}
	vec perp_old = new_norm - ndot * old_norm;
	vec dperp = 1.0f / (1 + ndot) * (old_norm + new_norm);
	new_u -= dperp * (new_u | perp_old);
	new_v -= dperp * (new_v | perp_old);
}


// Reproject a curvature tensor from the basis spanned by old_u and old_v
// (which are assumed to be unit-length and perpendicular) to the
// new_u, new_v basis.
template <class vec>
void proj_curv(const vec &old_u, const vec &old_v,
	       float old_ku, float old_kuv, float old_kv,
	       const vec &new_u, const vec &new_v,
	       float &new_ku, float &new_kuv, float &new_kv)
{
	vec r_new_u, r_new_v;
	rot_coord_sys(new_u, new_v, old_u % old_v, r_new_u, r_new_v);

	float u1 = r_new_u | old_u;
	float v1 = r_new_u | old_v;
	float u2 = r_new_v | old_u;
	float v2 = r_new_v | old_v;
	new_ku  = old_ku * u1*u1 + old_kuv * (2.0f  * u1*v1) + old_kv * v1*v1;
	new_kuv = old_ku * u1*u2 + old_kuv * (u1*v2 + u2*v1) + old_kv * v1*v2;
	new_kv  = old_ku * u2*u2 + old_kuv * (2.0f  * u2*v2) + old_kv * v2*v2;
}


// Like the above, but for dcurv
template <class vec, class vec4>
void proj_dcurv(const vec &old_u, const vec &old_v,
		const vec4 old_dcurv,
		const vec &new_u, const vec &new_v,
		vec4 &new_dcurv)
{
	vec r_new_u, r_new_v;
	rot_coord_sys(new_u, new_v, old_u % old_v, r_new_u, r_new_v);

	float u1 = r_new_u | old_u;
	float v1 = r_new_u | old_v;
	float u2 = r_new_v | old_u;
	float v2 = r_new_v | old_v;

	new_dcurv[0] = 	old_dcurv[0]*u1*u1*u1 +
		       		old_dcurv[1]*3.0f*u1*u1*v1 +
		       		old_dcurv[2]*3.0f*u1*v1*v1 +
		       		old_dcurv[3]*v1*v1*v1;
	new_dcurv[1] = 	old_dcurv[0]*u1*u1*u2 +
		       		old_dcurv[1]*(u1*u1*v2 + 2.0f*u2*u1*v1) +
		       		old_dcurv[2]*(u2*v1*v1 + 2.0f*u1*v1*v2) +
		       		old_dcurv[3]*v1*v1*v2;
	new_dcurv[2] = 	old_dcurv[0]*u1*u2*u2 +
		       		old_dcurv[1]*(u2*u2*v1 + 2.0f*u1*u2*v2) +
		       		old_dcurv[2]*(u1*v2*v2 + 2.0f*u2*v2*v1) +
		       		old_dcurv[3]*v1*v2*v2;
	new_dcurv[3] = 	old_dcurv[0]*u2*u2*u2 +
		       		old_dcurv[1]*3.0f*u2*u2*v2 +
		       		old_dcurv[2]*3.0f*u2*v2*v2 +
		       		old_dcurv[3]*v2*v2*v2;
}


// Given a curvature tensor, find principal directions and curvatures
// Makes sure that pdir1 and pdir2 are perpendicular to normal
template <class vec>
void diagonalize_curv(const vec &old_u, const vec &old_v,
		      float ku, float kuv, float kv,
		      const vec &new_norm,
		      vec &pdir1, vec &pdir2, float &k1, float &k2)
{
	vec r_old_u, r_old_v;
	rot_coord_sys(old_u, old_v, new_norm, r_old_u, r_old_v);

	float c = 1, s = 0, tt = 0;
	if (likely(kuv != 0.0f)) {
		// Jacobi rotation to diagonalize
		float h = 0.5f * (kv - ku) / kuv;
		tt = (h < 0.0f) ?
			1.0f / (h - sqrt(1.0f + h*h)) :
			1.0f / (h + sqrt(1.0f + h*h));
		c = 1.0f / sqrt(1.0f + tt*tt);
		s = tt * c;
	}

	k1 = ku - tt * kuv;
	k2 = kv + tt * kuv;

	if (fabs(k1) >= fabs(k2)) {
		pdir1 = c*r_old_u - s*r_old_v;
	} else {
		std::swap(k1, k2);
		pdir1 = s*r_old_u + c*r_old_v;
	}
	pdir2 = new_norm % pdir1;
}


/*-------------------------- IMPLEMENTATION: CurvatureEstimator */

CurvatureEstimator::CurvatureEstimator(ZahirMesh& m, OpenMesh::VPropHandleT<ZahirMesh::Scalar>& cv1, OpenMesh::VPropHandleT<ZahirMesh::Scalar>& cv2, OpenMesh::VPropHandleT<OpenMesh::Vec3f>& pdir1, OpenMesh::VPropHandleT<OpenMesh::Vec3f>& pdir2, OpenMesh::VPropHandleT<OpenMesh::Vec4f>& curvderiv): 
	mesh(m), vcurv1(cv1), vcurv2(cv2), vpdir1(pdir1), vpdir2(pdir2), vcurvderiv(curvderiv) {
	mesh.add_property(vweight, "vertex weights (tmp)");
	mesh.add_property(eweight, "edge weights (tmp)");
	mesh.add_property(vcurv12, "gaussian curvature (tmp)");
}

CurvatureEstimator::~CurvatureEstimator() {
	mesh.remove_property(vcurv12);
	mesh.remove_property(vweight);
	mesh.remove_property(eweight);
}

void CurvatureEstimator::calculate() {
	calculateWeights();
	calculateCurvature();
	calculateCurvatureDerivatives();
}

/* taken from Curvature Viewer, Example 2 from Source Code of the course "Geometric Modelling based on Polygonal Meshes" */
void CurvatureEstimator::calculateWeights() {
	ZahirMesh::VertexIter        v_it, v_end(mesh.vertices_end());
	ZahirMesh::EdgeIter          e_it, e_end(mesh.edges_end());
	ZahirMesh::VertexFaceIter    vf_it;
	ZahirMesh::FaceVertexIter    fv_it;
	ZahirMesh::HalfedgeHandle    h0, h1, h2;
	ZahirMesh::VertexHandle      v0, v1;
	ZahirMesh::Point             p0, p1, p2, d0, d1;
	ZahirMesh::Scalar            w, area, b(0.99);
		
	for (e_it = mesh.edges_begin(); e_it != e_end; ++e_it) {
		w  = 0.0;		
	    h0 = mesh.halfedge_handle(e_it.handle(), 0);
	    v0 = mesh.to_vertex_handle(h0);
	    p0 = mesh.point(v0);
			
	    h1 = mesh.halfedge_handle(e_it.handle(), 1);
	    v1 = mesh.to_vertex_handle(h1);
	    p1 = mesh.point(v1);
			
	    if (!mesh.is_boundary(h0)) { 
	    	h2 = mesh.next_halfedge_handle(h0);
	    	p2 = mesh.point(mesh.to_vertex_handle(h2));
	    	d0 = (p0 - p2).normalize();
	    	d1 = (p1 - p2).normalize();
	    	w += 1.0 / tan(acos(std::max(-b, std::min(b, (d0|d1)))));
	    }
	    
	    if (!mesh.is_boundary(h1)) {
	    	h2 = mesh.next_halfedge_handle(h1);
	    	p2 = mesh.point(mesh.to_vertex_handle(h2));
	    	d0 = (p0 - p2).normalize();
	    	d1 = (p1 - p2).normalize();
	    	w += 1.0 / tan(acos(std::max(-b, std::min(b, (d0|d1)))));
	    }
	    
	    // force weights to be non-negative for higher robustness
	    w = std::max(w, 0.0f);
	    
	    weight(e_it) = w;
	} 
	
	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) {
		area = 0.0;
			
	    for (vf_it = mesh.vf_iter(v_it); vf_it; ++vf_it) {
	    	fv_it = mesh.fv_iter(vf_it);
	    	const ZahirMesh::Point& P = mesh.point(fv_it);  ++fv_it;
	    	const ZahirMesh::Point& Q = mesh.point(fv_it);  ++fv_it;
	    	const ZahirMesh::Point& R = mesh.point(fv_it);
	    	area += ((Q-P)%(R-P)).norm() * 0.5f * 0.3333f;
	    }
	    
	    weight(v_it) = (fabs(area) > FLT_MIN ? 1.0 / (2.0 * area) : 0.0);
	}
}

/* adapted from the code in trimesh2 library by Szymon Rusinkiewicz */
void CurvatureEstimator::calculateCurvature() {
	ZahirMesh::VertexIter		v_it, v_end(mesh.vertices_end());
	ZahirMesh::FaceIter 		f_it, f_end(mesh.faces_end());
	ZahirMesh::FaceVertexIter	fv_it;
	
	ZahirMesh::VertexHandle 	vh;
	// Cálculo de direcciones principales para cada punto
	
	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it) {
		fv_it = mesh.fv_iter(f_it); // iterador de vértices sobre esta cara
		
		const ZahirMesh::Point& V1 = mesh.point(fv_it); ZahirMesh::VertexHandle VH1 = fv_it.handle(); ++fv_it;
		const ZahirMesh::Point& V2 = mesh.point(fv_it); ZahirMesh::VertexHandle VH2 = fv_it.handle(); ++fv_it;
		const ZahirMesh::Point& V3 = mesh.point(fv_it); ZahirMesh::VertexHandle VH3 = fv_it.handle(); ++fv_it;
		pdir1(VH1) = V2 - V1;
		pdir1(VH2) = V3 - V2;
		pdir1(VH3) = V1 - V3;
	}
	
	
	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) {
		vh = v_it.handle();
		pdir1(vh) = pdir1(vh) % mesh.normal(vh);
		pdir2(vh) = mesh.normal(vh) % pdir1(vh);
		pdir1(vh).normalize();
		pdir2(vh).normalize();
	}
	
	// Cálculo de curvatura por cara 
	
	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it) {
		fv_it = mesh.fv_iter(f_it); // iterador de vértices sobre esta cara
				
		const ZahirMesh::Point& V0 = mesh.point(fv_it); ZahirMesh::VertexHandle VH0 = fv_it.handle(); ++fv_it;
		const ZahirMesh::Point& V1 = mesh.point(fv_it); ZahirMesh::VertexHandle VH1 = fv_it.handle(); ++fv_it;
		const ZahirMesh::Point& V2 = mesh.point(fv_it); ZahirMesh::VertexHandle VH2 = fv_it.handle(); ++fv_it;
		
		ZahirMesh::Point 	e[3] = { V1 - V2, V2 - V0, V0 - V1 };
		OpenMesh::Vec3f 	n[3] = { mesh.normal(VH0), mesh.normal(VH1), mesh.normal(VH2) };
		
		OpenMesh::Vec3f	t = e[0];
		t.normalize();
		
		OpenMesh::Vec3f normal = e[0] % e[1];
		normal.normalize();
		
		OpenMesh::Vec3f b = normal % t;
		b.normalize();
		
		float m[3] 		= { 0.0, 0.0, 0.0};
		float w[3][3] 	= { {0.0, 0.0, 0.0}, 
							{0.0, 0.0, 0.0}, 
							{0.0, 0.0, 0.0} };
		
		for (int j = 0; j < 3; j++){
			float u 	= e[j] | t;
			float v 	= e[j] | b;
			w[0][0] 	+= u*u;
			w[0][1] 	+= u*v;
			w[2][2] 	+= v*v;
			int sigte 	= (j)<2 ? (j)+1 : (j)-2;
			int prev 	= (j)>0 ? (j)-1 : (j)+2;
			OpenMesh::Vec3f dn 	= n[prev] - n[sigte];
			float dnu 	= dn | t;
			float dnv 	= dn | b;
			m[0] 		+= dnu*u;
			m[1] 		+= dnu*v + dnv*u;
			m[2] 		+= dnv*v;
		}
		w[1][1] = w[0][0] + w[2][2];
		w[1][2] = w[0][1];
		
		// Aproximamos II mediante mínimos cuadrados.
	
		float diag[3];
		//TODO: manejar el caso de un triángulo sin curvatura.
		if(!ldltdc<float,3>(w, diag)) {
			//assert(false);
			continue;
		}

		ldltsl<float,3>(w, diag, m, m);
		 
		// Asociamos la curvatura "promedio" a los vertices
		//TODO: utilizar el promedio del paper, con pesos, no el promedio simple. 
		int j = 0;
		for (fv_it = mesh.fv_iter(f_it); j < 3; ++fv_it){
//			fv_it = mesh.fv_iter(f_it); // iterador de vértices sobre esta cara //TODO: esta variable está siendo usada más arriba ...
			ZahirMesh::VertexHandle vj = fv_it.handle(); 
			
			//int vj = triangulo_actual->obtenerVertice(j)->obtenerPunto()->obtenerIndice();
			float c1, c12, c2;
			proj_curv(t, b, m[0], m[1], m[2], pdir1(vj), pdir2(vj), c1, c12, c2);
			//float wt = 1.0f / (float) indices_triangulos_asociados[vj].size();
			float wt = weight(vj);
			cv1(vj)  += wt * c1;
			cv12(vj) += wt * c12;
			cv2(vj)  += wt * c2;
			++j;
//			++fv_it;
		}
	}
		
	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) {
		vh = v_it.handle();
		diagonalize_curv(pdir1(vh), pdir2(vh), cv1(vh), cv12(vh), cv2(vh), mesh.normal(vh), pdir1(vh), pdir2(vh), cv1(vh), cv2(vh));
	}
	
}

void CurvatureEstimator::calculateCurvatureDerivatives() {
	// Cálculo de derivada curvatura por cara 
	
	ZahirMesh::VertexIter		v_it, v_end(mesh.vertices_end());
	ZahirMesh::FaceIter 		f_it, f_end(mesh.faces_end());
	ZahirMesh::FaceVertexIter	fv_it;
	
	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it) {
		fv_it = mesh.fv_iter(f_it); // iterador de vértices sobre esta cara
						
		const ZahirMesh::Point& V0 = mesh.point(fv_it); ZahirMesh::VertexHandle VH0 = fv_it.handle(); ++fv_it;
		const ZahirMesh::Point& V1 = mesh.point(fv_it); ZahirMesh::VertexHandle VH1 = fv_it.handle(); ++fv_it;
		const ZahirMesh::Point& V2 = mesh.point(fv_it); ZahirMesh::VertexHandle VH2 = fv_it.handle(); ++fv_it;
		
		ZahirMesh::Point 	e[3] = { V1 - V2, V2 - V0, V0 - V1 };
		OpenMesh::Vec3f 	n[3] = { mesh.normal(VH0), mesh.normal(VH1), mesh.normal(VH2) };
		
		OpenMesh::Vec3f	t = e[0];
		t.normalize();
		
		OpenMesh::Vec3f normal = e[0] % e[1];
		normal.normalize();
		
		OpenMesh::Vec3f b = normal % t;
		b.normalize();
		
		float face_curvature[3][3];
		
		int j = 0;
		for (fv_it = mesh.fv_iter(f_it); j < 3; ++fv_it){
			ZahirMesh::VertexHandle vj = fv_it.handle(); 
			proj_curv(pdir1(vj), pdir2(vj), cv1(vj), 0, cv2(vj), t, b, face_curvature[j][0], face_curvature[j][1], face_curvature[j][2]);
			j++;
		}
		
		float m[4] 		= {  0,0,0,0};
		float w[4][4] 	= { {0,0,0,0}, 
							{0,0,0,0}, 
							{0,0,0,0}, 
							{0,0,0,0} };
		
		for (j = 0; j < 3; j++) {
			int sigte 	= (j) < 2 ? (j) + 1 : (j) - 2;
			int prev 	= (j) > 0 ? (j) - 1 : (j) + 2;
			float dfcurv[3] = {	face_curvature[prev][0] - face_curvature[sigte][0],
								face_curvature[prev][1] - face_curvature[sigte][1],
								face_curvature[prev][2] - face_curvature[sigte][2]
			};
			
			float u = e[j] | t;
			float v = e[j] | b;
			float u2 = u*u, v2 = v*v, uv = u*v;
			w[0][0] 	+= u2;
			w[0][1] 	+= uv;
			w[3][3] 	+= v2;
			m[0] 		+= u*dfcurv[0];
			m[1] 		+= v*dfcurv[0] + 2.0f * u * dfcurv[1];
			m[2] 		+= 2.0f * v * dfcurv[1] + u * dfcurv[2];
			m[3] 		+= v * dfcurv[2];
		}
		
		w[1][1] = 2.0f * w[0][0] + w[3][3];
		w[1][2] = 2.0f * w[0][1];
		w[2][2] = w[0][0] + 2.0f * w[3][3];
		w[2][3] = w[0][1];
		
		// Aproximamos mediante mínimos cuadrados.
		
		float d[4];
		
		if(!ldltdc<float,4>(w, d)){
//			assert(false);
			continue;
		}
		
		ldltsl<float,4>(w, d, m, m);
		
		OpenMesh::Vec4f face_dcurv(m);
		j = 0;
		for (fv_it = mesh.fv_iter(f_it); j < 3; ++fv_it){
			ZahirMesh::VertexHandle vj = fv_it.handle();
			
			OpenMesh::Vec4f this_vert_dcurv;
			proj_dcurv(t, b, face_dcurv, pdir1(vj), pdir2(vj), this_vert_dcurv);
			deriv(vj) += weight(vj) * this_vert_dcurv;
			j++;
		}
	}
}
