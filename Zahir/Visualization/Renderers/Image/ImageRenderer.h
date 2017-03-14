#ifndef IMAGERENDERER_H_
#define IMAGERENDERER_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include "../../Texturas/Textura.h"

//TODO: usar glPushAttribs, de modo de disactivar iluminación y otros posibles parámetros activos al momento de ejecutar el graficador.

class ImageRenderer
{
public:
	ImageRenderer(): pixel_1_to_1(false) {};
	virtual ~ImageRenderer() {};
	virtual void graficar(const Textura &t) {
		preparar(t);
		ejecutar(t);
		finalizar(t);
	};
protected:
	virtual void preparar(const Textura &t) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, t.obtenerIdentificador());
	};
	virtual void ejecutar(const Textura&) {
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
    	glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(1, 0); glVertex2f( 1, -1);
        glTexCoord2f(1, 1); glVertex2f( 1,  1);
        glTexCoord2f(0, 1); glVertex2f(-1,  1);
        glEnd();		
        glPopMatrix();
	};
	virtual void finalizar(const Textura&) {
		glDisable(GL_TEXTURE_2D);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	};
	
	bool pixel_1_to_1;
};

#endif /*IMAGERENDERER_H_*/
