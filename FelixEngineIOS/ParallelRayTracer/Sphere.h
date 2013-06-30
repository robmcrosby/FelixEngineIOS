//
//  Sphere.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__Sphere__
#define __RayTracer__Sphere__

#include "Traceable.h"

class Sphere: public Traceable {
public:
   Sphere(): _center(0), _radius(0) {}
   Sphere(vec3 c, double r, Material m);
   virtual ~Sphere();
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual void transform(const trans &t);
   
   virtual bool getCollision(const ray3 &ray, Collision *collision) const;
   virtual const Material* getMaterial() const {return &_material;}
protected:
   inline void updateBounds();
   
   bbox _bounds;
   vec3 _center;
   double _radius;
   Material _material;
   trans _trans;
};

#endif /* defined(__RayTracer__Sphere__) */
