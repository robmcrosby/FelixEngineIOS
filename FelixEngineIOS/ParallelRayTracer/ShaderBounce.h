//
//  ShaderBounce.h
//  RayTracer
//
//  Created by Robert Crosby on 4/22/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__ShaderBounce__
#define __RayTracer__ShaderBounce__

#include "Shader.h"

class ShaderBounce: public Shader {
public:
   virtual void draw(RayNode *ray, RayNode *sub) const;
   virtual col4 draw(RayNode *ray) const;
protected:
   inline vec3 refractDir(RayNode *ray) const;
   
   ShaderBounce(SHADER_TYPE type): Shader(type) {}
   virtual ~ShaderBounce() {}
};

class ShaderReflect: public ShaderBounce {
public:
   virtual void cast(RayNode *ray) const;
   
protected:
   ShaderReflect(): ShaderBounce(SHADER_REFLECT) {}
   virtual ~ShaderReflect() {}
   
   static ShaderReflect *Instance;
};


class ShaderRefract: public ShaderBounce {
public:
   virtual void cast(RayNode *ray) const;
   
protected:
   ShaderRefract(): ShaderBounce(SHADER_REFRACT) {}
   virtual ~ShaderRefract() {}
   
   static ShaderRefract *Instance;
};


#endif /* defined(__RayTracer__ShaderBounce__) */
