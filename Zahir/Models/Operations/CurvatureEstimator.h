#ifndef CURVATUREESTIMATOR_H_
#define CURVATUREESTIMATOR_H_

#include "../Modelo3D.h"

class CurvatureEstimator
{
public:
	CurvatureEstimator(ZahirMesh& m, 
			OpenMesh::VPropHandleT<ZahirMesh::Scalar>& cv1, 
			OpenMesh::VPropHandleT<ZahirMesh::Scalar>& cv2, 
			OpenMesh::VPropHandleT<OpenMesh::Vec3f>& pdir1, 
			OpenMesh::VPropHandleT<OpenMesh::Vec3f>& pdir2,
			OpenMesh::VPropHandleT<OpenMesh::Vec4f>& curvderiv);
	virtual ~CurvatureEstimator();
	void calculate();
	
protected:
	/// Core Functions
	void calculateWeights();
	void calculateCurvature();
	void calculateCurvatureDerivatives();
	
	/// Auxiliary Accessors
	ZahirMesh::Scalar& weight(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vweight, _vh);
	};
	ZahirMesh::Scalar& weight(ZahirMesh::EdgeHandle _eh) { 
		return mesh.property(eweight, _eh); 
	};
	ZahirMesh::Scalar& cv1(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vcurv1, _vh);
	};
	ZahirMesh::Scalar& cv2(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vcurv2, _vh);
	};
	ZahirMesh::Scalar& cv12(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vcurv12, _vh);
	};
	OpenMesh::Vec3f& pdir1(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vpdir1, _vh);
	};
	OpenMesh::Vec3f& pdir2(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vpdir2, _vh);
	};
	OpenMesh::Vec4f& deriv(ZahirMesh::VertexHandle _vh) {
		return mesh.property(vcurvderiv, _vh);
	};
	
	/// Auxiliary Properties
	OpenMesh::VPropHandleT<ZahirMesh::Scalar>  vweight;
	OpenMesh::EPropHandleT<ZahirMesh::Scalar>  eweight;
	OpenMesh::VPropHandleT<ZahirMesh::Scalar>  vcurv12;
	
	/// Mesh References
	ZahirMesh& mesh;
	OpenMesh::VPropHandleT<ZahirMesh::Scalar>&  vcurv1;
	OpenMesh::VPropHandleT<ZahirMesh::Scalar>&  vcurv2;
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>&  vpdir1;
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>&  vpdir2;
	OpenMesh::VPropHandleT<OpenMesh::Vec4f>&  vcurvderiv;
};

#endif /*CURVATUREESTIMATOR_H_*/
