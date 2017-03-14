#ifndef CARGADORTEXTURAS_H_
#define CARGADORTEXTURAS_H_

#include <GL/glew.h>
#include <iostream>
#include <QtGui>
#include <QtOpenGL>
#include <vector>
#include "Textura.h"
#include <assert.h>
#include <algorithm>


class CargadorTexturas
{
public:
	static unsigned int cargarTextura1D(float* textura_1D, int n) {
		unsigned int identificador;
		int width = 64; //TODO: ojo que se asume que mide menos de 64 ...
		
		// texturas 1d, mínimo 64 píxeles de ancho (límites de opengl)
		
		unsigned char *textura = (unsigned char*) malloc(sizeof(unsigned char) * width);
		assert(textura); 
		
		for (int i = 0; i < width; i++) {
			int indice = ceil(float((n - 1) * i) / float(width));
			
			float valor = textura_1D[indice];
			textura[i] = 255.0f * valor;
		}
		
		glGenTextures(1, &identificador);
		
		glBindTexture(GL_TEXTURE_2D, identificador);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, 1, width, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textura);

		free(textura);
		
		return identificador;
	};
	
	static unsigned int cargarTextura(std::string filename) {
		QImage t;
		QImage b;
				
		if ( !b.load(QString(filename.c_str())) ) {
			std::cerr << "can't load texture. " << std::endl;
			return 0;
		}
		
		std::cerr << "texture " << filename << ": " << b.width() << "x" << b.height() << " px" << std::endl;
		
		unsigned int identificador = 0;
		    
		t = QGLWidget::convertToGLFormat( b );
		glGenTextures( 1, &identificador );
		glBindTexture( GL_TEXTURE_2D, identificador );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		return identificador;
	};
	static unsigned int cargarTextura3D(std::vector<std::string> filenames, int u = 256, int v = 256, int d = 8) {
		assert(filenames.size() > 0 && filenames.size() == d);
		reverse(filenames.begin(), filenames.end());
		std::vector<QImage> images;
		images.resize(d);
		for (int i = 0; i < d; i++) {
			QImage im;
			if ( !im.load(QString(filenames.at(i).c_str())))
				return 0;
			if (im.height() != v && im.width() != u)
				im = im.scaled(u, v);
			images.at(i) = QGLWidget::convertToGLFormat( im );
		}
//		return 0;
		unsigned int identificador;
		
		uchar* textura = (uchar*) malloc(4 * u * v * images.size() * sizeof(uchar));
		if (textura == NULL) 
			return 0;
		
		for (int i = 0; i < d; i++) {
			
			// buscar la imagen que esté más cerca a esta profundidad
//			int indice = (i + 1) / d * (filenames.size() - 1);
			
			// copiarla al arreglo de textura
			memcpy(textura + i * u * v * 4, images.at(i).bits(), u * v * 4);
		}
		
		glGenTextures( 1, &identificador );
		glBindTexture( GL_TEXTURE_3D, identificador );
		//TODO: en windows puede hacer caput ! hay que usar glew para recuperar el puntero a la función
		glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA, u, v, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, textura);
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );
		
		free(textura);
		
		return identificador;
	};
	
	static unsigned int allocateTexture2D(int width, int height) {
		unsigned int identifier;
		glGenTextures(1, &identifier); // create (reference to) a new texture
	    
	    glBindTexture(GL_TEXTURE_2D, identifier);
	    // (set texture parameters here)
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	    //create the texture
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_FLOAT, 0);
	    return identifier;
	};
	static unsigned int allocateDepthTexture(int width, int height) {
		
	    // Check that extension is supported
	    if (glewGetExtension("GL_ARB_depth_texture") != GL_TRUE) {
	    	std::cerr << "GL_ARB_depth_texture not supported" << std::endl;
	        return 0;
	    }

	    unsigned int identifier = 0;
	    
	    // Create texture
	    glGenTextures(1, &identifier);
	    
	    if (identifier == 0)
	    	return identifier;

	    // Make texture active
	    glBindTexture(GL_TEXTURE_2D, identifier);

	    // Fiddle with texture parameters
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	    // Set DEPTH_TEXTURE_MODE to LUMINANCE
	    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	    // Initialize texture memory using DEPTH_TEXTURE for internal format and format
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	    return identifier;	
	};
};

/*
 * 
 * 
 * code from gamedev forums
 * 
void LoadTexture(string textname)
{
	GLuint textureNumber;
	string filename = DATA_DIR + textname + string(".png");
	SDL_Surface* surface = IMG_Load(filename.c_str());
	SDL_Rect zero;
	zero.h = surface->h;
	zero.w = surface->w;
	zero.x = 0;
	zero.y = 0;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int rmask = 0xff000000;
    int gmask = 0x00ff0000;
    int bmask = 0x0000ff00;
    int amask = 0x000000ff;
#else
    int rmask = 0x000000ff;
    int gmask = 0x0000ff00;
    int bmask = 0x00ff0000;
    int amask = 0xff000000;
#endif

	SDL_Surface* converted_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, 24, 
                                  rmask, gmask, bmask, amask);

	SDL_BlitSurface(surface, &zero, converted_surface, &zero);

	glGenTextures(1, &textureNumber);
	textures[textname] = textureNumber;
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	GLvoid* data = converted_surface->pixels;
	cout << textname << "'s format is... " << endl;
	cout << "Bits per pixel: " << (int)converted_surface->format->BitsPerPixel << endl;
	cout << endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(converted_surface);
}

*/

#endif /*CARGADORTEXTURAS_H_*/
