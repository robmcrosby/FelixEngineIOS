//
//  ShaderLight.h
//  RayTracer
//
//  Created by Robert Crosby on 4/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__ShaderLight__
#define __RayTracer__ShaderLight__

#include "Shader.h"

class ShaderLight: public Shader {
public:
   virtual void cast(RayNode *ray) const;
   virtual col4 draw(RayNode *ray) const;
   virtual void draw(RayNode *ray, RayNode *sub) const {}
   
protected:
   ShaderLight(): Shader(SHADER_LIGHT) {}
   virtual ~ShaderLight() {}
   
   static ShaderLight *Instance;
};
 
#endif /* defined(__RayTracer__ShaderLight__) */
