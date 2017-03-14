#ifndef CGCONTEXT_H_
#define CGCONTEXT_H_

#include <stdio.h>
#include <assert.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <string>
#include "../Color.h"
#include "../FuenteDeLuz.h"
#include "../Texturas/Textura.h"
#include "../Material.h"

/* Cg Context */

class CgContext {
public:
	CgContext() {};
	virtual ~CgContext() {};
	void initContext() { 
		mCgContext = cgCreateContext(); 
		mCgVertexProfile   = cgGLGetLatestProfile(CG_GL_VERTEX);
		mCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	};
	void initContext(std::string vertex_profile, std::string fragment_profile) {
		mCgContext = cgCreateContext();
		mCgVertexProfile   = cgGetProfile(vertex_profile.c_str());
		mCgFragmentProfile = cgGetProfile(fragment_profile.c_str());		
	};
	void destroyContext() { cgDestroyContext(mCgContext); };
	bool isValidVertexProfile() const { return mCgVertexProfile != CG_PROFILE_UNKNOWN; };
	bool isValidFragmentProfile() const { return mCgFragmentProfile != CG_PROFILE_UNKNOWN; };
	std::string getCgError() const { return std::string(cgGetErrorString( cgGetError() )); };
	const CGcontext& getContext() const { return mCgContext; };
	const CGprofile& vertexProfile() const { return mCgVertexProfile; };
	const CGprofile& fragmentProfile() const { return mCgFragmentProfile; };
	std::string vertexProfileName() const { return std::string(cgGetProfileString( mCgVertexProfile )); };
	std::string fragmentProfileName() const { return std::string(cgGetProfileString( mCgFragmentProfile )); };
	
	/// from nvidia's examples
	void checkForErrors(std::string class_name, std::string method) {
		CGerror error;
		const char *string = cgGetLastErrorString(&error);
		
		if (error != CG_NO_ERROR) {
			printf("ERROR IN %s, %s: %s\n", class_name.c_str(), method.c_str(), string);
			if (error == CG_COMPILER_ERROR)
				printf("%s\n", cgGetLastListing(mCgContext));
			assert(false);
			exit(1);
		}
	};
		
private:
	CGcontext mCgContext;
	CGprofile mCgVertexProfile;
	CGprofile mCgFragmentProfile;
};

typedef OpenMesh::SingletonT<CgContext> CgSingleContext;


/*  Cg Program */

class CgProgram {
public:
	enum 			cg_program_type {VERTEX_PROGRAM, FRAGMENT_PROGRAM, GEOMETRY_SHADER};
	enum			cggl_matrix_type {MODELVIEW_MATRIX, MODELVIEW_PROJECTION_MATRIX, PROJECTION_MATRIX};
	
private:
	bool			loaded;
	cg_program_type	type;
	CGprogram 		mCgShader;
	
public:
	CgProgram(cg_program_type _type): loaded(false), type(_type) {};
	CgProgram(const CgProgram& cgp): loaded(cgp.loaded), type(cgp.type), mCgShader(cgp.mCgShader) {};
	CgProgram& operator=(const CgProgram& cgp) { 
		if (this != &cgp) {
			loaded = cgp.loaded;
			type = cgp.type;
			mCgShader = cgp.mCgShader;
		}	
		return *this;
	};
	virtual ~CgProgram() {};
	void destroy() { cgDestroyProgram(mCgShader); };
	friend class CgContext;
	
	bool isLoaded() const { return loaded; };
	
	/* loaders */
	
	void loadFromFile(std::string filename, std::string entry_point = "main") {
		
		switch (type) {
		case VERTEX_PROGRAM:
			mCgShader = cgCreateProgramFromFile(CgSingleContext::Instance().getContext(), CG_SOURCE, filename.c_str(), CgSingleContext::Instance().vertexProfile(), entry_point.c_str(), NULL);
			break;
		case FRAGMENT_PROGRAM:
			mCgShader = cgCreateProgramFromFile(CgSingleContext::Instance().getContext(), CG_SOURCE, filename.c_str(), CgSingleContext::Instance().fragmentProfile(), entry_point.c_str(), NULL);
			break;
		case GEOMETRY_SHADER:
			
			break;
		}
		if (mCgShader) {
			cgGLLoadProgram(mCgShader);
			loaded = true;
		} else {
			std::cerr << CgSingleContext::Instance().getCgError() << std::endl;
			loaded = false;
		}
	};
	
