//
//  ShaderGI.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ShaderGI.h"
#include "RayNode.h"
#include "Distributions.h"

ShaderGI *ShaderGI::Instance = new ShaderGI();


void ShaderGI::cast(RayNode *ray) const {
   const RayOptions *opts = ray->scene->getOptions();
   *ray->destBuff = col4(0, 0, 0, 0);
   vec3 normal = ray->getNormal();
   ray3 ref;
   
   normal = ray->isInside() ? -normal : normal;
   ref.start = ray->isInside() ? ray->getOffsetIn() : ray->getOffsetOut();
   
   for (int i = 0; i < opts->giSamples; ++i) {
      ref.dir = Distributions::RandCosHemisphere(normal);
      ray->addSubRay(ref, ray->scene->getPrimShader());
   }
}

void ShaderGI::draw(RayNode *ray, RayNode *sub) const {
   ray->safeAddColor(sub->shader->draw(sub)/(double)ray->subcount);
}

col4 ShaderGI::draw(RayNode *ray) const {
   return *ray->destBuff;
}