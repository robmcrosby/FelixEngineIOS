//
//  Distributions.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/11/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef ParallelRayTracer_Distributions_h
#define ParallelRayTracer_Distributions_h

#include "Utils/Vector.h"
#include <vector>

enum DIST_TYPE {
   DIST_NONE = 0,
   DIST_1X1 = 1,
   DIST_2X2 = 2,
   DIST_3X3 = 3,
   DIST_4X4 = 4,
};

#define APORT_COUNT 500

typedef std::vector<vec2> dist2d;
typedef std::vector<dist2d> dists2d;

typedef std::vector<vec3> dist3d;
typedef std::vector<dist3d> dists3d;

struct Distributions {
   static double RandomDouble();
   
   static void SetRandSpread2D(dist2d *dist, size_t count);
   static void SetRandEven2D(dist2d *dist, DIST_TYPE type);
   static dists2d *CreateDistsEven2D(DIST_TYPE type, size_t count);
   
   static vec3 RandEvenHemisphere(vec3 normal);
   static void SetRandEvenHemisphere(dist3d *dist, size_t count, vec3 normal = vec3(0, 1, 0));
   
   static vec3 RandCosHemisphere(vec3 normal);
   static void SetRandCosHemisphere(dist3d *dist, size_t count, vec3 normal = vec3(0, 1, 0));
   
   static vec2 RandInvCosDisk();
};

#endif