	/* context related */
	
	void bind() const {
		assert(loaded);
		switch (type) {
		case VERTEX_PROGRAM:
			cgGLEnableProfile(CgSingleContext::Instance().vertexProfile());
			break;
		case FRAGMENT_PROGRAM:
			cgGLEnableProfile(CgSingleContext::Instance().fragmentProfile());
			break;
		case GEOMETRY_SHADER:
		default:
			break;
		}
		cgGLBindProgram(mCgShader);
	};
	
	void unBind() const {
		assert(loaded);
		
		switch (type) {
		case VERTEX_PROGRAM:
			cgGLDisableProfile(CgSingleContext::Instance().vertexProfile());
			break;
		case FRAGMENT_PROGRAM:
			cgGLDisableProfile(CgSingleContext::Instance().fragmentProfile());
			break;
		case GEOMETRY_SHADER:
		default:
			break;
		}
	};
	
	/* uniform parameters */
	
	void setMatrixParameter(std::string matrix_name, cggl_matrix_type matrix_type) {
		CGparameter	matrix_param = cgGetNamedParameter(mCgShader, matrix_name.c_str());
		switch (matrix_type) {
		case MODELVIEW_MATRIX:
			cgGLSetStateMatrixParameter(matrix_param, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
			break;
		case MODELVIEW_PROJECTION_MATRIX:
			cgGLSetStateMatrixParameter(matrix_param, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
			break;
		case PROJECTION_MATRIX:
			cgGLSetStateMatrixParameter(matrix_param, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
			break;
		}
	};
	
	void setFloatParameter(std::string param_name, float value) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter1f(cgParam, value);
	};
	
	void setIntParameter(std::string param_name, int value) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter1i(cgParam, value);
	};
	
	void setBoolParameter(std::string param_name, bool value) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter1i(cgParam, (int) value);
	};
	
	/* vector parameters */
	
	void setVec2fParameter(std::string param_name, const OpenMesh::Vec2f& v) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter2fv(cgParam, v.data());
	};
	
	void setVec3fParameter(std::string param_name, const OpenMesh::Vec3f& v) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter3fv(cgParam, v.data());
	};
	
	void setVec4fParameter(std::string param_name, const OpenMesh::Vec4f& v) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter4fv(cgParam, v.data());
	};
	
	void setVec2dParameter(std::string param_name, const OpenMesh::Vec2d& v) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter2dv(cgParam, v.data());
	};
	
	void setVec3dParameter(std::string param_name, const OpenMesh::Vec3d& v) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter3dv(cgParam, v.data());
	};
	
	void setVec4dParameter(std::string param_name, const OpenMesh::Vec4d& v) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter4dv(cgParam, v.data());
	};
	
	/* graphic classes */
	
	void setColorParameter(std::string param_name, const Color& c, bool include_alpha = false) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		if (include_alpha) 
			cgSetParameter4fv(cgParam, c.dataPointer());
		else 
			cgSetParameter3fv(cgParam, c.dataPointer());
	};
	
	void setLightSourceParameter(std::string param_name, const FuenteDeLuz& f) {
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgSetParameter4fv(cgParam, f.obtenerPosicionTransformada().data());
	};
	
	void setMaterialParameter(const Material& m) {
		setColorParameter("Kd", m.obtenerKd());
		setColorParameter("Ka", m.obtenerKa());
		setColorParameter("Ks", m.obtenerKs());
		setFloatParameter("shininess", m.obtenerBrillosidad());
	};
	
	/* textures */
	
	void bindTexture(std::string param_name, const Textura& t){
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgGLEnableTextureParameter(cgParam);
		cgGLSetTextureParameter(cgParam, t.obtenerIdentificador());
	};
	
	void unbindTexture(std::string param_name){
		CGparameter cgParam = cgGetNamedParameter(mCgShader, param_name.c_str());
		cgGLDisableTextureParameter(cgParam);
	};
	
	/* varying parameters (not implemented yet) */
};




#endif /*CGCONTEXT_H_*/
