//
//  Item.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef RayTracer_Item_h
#define RayTracer_Item_h

#include <iostream>
#include <string>
#include <limits>
#include "Utils/Vector.h"
#include "Utils/Matrix.h"
#include "Utils/Quaternion.h"
#include "Utils/Color.h"
#include "Utils/Ray.h"

#define INDENT string(indent*3, ' ')

enum Axis {
   AxisX,
   AxisY,
   AxisZ,
};

struct trans {
   mat4 mtx;
   mat4 invMtx;
   mat4 rot;
   mat4 invRot;
};

struct bbox {
   bbox(): high(std::numeric_limits<double>::min()), low(std::numeric_limits<double>::max()) {}
   bbox(const vec3 &v): high(v), low(v) {}
   
   inline void addPt(const vec3 &v) {high = high.max(v); low = low.min(v);}
   inline void addBox(const bbox &b) {high = high.max(b.high); low = low.min(b.low);}

   inline bool intersect(const ray3 &r, double *dist) const {
      int i;
      double tnear = 0, tfar = *dist;
      
      for (i = 0; i < 3; ++i) {
         double t0, t1, l = *(&low.x+i), h = *(&high.x+i);
         double d = *(&r.dir.x+i), s = *(&r.start.x+i);
         
         if (d > -0.0001 && d < 0.0001) {
            if (s < l || s > h)
               return false;
         }
         else {
            t0 = (l-s)/d;
            t1 = (h-s)/d;
            if (t0 > t1) {
               double tmp = t0;
               t0 = t1;
               t1 = tmp;
            }
            tnear = t0 > tnear ? t0 : tnear;
            tfar = t1 < tfar ? t1 : tfar;
            if (tnear > tfar)
               return false;
         }
      }
      *dist = tnear;
      return true;
   }
   inline double volume() const {
      if (high == std::numeric_limits<double>::min() && low == std::numeric_limits<double>::max())
         return -1;
      else {
         vec3 v = high - low;
         return v.x * v.y * v.z;
      }
   }
   inline vec3 center() const {return (high-low)/2.0 + low;}
   
   friend std::ostream &operator<<(std::ostream &os, const bbox &b) {
      return os << "{" << b.low << ", " << b.high << "}";
   }
   
   vec3 high;
   vec3 low;
};

struct Item {
   virtual ~Item() {}
   virtual vec3 getOrigin() const = 0;
   virtual bbox getBounds() const = 0;
   virtual void write(std::ostream &os, int indent = 0) const = 0;
   virtual void transform(const trans &t) = 0;
   
   friend std::ostream &operator<<(std::ostream &os, const Item &item) {
      item.write(os);
      return os;
   }
};

#endif
