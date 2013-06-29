//
//  Attributes.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_Attributes_h
#define FelixEngineIOS_Attributes_h

#include <string>
#include <map>

#define VAR_PROJ_MTX "ProjMtx"
#define VAR_VIEW_MTX "ViewMtx"
#define VAR_LOCAL_MTX "LocalMtx"
#define VAR_MODEL_MTX "ModelMtx"
#define VAR_NORM_MTX "NormalMtx"
#define VAR_TEX_MTX "TexMtx"

enum VAL_TYPE {
   VAL_NONE,
   VAL_INT,
   VAL_FLOAT,
   VAL_VEC2,
   VAL_VEC3,
   VAL_VEC4,
   VAL_MAT2X2,
   VAL_MAT3X3,
   VAL_MAT4X4,
};


/**
 * Defines a uniform value
 */
struct Uniform {
   Uniform(): type(VAL_NONE) {}
   Uniform(VAL_TYPE t, void *v, int c = 1): type(t), value(v), count(c) {}
   
   VAL_TYPE type;
   void *value;
   int count;
};

/**
 * Collection of uniforms
 */
class Uniforms {
public:
   Uniforms() {}
   
   typedef std::map<std::string, Uniform>::const_iterator const_iterator;
   inline const_iterator begin() const {return _uniforms.begin();}
   inline const_iterator end() const {return _uniforms.end();}
   
   inline void addUniform(const std::string &name, const Uniform &uniform) {
      _uniforms[name] = uniform;
   }
   inline void addUniform(const std::string &name, VAL_TYPE t, void *v, int c = 1) {
      addUniform(name, Uniform(t, v, c));
   }
   
   inline void clear() {_uniforms.clear();}
   inline void remove(const std::string &name) {_uniforms.erase(name);}
   
private:
   std::map<std::string, Uniform> _uniforms;
};


/**
 * Defines an attribute for a shader
 */
struct Attribute {
   Attribute(): size(0), offset(0) {}
   Attribute(int s, int o): size(s), offset(o) {}
   
   int size, offset;
};


/**
 * Collection of vertex attributes
 */
class Attributes {
public:
   Attributes(): _stride(0) {}
   
   typedef std::map<std::string, Attribute>::const_iterator const_iterator;
   inline const_iterator begin() const {return _attributes.begin();}
   inline const_iterator end() const {return _attributes.end();}
   
   inline int getStride() const {return _stride;}
   inline void addAttribute(const std::string &name, const Attribute &att) {
      _attributes[name] = att;
      _stride += att.size;
   }
   inline void addAttribute(const std::string &name, int size, int offset) {
      addAttribute(name, Attribute(size, offset));
   }
   
private:
   int _stride;
   std::map<std::string, Attribute> _attributes;
};

#endif
