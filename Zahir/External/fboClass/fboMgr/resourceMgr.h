/*
  Copyright (c) 2005
	  Aaron Lefohn	(lefohn@cs.ucdavis.edu)
	  Robert Strzodka (strzodka@stanford.edu)
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

#ifndef UCDAVIS_RESOURCE_MANAGER_H
#define UCDAVIS_RESOURCE_MANAGER_H

#include <map>
#include <iostream>
#include "keyedObject.h"

/*! 
Singleton Resource Manager class. This class keeps track of a collection of
resources distinguished by a key. 

Type Concept Information
  1) "KeyType" is the key used to distinguish the resources in the pool.
  2) "ValueType" is the user-visible resource type returned by "Acquire"
      and accepted by "Release."
  3) "KeyedValueType" is the actual resource type used internally by
      the ResourceMgr. KeyedValueType MUST be derived from ValueType,
	    contain a "Key()" method, and be able to construct a ValueType
	    using a Key (i.e., Must have constructor KeyedValueType(key)).

	    Most users can skip this template parameter and use the default.
	    Advanced users can define their own KeyedObjectFoo class that does
	    special runtime checks, custom construction from a Key, etc.
  4) "CreationPolicy" determines how (and if) the ResourceMgr creates
      new elements on-demand. The default behavior (NewKeyCreationPolicy)
	    creates a new element by calling the keyed_value_type(key) constructor.
	    Use NullCreationPolicy for a ResourceMgr that cannot create implicitly
	    create new elements. Other policies could be created for element types
	    needing custom construction.

  TODO: Make Singleton threadsafe

  Usage example (Managing FramebufferObject instances):
    typedef FboSizeFormatKey Key;
    typddef ResourceMgr< Key,
                         FramebufferObject, 
                         KeyedFramebufferObject > FramebufferMgr;

    FramebufferMgr* fboMgr = FramebufferMgr::Instance();

    FramebufferObject* fbo1 = fboMgr->Acquire( Key(512, 512, GL_RGBA8) );
    fbo1->Bind();
    fbo1->AttachTexture(...);
    ... Render ...

    FramebufferObject* fbo2 = fboMgr->Acquire( Key(128, 128, GL_LUMINANCE8) );
    fbo2->Bind();
    fbo2->AttachTexture(...);
    ... Render ...

    FramebufferObject::Disable();
    fboMgr->Release(fbo1);
    fboMgr->Release(fbo2);
*/

// Element Creation Policies
template <class KeyedValueType>
struct NullCreationPolicy
{
  // Use this creation policy to make a resource
  // manager that cannot create new elements.
  typedef typename KeyedValueType::key_type key_type;

  KeyedValueType* Create( const key_type& key ) {
    return NULL;
  }
};

template <class KeyedValueType>
struct NewKeyCreationPolicy
{
  typedef typename KeyedValueType::key_type key_type;

  // Use this creation policy to make a resource
  // manager that CAN create new elements using
  // a key-based constructor.
  KeyedValueType* Create( const key_type& key ) {
    return new KeyedValueType(key);
  }
};

// ResourceMgr Class
template <class KeyType,
          class ValueType,
          class KeyedValueType = KeyedObject<KeyType, ValueType>,
          class CreationPolicy = NewKeyCreationPolicy<KeyedValueType> >
class ResourceMgr : private CreationPolicy
{
public:
  // Public typedefs
  typedef KeyType			key_type;
  typedef ValueType		value_type;
  typedef KeyedValueType	keyed_value_type;
  typedef CreationPolicy  creation_policy;

  // Accessor to singleton instance
  static ResourceMgr* Instance();

  // Acquire a resource
  // - Returns a resource from the pool if a match to "key" is found.
  // - If no match is found, CreationPolicy::Create(key) is called and
  //   the results returned. Whether or not a new element is created
  //   depends on the implementation of CreationPolicy.
  // - NOTE : The return value should generally be assigned to a 
  //          'value_type' pointer, unless you need explicit access 
  //          to the members of 'keyed_value_type'.
  keyed_value_type* Acquire( const key_type& key );

  // Release the resource and return it to the pool
  // - NOTE : "elt" MUST have been obtained via Acquire(...)
  //          or this call will issue a warning and fail to add
  //          "elt" to the Manager.
  void Release( value_type* elt );

  // Delete all resources that are currently "checked-in" to manager
  // - Does NOT delete resources that are currently checked out.
  void Clear();

private:
  typedef ResourceMgr<key_type, value_type,
		      keyed_value_type, creation_policy>  this_type;
  typedef std::multimap<key_type, keyed_value_type*>      map_type;

  // Resource multimap
  map_type m_map;

  // Singleton static instance
  static this_type* m_instance;

  // Private ctors to prevent unauthorized construction
  ResourceMgr() {}
  ResourceMgr( const ResourceMgr& rhs ) {}
  ResourceMgr operator=(const ResourceMgr& rhs ) {return *this;}

  void _Insert( const keyed_value_type& keyedVal );
};

//------------------------------------------------------
// static class members
//------------------------------------------------------
template <class K, class V, class T, class C>
ResourceMgr<K,V,T,C>* ResourceMgr<K,V,T,C>::m_instance = NULL;

//------------------------------------------------------
// Accessor to singleton instance
//------------------------------------------------------
template <class K, class V, class T, class C>
ResourceMgr<K,V,T,C>* 
ResourceMgr<K,V,T,C>::Instance()
{
  if (!m_instance) {
    m_instance = new ResourceMgr();
  }
  return m_instance;
}

//------------------------------------------------------
// Acquire a resource
// - Returns either a resource from the pool or creates a new one
//------------------------------------------------------
template <class K, class V, class T, class C>
T*
ResourceMgr<K,V,T,C>::Acquire( const key_type& key )
{
  // Lookup key in map
  typename map_type::iterator it = m_map.find(key);

  // If found remove resource from map
  keyed_value_type* foundElt = NULL;
  if( it != m_map.end() ) {
    foundElt = it->second;
    m_map.erase(it);
  } 
  else { 	// else create a new one
    foundElt = Create(key);
  }

  return foundElt;
}

//------------------------------------------------------
// Release a resource and return it to the pool
//------------------------------------------------------
template <class K, class V, class T, class C>
void
ResourceMgr<K,V,T,C>::Release( value_type* elt )
{
  keyed_value_type* keyedElt = dynamic_cast<keyed_value_type*>(elt);
  if (keyedElt) {
    m_map.insert( map_type::value_type(keyedElt->Key(), keyedElt) );
  }
  else {
    std::cerr << "ResourceMgr::Release ERROR:\n"
              << "\tResource was not Acquired from a ResourceManager and\n"
              << "\tdoes not contain a key.\n\n";
  }
}

//------------------------------------------------------
// Clear the current pool
//------------------------------------------------------
template <class K, class V, class T, class C>
void
ResourceMgr<K,V,T,C>::Clear()
{
  typename map_type::iterator it;
  for (it = m_map.begin(); it != m_map.end(); ++it) {
    delete it->second;
  }
  m_map.clear();
}

#endif
