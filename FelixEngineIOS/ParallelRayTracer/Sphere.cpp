//
//  Sphere.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Sphere.h"
#include <vector>

using namespace std;


Sphere::Sphere(vec3 c, double r, Material m):
   _center(c),
   _radius(r),
   _material(m) {
   updateBounds();
}

Sphere::~Sphere() {
   
}

vec3 Sphere::getOrigin() const {
   return _center;
}

bbox Sphere::getBounds() const {
   return _bounds;
}

void Sphere::write(ostream &os, int indent) const {
   os << "Sphere" << endl;
   os << "  loc:" << _center << endl;
   os << "  radius: " << _radius << endl;
   os << "  ";
   _material.write(os);
   os << endl;
}

void Sphere::transform(const trans &t) {
   _trans = t;
   updateBounds();
}

bool Sphere::getCollision(const ray3 &ray, Collision *collision) const {
   ray3 invRay;
   
   invRay.start = _trans.invMtx * ray.start;
   invRay.dir = _trans.invMtx.toMat3() * ray.dir;
   
   vec3 dist(invRay.start - _center);
   double a = invRay.dir.dot(invRay.dir);
   double b = 2 * invRay.dir.dot(dist);
   double c = dist.dot(dist) - _radius*_radius;
   
   double disc = b*b - 4*a*c;
   if (disc < 0)
      return false;
   
   disc = sqrt(disc);
   a *= 2.0;
   
   double t0 = (-b + disc)/a;
   double t1 = (-b - disc)/a;
   
   double t = t0 < t1 ? t0 : t1;
   if (t < 0)
      t = t0 < t1 ? t1 : t0;
   
   if (t < 0 || collision->distance < t)
      return false;
   
   collision->distance = t;
   
   // find the location of the collision
   collision->loc = ray.pointAt(collision->distance);
   
   // set the normal
   vec3 loc = invRay.pointAt(t) - _center;
   collision->normal = _trans.invMtx.toMat3().transposed() * loc;
   collision->normal.normalize();
   collision->offset = collision->normal;
   
   // get the uvs
   loc.normalize();
   collision->uv.x = 0.5 - atan2(loc.z, loc.x)/(2*Pi);
   collision->uv.y = 0.5 + asin(loc.y)/Pi;
   
   // set this as the collided item
   collision->item = this;
   return true;
}

void Sphere::updateBounds() {
   vector<vec3>::iterator pt;
   vector<vec3> pts(8, _center);
   
   pts.at(0) += vec3(_radius);
   pts.at(1) += vec3(_radius, _radius, -_radius);
   pts.at(2) += vec3(_radius, -_radius, _radius);
   pts.at(3) += vec3(-_radius, _radius, _radius);
   
   pts.at(4) += vec3(_radius, -_radius, -_radius);
   pts.at(5) += vec3(-_radius, -_radius, _radius);
   pts.at(6) += vec3(-_radius, _radius, -_radius);
   pts.at(7) += vec3(-_radius);
   
   _bounds = bbox();
   for (pt = pts.begin(); pt != pts.end(); ++pt)
      _bounds.addPt(_trans.mtx * *pt);
}
