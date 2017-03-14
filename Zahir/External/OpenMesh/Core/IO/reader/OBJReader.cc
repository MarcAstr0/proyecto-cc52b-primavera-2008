/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *        Copyright (C) 2003 by Computer Graphics Group, RWTH Aachen         *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Lesser General Public License as published    *
 *  by the Free Software Foundation, version 2.1.                            *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Lesser General Public License for more details.                          *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public         *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/


#define LINE_LEN 4096

//== INCLUDES =================================================================


// OpenMesh
#include <OpenMesh/Core/IO/reader/OBJReader.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/color_cast.hh>
// STL
#if defined(OM_CC_MIPS)
#  include <ctype.h>
/// \bug Workaround for STLPORT 4.6: isspace seems not to be in namespace std!
#elif defined(_STLPORT_VERSION) && (_STLPORT_VERSION==0x460)
#  include <cctype>
#else
#  include <cctype>
using std::isspace;
#endif

#ifndef WIN32
#include <string.h>
#endif

//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


_OBJReader_  __OBJReaderInstance;
_OBJReader_& OBJReader() { return __OBJReaderInstance; }


//=== IMPLEMENTATION ==========================================================


_OBJReader_::
_OBJReader_() 
{ 
  IOManager().register_module(this); 
}


//-----------------------------------------------------------------------------


bool 
_OBJReader_::
read(const std::string& _filename, BaseImporter& _bi, Options& _opt)
{
  FILE* in = fopen(_filename.c_str(), "r");
  if (!in)
  {
    omerr() << "[OBJReader] : cannot not open file " 
          << _filename 
          << std::endl;
    return false;
  }

  {
#if defined(WIN32)
    std::string::size_type dot = _filename.find_last_of("\\/");
#else
    std::string::size_type dot = _filename.rfind("/");
#endif
    path_ = (dot == std::string::npos) 
      ? "./" 
      : std::string(_filename.substr(0,dot+1));
  }

  bool ok = read(in, _bi, _opt);
  
  fclose(in);
  return ok;
}


//-----------------------------------------------------------------------------

bool 
_OBJReader_::
read_mtl(FILE* _in)
{
  char   line[LINE_LEN];

  std::string key;
  Material    mat;
  float       f1,f2,f3;
  bool        indef = false;

  memset(line,0,LINE_LEN);
  while( _in && !feof(_in) && fgets(line, LINE_LEN, _in) )
  {
    if (line[0] == '#') // skip comments
    {
      memset(line,0,LINE_LEN);
      continue;
    }

    else if( isspace(line[0]) )
    {
      if (indef && !key.empty() && mat.is_valid())
      {
        materials_[key] = mat;
        mat.cleanup();
      }
    }
    
    else if (strncmp(line, "newmtl ", 7)==0) // begin new material definition
    {
      char *p0 = line+6, *p1;
      while( isspace(*++p0) ) {} ; p1=p0;
      while(!isspace(*p1)) ++p1; *p1='\0';
      key   = p0;
      indef = true;
    }

    else if (strncmp(line, "Kd ", 3)==0) // diffuse color
    {
      if (sscanf(line, "Kd %f %f %f", &f1, &f2, &f3))
        mat.set_Kd(f1,f2,f3);
    }

    else if (strncmp(line, "Ka ", 3)==0) // ambient color
    {
      if (sscanf(line, "Ka %f %f %f", &f1, &f2, &f3))
        mat.set_Ka(f1,f2,f3);
    }

    else if (strncmp(line, "Ks ", 3)==0) // specular color
    {
      if (sscanf(line, "Ks %f %f %f", &f1, &f2, &f3))
        mat.set_Ks(f1,f2,f3);
    }
#if 0
    else if (strncmp(line, "illum ", 6)==0) // diffuse/specular shading model
    {
      ; // just skip this
    }

    else if (strncmp(line, "Ns ", 3)==0) // Shininess [0..200]
    {
      ; // just skip this
    }

    else if (strncmp(line, "map_", 4)==0) // map images
    {
      // map_Kd, diffuse map
      // map_Ks, specular map
      // map_Ka, ambient map
      // map_Bump, bump map
      // map_d,  opacity map
      ; // just skip this
    }
#endif

    else if (strncmp(line, "Tr ", 3)==0 ) // transparency value
    {
      if (sscanf(line, "Tr %f", &f1))
        mat.set_Tr(f1);
    }
    else if (strncmp(line, "d ", 3)==0 ) // transparency value
    {
      if (sscanf(line, "d %f", &f1))
        mat.set_Tr(f1);
    }

    if (feof( _in ) && indef && mat.is_valid() && !key.empty())
      materials_[key] = mat;

    memset(line,0,LINE_LEN);

  }
  return true;
}

