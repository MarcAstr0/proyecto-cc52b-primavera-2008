/*
  Copyright (c) 2005
	  Aaron Lefohn	(lefohn@cs.ucdavis.edu)
	  Robert Strzodka	(strzodka@caesar.de)
  All rights reserved.

  This software is licensed under the BSD open-source license. See
  http://www.opensource.org/licenses/bsd-license.php for more detail.

  *************************************************************
  Redistribution and use in source and binary forms, with or 
  without modification, are permitted provided that the following 
  conditions are met:

  Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer. 

  Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution. 

  Neither the name of the University of Californa, Davis, the Center
  of Advanced European Studies and Research (Caesar) nor the names of 
  the contributors may be used to endorse or promote products derived 
  from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
  OF SUCH DAMAGE.
*/

#ifndef UCDAVIS_KEYED_FRAMEBUFFER_OBJECT_H
#define UCDAVIS_KEYED_FRAMEBUFFER_OBJECT_H

#include <GL/glew.h>
#include <framebufferObject.h>
#include "keyedObject.h"

// Struct that serves as FBO key
class FboSizeFormatKey 
{
public:
	FboSizeFormatKey(); // Intentionally undefined default ctor

  FboSizeFormatKey(int w, int h, GLenum f) 
		: m_width(w),
		  m_height(h),
		  m_format(f)
	{}

	FboSizeFormatKey(GLenum texTarget, GLuint texId)
	{
		glPushAttrib(GL_TEXTURE_BIT);
		glBindTexture( texTarget, texId );
		glGetTexLevelParameteriv( texTarget, 0, GL_TEXTURE_WIDTH, (GLint*)&m_width );
		glGetTexLevelParameteriv( texTarget, 0, GL_TEXTURE_HEIGHT, (GLint*)&m_height );
		glGetTexLevelParameteriv( texTarget, 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*)&m_format );
		glPopAttrib();
	}

	bool operator<(const FboSizeFormatKey& rhs) const {
		if(m_width  != rhs.m_width)  return ( m_width  < rhs.m_width  );
		if(m_height != rhs.m_height) return ( m_height < rhs.m_height );
		                             return ( m_format < rhs.m_format );
	}

	int		  Width() const  { return m_width; }
	int		  Height() const { return m_height; }
	GLenum	Format() const { return m_format; }

private:
	int    m_width;
	int    m_height;
	GLenum m_format;
};

// Keyed FBO class
// - This class wraps an FBO and adds a key that identifies the FBO
//   to the FBO Manager by size and format. 
//
// - It extends the functionality of KeyedObject<K,V> by adding
//   runtime checks that ensure the attached textures and renderbuffers
//   match the declared key's size and format.
//
// - A KeyedFramebufferObject behaves just like a normal FBO
//   and its extistence can be completely hidden from the user.
//
// - Compiling with NDEBUG removes checking and makes this class identical
//   to a KeyedObject<FboSizeFormatKey, FramebufferObject>.
class KeyedFramebufferObject : public KeyedObject<FboSizeFormatKey, 
												                          FramebufferObject>
{
public:
	typedef	FboSizeFormatKey				        key_type;
	typedef FramebufferObject				        value_type;
	typedef KeyedObject<key_type, 
					            FramebufferObject>	base_type;

	KeyedFramebufferObject( const key_type& key );
	virtual ~KeyedFramebufferObject();

	// Public interface...
	//  - See FramebufferObject

#ifndef NDEBUG
	/// Checks size of texture/renderbuffer if in debug mode (no -DNDEBUG)
	/// - In a release build, the base class methods will be called.

	/// Bind a texture to the "attachment" point of this FBO
	virtual void AttachTexture( GLenum texTarget, GLuint texId,
								              GLenum attachment = GL_COLOR_ATTACHMENT0_EXT,
								              int mipLevel      = 0,
								              int zSlice        = 0 );

	/// Bind a render buffer to the "attachment" point of this FBO
	virtual void AttachRenderBuffer( GLuint buffId,
                                   GLenum attachment = GL_COLOR_ATTACHMENT0_EXT );
#endif

private:
	KeyedFramebufferObject(); // Intentionally undefined (and private) default ctor
};

#endif
