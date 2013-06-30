//
//  ShaderAO.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 6/8/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ShaderAO.h"
#include "RayNode.h"
#include "Distributions.h"

ShaderAO *ShaderAO::Instance = new ShaderAO();


void ShaderAO::cast(RayNode *ray) const {
   const RayOptions *opts = ray->scene->getOptions();
   
   if (ray->level == 1) { // main ray
      if (ray->castSelf()) {
         vec3 normal = ray->getNormal();
         ray3 ref;
         
         *ray->destBuff = col4(1, 1, 1, 1);
         
         ref.start = ray->getOffsetOut();
         for (int i = 0; i < opts->aoSamples; ++i) {
            ref.dir = Distributions::RandCosHemisphere(normal);
            ray->addSubRay(ref, SHADER_AO);
         }
      }
      else
         *ray->destBuff = col4(0, 0, 0, 1);
   }
   else { // feeler ray
      if (ray->castSelf()) {
         double c = -1.0/(double)opts->aoSamples;
         *ray->destBuff = col4(c, c, c, 0);
      }
      else
         *ray->destBuff = col4(0, 0, 0, 0);
   }
}

void ShaderAO::draw(RayNode *ray, RayNode *sub) const {
   ray->safeAddColor(sub->shader->draw(sub));
}

col4 ShaderAO::draw(RayNode *ray) const {
   return *ray->destBuff;
}