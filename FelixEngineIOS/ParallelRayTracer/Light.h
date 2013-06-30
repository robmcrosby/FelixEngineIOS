//
//  Light.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef RayTracer_Light_h
#define RayTracer_Light_h

#include "Item.h"
#include "Utils/Color.h"

struct Light: public Item {
   virtual ~Light() {}
   virtual col3 getColor() const = 0;
   virtual ray3 getRay(const vec3 &pt) const = 0;
};

#endif
