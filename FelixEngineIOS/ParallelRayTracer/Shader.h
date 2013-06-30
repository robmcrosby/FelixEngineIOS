//
//  Shader.h
//  RayTracer
//
//  Created by Robert Crosby on 4/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef RayTracer_Shader_h
#define RayTracer_Shader_h

#include "Utils/Color.h"

class RayNode;

enum SHADER_TYPE {
   SHADER_ROOT,
   SHADER_LIGHT,
   SHADER_REFLECT,
   SHADER_REFRACT,
   SHADER_COLOR,
   SHADER_PHONG,
   SHADER_BLINN,
   SHADER_GAUSSIAN,
   SHADER_GI,
   SHADER_AO,
};

class Shader {
public:
   virtual void cast(RayNode *ray) const = 0;
   virtual void draw(RayNode *ray, RayNode *sub) const = 0;
   virtual col4 draw(RayNode *ray) const = 0;
   
   inline bool operator==(SHADER_TYPE type) const {return _type == type;}
   inline SHADER_TYPE getType() const {return _type;}
   
   static const Shader *GetShader(SHADER_TYPE type);
protected:
   Shader(SHADER_TYPE type);
   virtual ~Shader() {}
   
private:
   SHADER_TYPE _type;
   Shader *_next;
   
   static Shader *Shaders;
};

class ShaderRoot: public Shader {
public:
   virtual void cast(RayNode *ray) const;
   virtual void draw(RayNode *ray, RayNode *sub) const;
   virtual col4 draw(RayNode *ray) const;

protected:
   ShaderRoot(): Shader(SHADER_ROOT) {}
   virtual ~ShaderRoot() {}
   
   static ShaderRoot *Instance;
};

#endif
