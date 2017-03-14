#ifndef MESHCOLORIZER_H_
#define MESHCOLORIZER_H_

class MeshColorizer
{
public:
	MeshColorizer(ZahirMesh& m, OpenMesh::VPropHandleT< OpenMesh::Vec3f >& attribute):
		mesh(m), color_attribute(attribute) {
		
	};
	OpenMesh::Vec3f& color(ZahirMesh::VertexHandle _vh) {
		return mesh.property(color_attribute, _vh);
	};
	virtual ~MeshColorizer() {};
	virtual void colorize()=0;
	
protected:
	ZahirMesh& mesh;
	OpenMesh::VPropHandleT< OpenMesh::Vec3f >& color_attribute;
};

#endif /*MESHCOLORIZER_H_*/
