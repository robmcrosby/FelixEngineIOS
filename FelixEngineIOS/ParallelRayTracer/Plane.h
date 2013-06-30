//
//  Plane.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__Plane__
#define __RayTracer__Plane__

#include "Traceable.h"

class Plane: public Traceable {
public:
   Plane();
   Plane(vec4 eq, Material mat);
   virtual ~Plane();
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual void transform(const trans &t);
   
   virtual bool getCollision(const ray3 &ray, Collision *collision) const;
   virtual const Material* getMaterial() const {return &_material;}
private:
   vec3 _point;
   vec3 _normal;
   vec3 _uAxis;
   vec3 _vAxis;
   Material _material;
   trans _trans;
};

#endif /* defined(__RayTracer__Plane__) */
