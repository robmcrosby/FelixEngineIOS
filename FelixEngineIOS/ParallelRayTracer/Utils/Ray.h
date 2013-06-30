//
//  Ray.h
//  FirstRays
//
//  Created by Robert Crosby on 3/31/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FirstRays_Ray_h
#define FirstRays_Ray_h

#include "Vector.h"

template <typename T>
struct Ray3 {
   Ray3(): start(0), dir(0), length(0) {}
   Ray3(Vector3<T> s, Vector3<T> e): start(s), dir(e-s) {
      length = dir.length();
      dir.normalize();
   }
   
   inline Vector3<T> pointAt(T t) const {return dir*t + start;}
   friend std::ostream &operator<<(std::ostream &os, const Ray3 &r) {
      return os << "[" << r.start << "," << r.dir << "]";
   }
   
   Vector3<T> start;
   Vector3<T> dir;
   T length;
};

typedef Ray3<double> ray3;

#endif
