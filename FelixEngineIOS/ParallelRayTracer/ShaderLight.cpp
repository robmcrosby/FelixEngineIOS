//
//  ShaderLight.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ShaderLight.h"
#include "RayNode.h"

ShaderLight *ShaderLight::Instance = new ShaderLight();


void ShaderLight::cast(RayNode *ray) const {
   ray->castSelf();
}

col4 ShaderLight::draw(RayNode *ray) const {
   const Light *light = ray->getLight();
   return light == NULL ? col4(0, 0, 0, 1) : col4(light->getColor(), 1);
}