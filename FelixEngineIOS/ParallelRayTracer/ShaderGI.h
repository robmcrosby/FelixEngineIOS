//
//  ShaderGI.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__ShaderGI__
#define __ParallelRayTracer__ShaderGI__

#include "Shader.h"

class ShaderGI: public Shader {
public:
   virtual void cast(RayNode *ray) const;
   virtual col4 draw(RayNode *ray) const;
   virtual void draw(RayNode *ray, RayNode *sub) const;
   
protected:
   ShaderGI(): Shader(SHADER_GI) {}
   virtual ~ShaderGI() {}
   
   static ShaderGI *Instance;
};

#endif /* defined(__ParallelRayTracer__ShaderGI__) */
