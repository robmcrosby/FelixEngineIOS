//
//  Plane.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Plane.h"

using namespace std;

Plane::Plane() {}

Plane::Plane(vec4 eq, Material mat) {
   _material = mat;
   _normal = eq.xyz().normalized();
   _uAxis = _normal.cross(vec3(0, 0, 1)).normalized();
   _vAxis = _normal.cross(_uAxis).normalized();
   _point = _normal * eq.w;
}

Plane::~Plane() {
   
}

vec3 Plane::getOrigin() const {
   return _point;
}

bbox Plane::getBounds() const {
   return bbox();
}

void Plane::write(ostream &os, int indent) const {
   os << " Plane" << endl;
}

void Plane::transform(const trans &t) {
   _trans = t;
}

bool Plane::getCollision(const ray3 &ray, Collision *collision) const {
   double dot, t;
   ray3 invRay;
   
   invRay.start = _trans.invMtx * ray.start;
   invRay.dir = _trans.invRot.toMat3() * ray.dir;
   
   /* check if the ray is parallel */
   dot = -invRay.dir.dot(_normal);
   if (dot < 0.001 && dot > -0.001)
      return false;
   
   /* deterime the distance */
   t = _normal.dot(invRay.start - _point) / dot;
   if (t < 0 || collision->distance < t)
      return false;
   
   collision->item = this;
   collision->distance = t;
   collision->loc = ray.pointAt(t);
   collision->normal = _trans.invMtx.toMat3().transposed() * _normal;
   collision->normal.normalize();
   collision->offset = collision->normal;
   
   vec3 loc = invRay.pointAt(t) - _point;
   collision->uv.x = _uAxis.dot(loc)*0.1;
   collision->uv.y = _vAxis.dot(loc)*0.1;
   
   return true;
}