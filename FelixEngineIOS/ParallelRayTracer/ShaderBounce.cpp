//
//  ShaderBounce.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/22/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ShaderBounce.h"
#include "RayNode.h"
#include "Traceable.h"

using namespace std;
using namespace tbb;


ShaderReflect *ShaderReflect::Instance = new ShaderReflect();
ShaderRefract *ShaderRefract::Instance = new ShaderRefract();


// Shader Bounce
//---------------------------------------------------

void ShaderBounce::draw(RayNode *ray, RayNode *sub) const {
   ray->safeAddColor(sub->shader->draw(sub) / (double)ray->subcount);
}

col4 ShaderBounce::draw(RayNode *ray) const {
   return *ray->destBuff;
}

vec3 ShaderBounce::refractDir(RayNode *ray) const {
   const Material *mat = ray->getMaterial();
   vec3 normal = ray->getNormal();
   double n1, n2, dot, sqr;
   
   if (ray->isInside()) { // going out
      n1 = mat->ior;
      n2 = 1.0;
      normal = -normal;
   }
   else { // going in
      n1 = 1.0;
      n2 = mat->ior;
   }
   
   dot = normal.dot(ray->dir);
   sqr = 1 - (n1*n1*(1 - dot * dot))/(n2*n2);
   
   if (sqr > 0) {
      sqr = sqrt(sqr);
      return (ray->dir - normal*dot) * (n1/n2) - normal * sqr;
   }
   return 0;
}


// Shader Reflection
//---------------------------------------------------

void ShaderReflect::cast(RayNode *ray) const {
   ray3 ref;
   
   *ray->destBuff = col4(0, 0, 0, 0);
   
   ref.dir = ray->getReflection();
   ref.start = ray->isInside() ? ray->getOffsetIn() : ray->getOffsetOut();
   ray->addSubRay(ref, ray->scene->getPrimShader());
   ray->dir = refractDir(ray);
}


// Shader Refraction
//---------------------------------------------------

void ShaderRefract::cast(RayNode *ray) const {
   ray3 ref;
   
   *ray->destBuff = col4(0, 0, 0, 0);
   
   ref.dir = refractDir(ray);
   if (!(ref.dir == vec3(0))) {
      ref.start = ray->isInside() ? ray->getOffsetOut() : ray->getOffsetIn();
      ray->addSubRay(ref, ray->scene->getPrimShader());
   }
   ray->dir = ref.dir;
}

