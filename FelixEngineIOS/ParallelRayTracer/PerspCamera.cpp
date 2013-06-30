//
//  PerspCamera.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "PerspCamera.h"
#include "RayNode.h"
#include "Distributions.h"

using namespace std;

PerspCamera::PerspCamera(vec3 loc, vec3 up, vec3 right, vec3 look) {
   _loc = loc;
   _size = vec3(right.length(), up.length(), -DEF_DIST);
   _trans.mtx = mat4::Magic(look - loc, up, loc);
   
   _focus = 1;
   _apature = 0;
   _samples = 1;
}

PerspCamera::~PerspCamera() {
   
}

vec3 PerspCamera::getOrigin() const {
   return _loc;
}

bbox PerspCamera::getBounds() const {
   return _loc;
}

string PerspCamera::getName() const {
   return "PerspCamera";
}

void PerspCamera::write(ostream &os, int indent) const {
   os << " PerspCamera" << endl;
   os << "  loc:" << _loc << endl;
}

vec3 PerspCamera::vectorOn(const ray3 &ray) const {
   return _loc - ray.start;
}

vec3 PerspCamera::vectorFrom(const vec3 &loc) const {
   return _loc - loc;
}

void PerspCamera::transform(const trans &t) {
   _loc =  t.mtx * _loc;
   _trans.mtx = t.mtx * _trans.mtx;
}

ray3 PerspCamera::createRay(double u, double v) const {
   vec3 end((u*2 - 1.0f)/2.0f*_size.x, (v*2 - 1.0f)/2.0f*_size.y, -_size.z);
   vec3 start = _apature ? vec3(Distributions::RandInvCosDisk(), 0) * _apature : 0;
   
   end *= _focus;
   ray3 ray(start, end);
   
   ray.start = _trans.mtx * ray.start;
   ray.dir = _trans.mtx.toMat3() * ray.dir;
   ray.dir.normalize();
   
   return ray;
}

void PerspCamera::createRays(RayNode *ray, const vec2 &loc) const {
   for (int i = 0; i < _samples; ++i)
      ray->addSubRay(createRay(loc.x, loc.y), ray->scene->getPrimShader());
}