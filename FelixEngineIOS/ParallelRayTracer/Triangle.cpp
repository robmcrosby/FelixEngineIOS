//
//  Triangle.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Triangle.h"

using namespace std;

Triangle::Triangle(): _points(0), _normals(0), _uvs(0) {}

Triangle::Triangle(const vec3 *pts, Material mat): _material(mat), _normals(0), _uvs(0) {
   setPoints(pts);
   setNormal();
}

Triangle::Triangle(const vec3 *pts, const vec2 *uvs, Material mat): _material(mat), _normals(0) {
   setPoints(pts);
   setNormal();
   setUVs(uvs);
}

Triangle::Triangle(const vec3 *pts, const vec3 *norms, Material mat): _material(mat), _uvs(0) {
   setPoints(pts);
   setNormal();
   setNormals(norms);
}

Triangle::Triangle(const vec3 *pts, const vec3 *norms, const vec2 *uvs, Material mat): _material(mat) {
   setPoints(pts);
   setNormal();
   setNormals(norms);
   setUVs(uvs);
}

Triangle::~Triangle() {
   delete _points;
   delete _normals;
   delete _uvs;
}

vec3 Triangle::getOrigin() const {
   return getMean();
}

bbox Triangle::getBounds() const {
   bbox box(_trans.mtx * _points[0]);
   box.addPt(_trans.mtx * _points[1]);
   box.addPt(_trans.mtx * _points[2]);
   return box;
}

void Triangle::write(ostream &os, int indent) const {
   os << " Triangle" << endl;
}

void Triangle::transform(const trans &t) {
   _trans = t;
}

bool Triangle::getCollision(const ray3 &ray, Collision *collision) const {
   vec3 e1 = _points[1] - _points[0];
   vec3 e2 = _points[2] - _points[0];
   vec3 p;
   ray3 invRay;
   
   invRay.start = _trans.invMtx * ray.start;
   invRay.dir = _trans.invMtx.toMat3() * ray.dir;
   
   p = invRay.dir.cross(e2);
   double a = e1.dot(p);
   
   if (a > -0.00001 && a < 0.00001)
      return false;
   
   double f = 1/a;
   vec3 s = invRay.start - _points[0];
   double u = f * s.dot(p);
   if (u < 0)
      return false;
   
   vec3 q = s.cross(e1);
   double v = f * invRay.dir.dot(q);
   if (v < 0 || u+v > 1.0)
      return false;
   
   double t = f * e2.dot(q);
   
   // distance and location
   if (t < 0 || collision->distance < t)
      return false;
   collision->item = this;
   collision->distance = t;
   collision->loc = ray.pointAt(t);
   
   // normal and offset
   collision->offset = _trans.mtx.toMat3() * _normal;
   collision->offset.normalize();
   if (_normals) {
      collision->normal = _normals[1]*u + _normals[2]*v + _normals[0]*(1-u-v);
      collision->normal = _trans.invMtx.toMat3().transposed() * collision->normal;
   }
   else
      collision->normal = collision->offset;
   
   // uvs
   if (_uvs)
      collision->uv = _uvs[1]*u + _uvs[2]*v + _uvs[0]*(1-u-v);
   
   return true;
}

vec3 Triangle::getMean() const {
   return (_points[0] + _points[1] + _points[2]) / 3.0;
}

void Triangle::setPoints(const vec3 *pts) {
   _points = new vec3[3];
   _points[0] = pts[0];
   _points[1] = pts[1];
   _points[2] = pts[2];
}

void Triangle::setNormals(const vec3 *norms) {
   _normals = new vec3[3];
   _normals[0] = norms[0];
   _normals[1] = norms[1];
   _normals[2] = norms[2];
}

void Triangle::setUVs(const vec2 *uvs) {
   _uvs = new vec2[3];
   _uvs[0] = uvs[0];
   _uvs[1] = uvs[1];
   _uvs[2] = uvs[2];
}

void Triangle::setNormal() {
   vec3 v0 = _points[1] - _points[0];
   vec3 v1 = _points[2] - _points[0];
   
   _normal = v0.cross(v1).normalized();
}