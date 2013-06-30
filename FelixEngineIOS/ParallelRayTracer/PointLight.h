//
//  PointLight.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__PointLight__
#define __RayTracer__PointLight__

#include "Light.h"

class PointLight: public Light {
public:
   PointLight();
   PointLight(vec3 loc, col3 col);
   virtual ~PointLight();
   
   virtual col3 getColor() const;
   virtual ray3 getRay(const vec3 &pt) const;
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual std::string getName() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual vec3 vectorOn(const ray3 &ray) const;
   virtual vec3 vectorFrom(const vec3 &loc) const;
   virtual void transform(const trans &t);
   
protected:
   vec3 _loc;
   col3 _color;
   trans _trans;
};

#endif /* defined(__RayTracer__PointLight__) */