//-----------------------------------------------------------------------------


bool 
_OBJReader_::
read(FILE* _in, BaseImporter& _bi, Options& _opt)
{
  omlog() << "[OBJReader] : read file\n";


  char                   s[LINE_LEN];
  float                  x, y, z, u, v, w;

  BaseImporter::VHandles vhandles;
  std::vector<Vec3f>     normals;
  std::vector<Vec2f>     texcoords;

  std::map<std::string,Material> materials;
  std::string            matname;
  
  std::map<int, bool> 	textured_vertices;
  int 					textured_offset = 0;
  std::vector<Vec3f>	vertices;

  memset(&s, 0, LINE_LEN);

  while(_in && !feof(_in) && fgets(s, LINE_LEN, _in))
  {  
    // comment
    
	 if (s[0] == '#' || isspace(s[0])) continue;   
    
    // material file
    else if (strncmp(s, "mtllib ", 7)==0)
    {
      char mtlfile[256];

      if ( sscanf(s, "mtllib %s", mtlfile) )
      {
        std::string file = path_ + mtlfile;
        omlog() << "Load material file " << file << std::endl;

        FILE * _mtl = fopen( file.c_str(), "r" );
	if ( _mtl )
	{
	  if ( !read_mtl(_mtl) )
	    omerr() << "  Warning! Could not read file properly!\n";
	  fclose( _mtl );
	  omlog() << "  " << materials_.size() << " materials loaded.\n";
	}
	else
	  omerr() << "  Warning! Material file '" << file << "' not found!\n";
      }
    }

    // usemtl
    else if (strncmp(s, "usemtl ", 7)==0)
    {
      char *p0 = s+6, *p1;
      while( isspace(*++p0) ) {}; p1=p0;
      while(!isspace(*p1)) ++p1; *p1='\0';
      matname = p0;      
      if (materials_.find(matname)==materials_.end())
      {
        omerr() << "Warning! Material '" << matname 
              << "' not defined in material file.\n";
        matname="";
      }
    }

    // vertex
    else if (strncmp(s, "v ", 2) == 0)
    {
      if (sscanf(s, "v %f %f %f", &x, &y, &z))
        _bi.add_vertex(OpenMesh::Vec3f(x,y,z));
      	vertices.push_back(OpenMesh::Vec3f(x,y,z));
      	textured_vertices[_bi.n_vertices() - 1] = -1; // init all vertices "untextured"
    }


    // texture coord
    else if (strncmp(s, "vt ", 3) == 0)
    {
      int scanned = sscanf(s, "vt %f %f %f", &u, &v, &w);
      
      if (scanned == 2 || scanned == 3)
      {
        texcoords.push_back(OpenMesh::Vec2f(u, v));
        _opt += Options::VertexTexCoord;
      }
      else
      {
    	  std::cerr << "linea: " << s << std::endl;
        omerr() << "Only single 2D texture coordinate per vertex"
              << "allowed! (readed: " << scanned << ")" << std::endl;
        return false;
      }
    }


    // normal
    else if (strncmp(s, "vn ", 3) == 0)
    {
      if (sscanf(s, "vn %f %f %f", &x, &y, &z))
        normals.push_back(OpenMesh::Vec3f(x,y,z));
      _opt += Options::VertexNormal;
    }


    // face
    else if (strncmp(s, "f ", 2) == 0)
    {
      int component(0), nV(0);
      bool endOfVertex(false);
      char *p0, *p1(s+1);

      int current_v = -1;
      bool added_v = false;
      
      vhandles.clear();

      while (*p1 == ' ') ++p1; // skip white-spaces

      while (p1)
      {
        p0 = p1;

        // overwrite next separator

        // skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
        while (*p1 != '/' && *p1 != '\r' && *p1 != '\n' && 
               *p1 != ' ' && *p1 != '\0')
          ++p1;
            
        // detect end of vertex
        if (*p1 != '/') endOfVertex = true;
        
        // replace separator by '\0'
        if (*p1 != '\0') 
        {
          *p1 = '\0';
          p1++; // point to next token
        }

        // detect end of line and break
        if (*p1 == '\0' || *p1 == '\n')
          p1 = 0;
            
        // read next vertex component
        if (*p0 != '\0')
        {
        	int p0_index = atoi(p0) - 1;
          switch (component)
          {
            case 0: // vertex
            	if (p0_index < 0)
            		p0_index = _bi.n_vertices() + p0_index + 1;
//            	std::cerr << "vhandles: " << vhandles.size() << std::endl;
//            	            	std::cerr << "p0: " << p0_index << std::endl;
            	
            	//vhandles.push_back(VertexHandle(p0_index + textured_offset));
            	added_v = false;
            	current_v = p0_index + textured_offset;

            	break;
              
            case 1: // texture coord
            	if (p0_index < 0) 
            		p0_index = texcoords.size() + p0_index + 1;
//            	std::cerr << "texcoord: " << p0_index << "/" << texcoords.size() << std::endl;
              assert(current_v >= 0);
              
	    	  	if (textured_vertices[current_v] >= 0 && textured_vertices[current_v] != p0_index) {
	          		// if this vertex already has a texture different than the one we need, we have to duplicate it. 
	          		textured_offset++;
	          		_bi.add_vertex(vertices[current_v]);
	          		vertices.push_back(vertices[current_v]); // we need to mirror the new vertex here too
	          		current_v = _bi.n_vertices() - 1;
	          		std::cout << "adding duplicated vertex\n";
	          	} 
	    	  	textured_vertices[current_v] = p0_index;
	    	  	assert(_bi.n_vertices() > current_v);
              		
				  added_v = true;
				  vhandles.push_back(VertexHandle(current_v));
				  
				  assert(!vhandles.empty());
				  assert((unsigned int)(p0_index) < texcoords.size());
				  
					  
				  _bi.set_texcoord(vhandles.back(), texcoords[p0_index]);
				  // aquí debería poder acceder a la malla para poder hacer un set_property().
				  break;
              
            case 2: // normal
            	if (p0_index < 0)
            		p0_index= normals.size() + p0_index + 1;
            	
            if (!added_v) {
            	vhandles.push_back(VertexHandle(current_v));
          		added_v = true;
            }
            	
              assert(!vhandles.empty());
              assert((unsigned int)(p0_index) < normals.size());
              _bi.set_normal(vhandles.back(), normals[p0_index]);
              break;
          }
        }
        
        if (!added_v) {
        	vhandles.push_back(VertexHandle(current_v + textured_offset));
      		added_v = true;
        }

        ++component;

        if (endOfVertex)
        {
          component = 0;
          nV++;
          endOfVertex = false;
        }
      }

      size_t n_faces = _bi.n_faces(); // 
      FaceHandle fh = _bi.add_face(vhandles);

      if ( !matname.empty() && materials_[matname].has_Kd() )
      {
	std::vector<FaceHandle> newfaces;
	
/*	if ( fh.is_valid() )
	  newfaces.push_back(fh);
	else */
	for( size_t i=0; i < _bi.n_faces()-n_faces; ++i )
	  newfaces.push_back(FaceHandle(n_faces+i));	    
	
	
	Material & mat = materials_[matname];

	Vec3uc fc = color_cast<Vec3uc, Vec3f>(mat.Kd());	

	for (std::vector<FaceHandle>::iterator it=newfaces.begin();
	     it != newfaces.end(); ++it)
	  _bi.set_color( *it, fc );
	_opt += Options::FaceColor;
      }
    }

    memset(&s, 0, LINE_LEN);
  }


  return true;
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
