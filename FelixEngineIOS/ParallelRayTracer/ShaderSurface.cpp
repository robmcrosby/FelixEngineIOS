//
//  ShaderSurface.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ShaderSurface.h"
#include "Traceable.h"
#include "Scene.h"
#include "RayNode.h"

using namespace std;

ShaderColor *ShaderColor::Instance = new ShaderColor();
ShaderPhong *ShaderPhong::Instance = new ShaderPhong();
ShaderBlinn *ShaderBlinn::Instance = new ShaderBlinn();
ShaderGaussian *ShaderGaussian::Instance = new ShaderGaussian();

// Shader Surface
//---------------------------------------------------

void ShaderSurface::cast(RayNode *ray) const {
   const Material *mat;
   const RayOptions *opts = ray->scene->getOptions();
   int curLevel = ray->level/2;
   
   ray->castSelf();
   *ray->destBuff = col4(0, 0, 0, 1);
   
   if ((mat = ray->getMaterial())) {
      
      if (curLevel < opts->maxLevels) {
         if (!mat->fra)
            addLightRays(ray);
         if (mat->ref || mat->fra)
            ray->addSubRay(*ray, SHADER_REFLECT, ray->collision);
         if (mat->fra)
            ray->addSubRay(*ray, SHADER_REFRACT, ray->collision);
         if (curLevel < opts->giLevels && !(*ray->parrent->shader == SHADER_GI) && !(mat->ref || mat->fra))
            ray->addSubRay(*ray, SHADER_GI, ray->collision);
         else if (!mat->fra)
            *ray->destBuff = col4(mat->getAmbient() , 1);
      }
      else
         addLightRays(ray);
   }
}

col4 ShaderSurface::draw(RayNode *ray) const {
   addBounce(ray);
   return *ray->destBuff;
}

void ShaderSurface::draw(RayNode *ray, RayNode *sub) const {
   const Material *mat = ray->getMaterial();
   
   if (*sub->shader == SHADER_LIGHT)
      addLight(ray, sub);
   else if (*sub->shader == SHADER_GI)
      ray->safeAddColor(col4(mat->getColor(ray->collision.uv), 0) *  *sub->destBuff);
   else if (*sub->shader == SHADER_REFLECT || *sub->shader == SHADER_REFRACT)
      ray->store(sub);
}

void ShaderSurface::addLight(RayNode *ray, RayNode *sub) const {
   const Light *light = sub->getLight();
   
   if (light) {
      const Material *mat = ray->getMaterial();
      col3 color;
      
      color = light->getColor() * mat->getDiffuse(ray->collision.uv) * diffuse(*ray, *sub);
      color += light->getColor() * mat->getSpecular(ray->collision.uv) * specular(*ray, *sub);
      ray->safeAddColor(col4(color, 0));
   }
}

void ShaderSurface::addBounce(RayNode *ray) const {
   const Material *mat = ray->getMaterial();
   
   RayNode *reflection = ray->findStored(SHADER_REFLECT);
   RayNode *refraction = ray->findStored(SHADER_REFRACT);
   
   if ((!refraction || !mat->fra) && reflection) {
      *ray->destBuff *= 1.0 - mat->ref;
      *ray->destBuff += reflection->shader->draw(reflection) * mat->ref;
   }
   else if (reflection && refraction && mat->fra) {
      double R = shlick(ray, refraction);
      col4 color = *reflection->destBuff * R;
      color += *refraction->destBuff * (1.0 - R);
      color *= beers(ray, refraction);
      *ray->destBuff += color;
   }
}

double ShaderSurface::diffuse(const RayNode &vRay, const RayNode &lRay) const {
   double diffuse = vRay.getNormal().dot(lRay.dir);
   return diffuse < 0 ? 0 : diffuse;
}

void ShaderSurface::addLightRays(RayNode *ray) const {
   const vector<Light*> *lights = ray->scene->getLights();
   vector<Light*>::const_iterator light;
   vec3 offset = ray->getOffsetOut();
   
   for (light = lights->begin(); light != lights->end(); ++light) {
      ray3 lightRay = (*light)->getRay(offset);
      ray->addSubRay(lightRay, SHADER_LIGHT, Collision(lightRay, *light));
   }
}

double ShaderSurface::shlick(RayNode *ray, RayNode *sub) const {
   const Material *mat = ray->getMaterial();
   double R, a, b, c;
   
   if (sub->dir == vec3(0))
      return 1.0;
   
   c = ray->dir.dot(ray->getNormal());
   c = c < 0 ? -c : sub->dir.dot(ray->getNormal());
   
   a = mat->ior - 1.0;
   b = mat->ior + 1.0;
   R = (a*a)/(b*b);
   return R + (1.0 - R)*pow((1.0 - c), 5);
}

col4 ShaderSurface::beers(RayNode *ray, RayNode *sub) const {
   return col4(1, 1, 1, 1);
}


// Shader Color
//---------------------------------------------------

void ShaderColor::cast(RayNode *ray) const {
   ray->castSelf();
   *ray->destBuff = col4(0, 0, 0, 1);
}

col4 ShaderColor::draw(RayNode *ray) const {
   const Material *mat = ray->getMaterial();
   
   if (mat)
      return col4(mat->color, 1.0);
   return *ray->destBuff;
}



// Shader Phong
//---------------------------------------------------

double ShaderPhong::specular(const RayNode &vRay, const RayNode &lRay) const {
   const Material *mat = vRay.getMaterial();
   double specular = vRay.getReflection().dot(lRay.dir);
   double smooth = mat->rou < 0.001 ? 1000 : 1.0/mat->rou;
   
   specular = specular < 0 ? 0 : specular;
   return pow(specular, smooth);
}


// Shader Blinn
//---------------------------------------------------

double ShaderBlinn::specular(const RayNode &vRay, const RayNode &lRay) const {
   const Material *mat = vRay.getMaterial();
   double specular = vRay.getHalfVec(lRay.dir).dot(vRay.getNormal());
   double smooth = mat->rou < 0.001 ? 1000 : 1.0/mat->rou;
   
   specular = specular < 0 ? 0 : specular;
   return pow(specular, smooth);
}


// Shader Gaussian
//---------------------------------------------------

double ShaderGaussian::specular(const RayNode &vRay, const RayNode &lRay) const {
   const Material *mat = vRay.getMaterial();
   double angle = vRay.getHalfVec(lRay.dir).dot(vRay.getNormal());
   
   if (angle <= 0.0 || mat->rou <= 0.0)
      return 0.0;
   
   angle = acos(angle);
   angle /= mat->rou;
   angle *= angle;
   
   return exp(-angle);
}



