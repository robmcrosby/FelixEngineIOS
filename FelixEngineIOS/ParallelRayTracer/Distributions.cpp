//
//  Distributions.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/11/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Distributions.h"
#include <cstdlib>

using namespace std;

double Distributions::RandomDouble() {
   return (double)rand()/(double)RAND_MAX;
}

void Distributions::SetRandSpread2D(dist2d *dist, size_t count) {
   dist2d::iterator itr;
   
   dist->resize(count);
   for (itr = dist->begin(); itr != dist->end(); ++itr) {
      itr->x = RandomDouble();
      itr->y = RandomDouble();
   }
}

void Distributions::SetRandEven2D(dist2d *dist, DIST_TYPE type) {
   if (type == DIST_NONE) {
      dist->resize(1);
      dist->front() = vec2(0.5);
   }
   else {
      int i, j, dim = (int)type;
      double inter = 1.0/(double)dim;
      dist2d::iterator itr;
      
      dist->resize(dim*dim);
      for (j = 0, itr = dist->begin(); j < dim; ++j) {
         for (i = 0; i < dim; ++i, ++itr) {
            itr->x = RandomDouble()*inter + i*inter;
            itr->y = RandomDouble()*inter + j*inter;
         }
      }
   }
};

dists2d *Distributions::CreateDistsEven2D(DIST_TYPE type, size_t count) {
   dists2d *dists;
   dists2d::iterator itr;
   
   dists = type == DIST_NONE ? new dists2d(1) : new dists2d(count);
   
   for (itr = dists->begin(); itr != dists->end(); ++itr)
      SetRandEven2D(&*itr, type);
   
   return dists;
}

vec3 Distributions::RandEvenHemisphere(vec3 normal) {
   int abortCount = 0;
   vec3 v;
   
   while (true) {
      v.x = RandomDouble() - 0.5;
      v.y = RandomDouble() - 0.5;
      v.z = RandomDouble() - 0.5;
      v.normalize();
      
      if (v.dot(normal) > 0)
         return v;
      
      if (++abortCount > APORT_COUNT)
         return normal;
   }
}

void Distributions::SetRandEvenHemisphere(dist3d *dist, size_t count, vec3 normal) {
   dist3d::iterator itr;
   
   dist->resize(count);
   for (itr = dist->begin(); itr != dist->end(); ++itr)
      *itr = RandEvenHemisphere(normal);
}

vec3 Distributions::RandCosHemisphere(vec3 normal) {
   double Xi1 = RandomDouble();
   double Xi2 = RandomDouble();
   
   double theta = acos(sqrt(1.0-Xi1));
   float  phi = TwoPi * Xi2;
   
   float xs = sinf(theta) * cosf(phi);
   float ys = cosf(theta);
   float zs = sinf(theta) * sinf(phi);
   
   vec3 y = normal;
   vec3 h = y;
   if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z))
      h.x = 1.0;
   else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z))
      h.y = 1.0;
   else
      h.z= 1.0;
   
   
   vec3 x = h.cross(y).normalized();
   vec3 z = x.cross(y).normalized();
   
   vec3 dir = x * xs + y * ys + z * zs;
   return dir.normalized();
}

void Distributions::SetRandCosHemisphere(dist3d *dist, size_t count, vec3 normal) {
   dist3d::iterator itr;
   
   dist->resize(count);
   for (itr = dist->begin(); itr != dist->end(); ++itr)
      *itr = RandCosHemisphere(normal);
}

vec2 Distributions::RandInvCosDisk() {
   double Xi1 = RandomDouble();
   double Xi2 = RandomDouble();
   
   double theta = acos(sqrt(1.0-Xi1));
   float  phi = TwoPi * Xi2;
   
   vec2 loc;
   loc.x = sinf(theta) * cosf(phi);
   loc.y = sinf(theta) * sinf(phi);
   
   return loc;
}