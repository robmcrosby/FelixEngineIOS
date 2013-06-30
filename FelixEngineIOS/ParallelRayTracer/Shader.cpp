//
//  Shaders.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Shader.h"
#include "Scene.h"
#include "RayNode.h"

using namespace std;
using namespace tbb;

Shader* Shader::Shaders;
ShaderRoot *ShaderRoot::Instance = new ShaderRoot();

const Shader *Shader::GetShader(SHADER_TYPE type) {
   Shader *shader = Shaders;
   
   while (shader && shader->_type != type)
      shader = shader->_next;
   return shader;
}

Shader::Shader(SHADER_TYPE type): _type(type), _next(Shaders) {
   Shaders = this;
}

void ShaderRoot::cast(RayNode *ray) const {
   size_t pixel = ray->job->getPixel();
   ray->subcount = 0;
   
   if (pixel < ray->job->getTotal()) {
      const Camera *camera = ray->scene->getCamera();
      const dist2d *dist = ray->job->getDist(pixel);
      vec2 size = ray->job->getBuffSize();
      vec2 loc = ray->job->getLoc(pixel);
      
      // set the destination
      ray->destBuff = ray->job->getDest(pixel);
      *ray->destBuff = col4(0, 0, 0, 0);
      
      for (int i = 0; i < dist->size(); ++i)
         camera->createRays(ray, (loc + dist->at(i))/size);
      
      /*
      for (int i = 0; i < dist->size(); ++i) {
         ray3 pRay = camera->createRay((loc + dist->at(i))/size);
         ray->addSubRay(pRay, ray->scene->getPrimShader());
      }*/
      
      ray->state = RAY_UNCASTED;
   }
}

void ShaderRoot::draw(RayNode *ray, RayNode *sub) const {
   ray->safeAddColor(sub->shader->draw(sub) / (double)ray->subcount);
}

col4 ShaderRoot::draw(RayNode *ray) const {
   return col4();
}