//
//  Camera.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__Camera__
#define __RayTracer__Camera__

#include "Item.h"

class RayNode;

struct Camera: public Item {
   virtual ~Camera() {}
   
   virtual void createRays(RayNode *ray, const vec2 &loc) const = 0;
   virtual ray3 createRay(double u, double v) const = 0;
   inline  ray3 createRay(const vec2 &l) const {return createRay(l.x, l.y);}
};

#endif /* defined(__RayTracer__Camera__) */
