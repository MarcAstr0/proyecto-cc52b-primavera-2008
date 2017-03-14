/*
  Copyright (c) 2005
	  Aaron Lefohn	(lefohn@cs.ucdavis.edu)
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

#include "keyedFramebufferObject.h"
#include <iostream>
using namespace std;


KeyedFramebufferObject::KeyedFramebufferObject( const key_type& key )
  : base_type(key)
{}

KeyedFramebufferObject::~KeyedFramebufferObject()
{}

#ifndef NDEBUG
void
KeyedFramebufferObject::AttachTexture( GLenum texTarget, GLuint texId,
                                       GLenum attachment, int mipLevel, int zSlice)
{
  // Get m_width and m_height of texture being attached
  GLint width, height;
  glPushAttrib(GL_TEXTURE_BIT); // Save texture state
  glBindTexture(texTarget, texId);
  glGetTexLevelParameteriv(texTarget, mipLevel, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(texTarget, mipLevel, GL_TEXTURE_HEIGHT, &height);
  glPopAttrib();

  if ( ( texId != 0 ) && ( width != Key().Width() || height != Key().Height() ) ) {
    std::cerr << "KeyedFramebufferObject::AttachTexture WARNING:\n"
              << "\tAttaching texture with size (" << width << ", " << height << ") to FBO intended\n"
              << "\tfor texture with size (" << Key().Width() << ", " << Key().Height() << ")\n\n";
  }

  // Call real AttachTexture call
  FramebufferObject::AttachTexture(texTarget, texId, attachment, mipLevel, zSlice);
}

void 
KeyedFramebufferObject::AttachRenderBuffer( GLuint buffId, GLenum attachment )
{		
  // Get m_width and m_height of rendebuffer being attached
  GLint savedId = 0;
  glGetIntegerv(GL_RENDERBUFFER_BINDING_EXT, &savedId);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, buffId);

  GLint width, height;
  glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &width);
  glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, savedId);

  if ( ( buffId != 0 ) && ( width != Key().Width() || height != Key().Height() ) ) {
    std::cerr << "KeyedFramebufferObject::AttachRenderBuffer WARNING:\n"
              << "\tAttaching render buffer with size (" << width << ", " << height << ") to FBO intended\n"
              << "\tfor buffers with size (" << Key().Width() << ", " << Key().Height() << ")\n\n";
  }

  // Call real AttachRenderBuffer call
  FramebufferObject::AttachRenderBuffer(buffId, attachment);
}
#endif // End NDEBUG


