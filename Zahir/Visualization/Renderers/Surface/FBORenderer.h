#ifndef FBORENDERER_H_
#define FBORENDERER_H_

#include <GL/glew.h>
#include "SurfaceRenderer.h"
#include <Zahir/External/fboClass/framebufferObject.h>
#include <Zahir/External/fboClass/renderbuffer.h>
#include <Zahir/Visualization/GPU/CgWrapper.h>
#include "../Image/ImageRenderer.h"
#include "../../Texturas/CargadorTexturas.h"

/** por el momento simplemente es un copy paste del tutorial de gpgpu */

class FBORenderer: public SurfaceRenderer
{
    public:
	FBORenderer(SurfaceRenderer* sr, int width, int height):
            fragment_shader(CgProgram::FRAGMENT_PROGRAM),
            _fbo(),
            _rb(),  // optional: create a new render buffer object
            _iWidth(width), _iHeight(height) {
		surface_renderer = sr;
		
		// textures
		int textures = 2; // solía ser 2
		for (int i = 0; i < textures; i++) {
//                     _iTexture[i] = CargadorTexturas::allocateDepthTexture(_iWidth, _iHeight);
                    _iTexture[i] = CargadorTexturas::allocateTexture2D(_iWidth, _iHeight);
		}
	    
//                 ahora el programa en Cg
                std::string zahir_path = getenv("ZAHIR_PATH");
//                 fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/LaplacianFilter/fragment_program.cg");
                fragment_shader.loadFromFile(zahir_path + "Zahir/Visualization/Renderers/Surface/SobelFilter/fragment_program.cg");
	   
                _fbo.Bind(); // Bind framebuffer object.
//                 _rb.Bind();
            
                // Attach texture to framebuffer color buffer
                _fbo.AttachTexture(GL_TEXTURE_2D, _iTexture[0], GL_COLOR_ATTACHMENT0_EXT);
//                 _fbo.AttachTexture(GL_TEXTURE_2D, _iTexture[1], GL_COLOR_ATTACHMENT1_EXT);
	    
                // Optional: initialize depth renderbuffer
                _rb.Set( GL_DEPTH_COMPONENT24, _iWidth, _iHeight );
                _fbo.AttachRenderBuffer( _rb.GetId(), GL_DEPTH_ATTACHMENT_EXT );
		
                fbo.asignarIdentificador(_iTexture[0]);
	    
                // Validate the FBO after attaching textures and render buffers
                _fbo.IsValid();
            
                // Disable FBO rendering for now...
                FramebufferObject::Disable();
	    
                image_renderer = new ImageRenderer();
	};
	
	virtual ~FBORenderer() {
            fbo.eliminarTextura();
            delete image_renderer;
	};
	
	const Textura& getRenderedTexture() { return fbo; };
	
	void preparar(Modelo3D* model, Material*) 
	{
            glGetIntegerv(GL_DRAW_BUFFER, &_currentDrawbuf); // Save the current Draw buffer
            _fbo.Bind(); // Render to the FBO
	    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); // Draw into the first texture
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    
            surface_renderer->graficar(model);
            
            FramebufferObject::Disable();
	    glDrawBuffer(_currentDrawbuf);
	};
	
	void ejecutar(Modelo3D* model, Material*) 
	{
            fragment_shader.bind();
            fragment_shader.bindTexture("texture", fbo);
            
            image_renderer->graficar(fbo);
            
            fragment_shader.unBind();
	};
	
	void finalizar(Modelo3D*,Material*) {};
	
    protected:
        SurfaceRenderer* surface_renderer;
        ImageRenderer* image_renderer;
        Textura fbo;
        CgProgram fragment_shader;
        
        int _iWidth, _iHeight; // The dimensions of our array
        
        GLuint _iTexture[2]; // The texture used as a data array
        FramebufferObject _fbo; // The framebuffer object used for rendering to the texture
        Renderbuffer _rb; // Optional: The renderbuffer object used for depth
        
        GLint _currentDrawbuf;
        
        bool needs_depth_buffer;
};

#endif /*FBORENDERER_H_*/
