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

#ifndef UCDAVIS_KEYED_OBJECT_H
#define UCDAVIS_KEYED_OBJECT_H

// Keyed Object class
// - This class wraps an Object and adds a key that contains 
//   size and m_format.
// - KeyedObject is derived from its value_type so that a KeyedObject
//   behaves exactly like a non-keyed object (the keyed
//   object can be transparent).
template <class KeyType, class ValueType>
class KeyedObject : public ValueType
{
public:
  typedef	KeyType		  key_type;
  typedef ValueType	  value_type;
  typedef value_type	base_type;

  KeyedObject( const key_type& key );
  KeyedObject( const key_type& key, const value_type& value );
  virtual ~KeyedObject();

  const key_type& Key() const {
    return m_key;
  }

private:
  // Key used to identify this Object using this Key.
  const key_type m_key;

  // Intentionally undefined default (and private) ctor
  KeyedObject();
};

template <class K, class V>
KeyedObject<K,V>::KeyedObject( const key_type& key )
	: base_type(),
	  m_key(key)
{}

template <class K, class V>
KeyedObject<K,V>::KeyedObject( const key_type& key, const value_type& value )
	: base_type(value), // Call copy ctor for value_type
	  m_key(key)
{}

template <class K, class V>
KeyedObject<K,V>::~KeyedObject()
{}

#endif

