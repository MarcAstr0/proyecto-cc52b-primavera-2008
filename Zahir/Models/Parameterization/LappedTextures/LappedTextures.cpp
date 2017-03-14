// #include "LappedTextures.h"
// 
// /* PATCH */
// 
// LappedTextures::Patch::Patch(ZahirMesh& mesh): ptr_parent_mesh(&mesh) {
// 	submesh.add_property(dihedral_angle, "dihedral angle (tmp)");
// 	submesh.add_property(original_face_handle, "handle to the original face in the parent mesh (tmp)");
// }
// 
// LappedTextures::Patch::~Patch() {}
// 
// LappedTextures::Patch& LappedTextures::Patch::operator=(const Patch& p) {
// 	if (this != &p) {
// 		submesh = p.submesh;
// 		ptr_parent_mesh = p.ptr_parent_mesh;
// 		dihedral_angle = p.dihedral_angle;
// 		original_face_handle = p.original_face_handle;
// 		handle_mapping = p.handle_mapping;
// 	}
// 	return *this;
// }
// 
// void LappedTextures::Patch::add_face(ZahirMesh::FaceHandle fh) {
// 	// copiar los vértices de la cara original
// 	// asignar el mapeo a los vértices originales
// }
// 
// bool LappedTextures::Patch::can_add_face(ZahirMesh::FaceHandle fh) {
// 	return true;
// }
// 
// /* LAPPED TEXTURES */
// 
// LappedTextures::LappedTextures(ZahirMesh& _mesh): mesh(_mesh) {
// 	mesh.add_property(visited_faces_property, "visited faces (tmp)");
// 
// 	ZahirMesh::FaceIter f_it, f_end(mesh.faces_end());
// 
// 	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)
// 		set_visited(f_it.handle(), false);
// }
// 
// LappedTextures::~LappedTextures() {
// 	mesh.remove_property(visited_faces_property);
// }
// 
// void LappedTextures::process() {
// 		make_patches();
// 		//optimize();
// 		
// 		for (std::list<ZahirMesh>::iterator it = patches.begin(); it != patches.end(); it++)
// 			parameterize_patch(*it);
// }
// 
// void LappedTextures::grow_patch(ZahirMesh::FaceHandle fh){
// 	// Add its 1-Ring to the patch
// 	std::list<ZahirMesh::FaceHandle> added_faces;
// 	
// 	// This iterator represents the 1-Ring iterator.
// 	ZahirMesh::FaceFaceIter ff_it;
// 	
// 	for (ff_it = mesh.ff_iter(f_it.handle()); ff_it; ++ff_it) {
// 		if (can_grow(fh)) {
// 			add_face_to_patch(fh);
// 			added_faces.push_back(fh);
// 		}
// 	}
// 	
// 	/*
// 	// grow through the added faces.
// 	for (std::list<ZahirMesh::FaceHandle>::iterator it = added_faces.begin(); it != added_faces.end(); it++)
// 		grow_patch(*it);
// 	*/
// }
// 
// void LappedTextures::make_patches() {
// 	ZahirMesh::FaceIter f_it, f_end(mesh.faces_end());
// 	
// 	// Iterate over all faces
// 	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it) {
// 		if (is_visited(f_it.handle()))
// 			continue;
// 		
// 		add_face_to_patch(f_it.handle());
// 		set_visited(f_it.handle(), true);
// 		grow_patch(fh);
// 		// Add this face as base of the patch
// 	}
// }
// 
// void LappedTextures::parameterize_patch(ZahirMesh& patch) {
// 	
// }
