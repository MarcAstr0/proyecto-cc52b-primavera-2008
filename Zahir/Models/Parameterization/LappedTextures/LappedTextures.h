// #ifndef LAPPEDTEXTURES_H_
// #define LAPPEDTEXTURES_H_
// 
// #include "../../Modelo3D.h"
// #include <map>
// 
// class LappedTextures
// {
// public:
// 	LappedTextures(ZahirMesh &mesh);
// 	virtual ~LappedTextures();
// 	void process();
// 	
// protected:
// 	/***
// 	 * Internal class. Holds a patch to be flattened and parameterized.
// 	 */
// 	//TODO: ver como se organizarán las coordenadas de textura.
// 	class Patch {
// 	public:
// 		Patch(ZahirMesh& mesh);
// 		virtual ~Patch();
// 		Patch& operator=(const Patch& p);
// 		void add_face(ZahirMesh::FaceHandle fh);
// 		bool can_add_face(ZahirMesh::FaceHandle fh);
// 	protected:		
// 		ZahirMesh*				ptr_parent_mesh;
// 		ZahirMesh						submesh;
// 		OpenMesh::EPropHandleT<float>	dihedral_angle;
// 		OpenMesh::FPropHandleT<ZahirMesh::FaceHandle> original_face_handle;
// 		std::map<int, int>		handle_mapping;
// 	};
// 	
// 	/***
// 	 * Auxiliary inline functions.
// 	 */
// 	bool is_visited(ZahirMesh::FaceHandle fh) {
// 		return mesh.property(visited_faces_property, fh);
// 	};
// 	void set_visited(ZahirMesh::FaceHandle fh, bool visited) {
// 		mesh.property(visited_faces_property, fh) = visited;
// 	};
// 	void close_patch(){
// 		patches.push_back(current_patch);
// 		current_patch = Patch(mesh);
// 	};
// 	void add_face_to_patch(ZahirMesh::FaceHandle fh) {
// 		// aquí agrego la cara
// 		current_patch.add_face(fh);
// 	};
// 	bool can_grow(ZahirMesh::FaceHandle fh) {
// 		if (is_visited(fh))
// 			return false;
// 		return current_patch.can_add_face(fh);
// 	};
// 	
// 	/***
// 	 * Key functions.
// 	 */
// 	void make_patches();
// 	void grow_patch(ZahirMesh::FaceHandle fh);
// 	bool can_grow(ZahirMesh::FaceHandle fh);
// 	void parameterize_patch(ZahirMesh& patch);
// 	void optimize() {};
// 	
// protected:
// 	OpenMesh::FPropHandleT<bool>  	visited_faces_property;
// 	ZahirMesh& 						mesh;
// 	
// 	std::list<Patch> 				patches;
// 	Patch 							current_patch;
// };
// 
// #endif /*LAPPEDTEXTURES_H_*/
