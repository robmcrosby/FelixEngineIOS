//
//  ShaderAO.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 6/8/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__ShaderAO__
#define __ParallelRayTracer__ShaderAO__

#include "Shader.h"

class ShaderAO: public Shader {
public:
   virtual void cast(RayNode *ray) const;
   virtual col4 draw(RayNode *ray) const;
   virtual void draw(RayNode *ray, RayNode *sub) const;
   
protected:
   ShaderAO(): Shader(SHADER_AO) {}
   virtual ~ShaderAO() {}
   
   static ShaderAO *Instance;
};

#endif /* defined(__ParallelRayTracer__ShaderAO__) */
