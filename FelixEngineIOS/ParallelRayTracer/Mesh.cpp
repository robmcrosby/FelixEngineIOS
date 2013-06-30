//
//  Mesh.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/6/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Mesh.h"

using namespace std;

bool MeshTri::getCollision(const ray3 &ray, double *dist, vec3 *normal, vec2 *uv) const {
   vec3 p = ray.dir.cross(_edge2);
   double t, a = _edge1.dot(p);
   
   if (a > -0.00001 && a < 0.00001)
      return false;
   
   double f = 1/a;
   vec3 s = ray.start - *_points.x;
   double u = f * s.dot(p);
   if (u < 0)
      return false;
   
   vec3 q = s.cross(_edge1);
   double v = f * ray.dir.dot(q);
   if (v < 0 || u+v > 1.0)
      return false;
   
   t = f * _edge2.dot(q);
   
   if (t < 0)
      return false;
   
   *dist = t;
   
   // find the normal
   if (_normals.x != NULL)
      *normal = *_normals.y*u + *_normals.z*v + *_normals.x*(1-u-v);
   else
      *normal = _normal;
   
   // find the uv
   if (_uvs.x != NULL)
      *uv = *_uvs.y*u + *_uvs.z*v + *_uvs.x*(1-u-v);
   else
      *uv = 0;
   
   return true;
}

void MeshTri::setPoints(const vec3 *points, const ivec3 &idxs) {
   _points.x = points+idxs.x;
   _points.y = points+idxs.y;
   _points.z = points+idxs.z;
   setNormal();
}
void MeshTri::setNormals(const vec3 *normals, const ivec3 &idxs) {
   _normals.x = normals+idxs.x;
   _normals.y = normals+idxs.y;
   _normals.z = normals+idxs.z;
}

void MeshTri::setUVs(const vec2 *uvs, const ivec3 &idxs) {
   _uvs.x = uvs+idxs.x;
   _uvs.y = uvs+idxs.y;
   _uvs.z = uvs+idxs.z;
}

void MeshTri::setNormal() {
   _edge1 = *_points.y - *_points.x;
   _edge2 = *_points.z - *_points.x;
   _normal = _edge1.cross(_edge2).normalized();
}



Mesh::Mesh(const vec3 &low, const vec3 &high, const Material &mat): _mat(mat) {
   // create a cube
   
   // add points
   addPoint(high);
   addPoint(vec3(high.x, high.y, low.z));
   addPoint(vec3(low.x, high.y, low.z));
   addPoint(vec3(low.x, high.y, high.z));
   addPoint(vec3(high.x, low.y, high.z));
   addPoint(vec3(high.x, low.y, low.z));
   addPoint(low);
   addPoint(vec3(low.x, low.y, high.z));
   
   // add normals
   addNormal(vec3(0, 1, 0));  //top
   addNormal(vec3(0, -1, 0)); //bottom
   addNormal(vec3(0, 0, 1));  //front
   addNormal(vec3(0, 0, -1)); //back
   addNormal(vec3(1, 0, 0));  //right
   addNormal(vec3(-1, 0, 0)); //left
   
   // add uvs
   addUV(vec2(0, 0));
   addUV(vec2(1, 0));
   addUV(vec2(1, 1));
   addUV(vec2(0, 1));
   
   // add triangles
   // top
   addTri(ivec3(0, 1, 2), 0, ivec3(2, 3, 0));
   addTri(ivec3(0, 2, 3), 0, ivec3(2, 0, 1));
   
   // bottom
   addTri(ivec3(4, 7, 6), 1, ivec3(2, 3, 0));
   addTri(ivec3(4, 6, 5), 1, ivec3(2, 0, 1));
   
   // front
   addTri(ivec3(0, 3, 7), 2, ivec3(2, 3, 0));
   addTri(ivec3(0, 7, 4), 2, ivec3(2, 0, 1));
   
   // back
   addTri(ivec3(2, 1, 5), 3, ivec3(2, 3, 0));
   addTri(ivec3(2, 5, 6), 3, ivec3(2, 0, 1));
   
   // right
   addTri(ivec3(1, 0, 4), 4, ivec3(2, 3, 0));
   addTri(ivec3(1, 4, 5), 4, ivec3(2, 0, 1));
   
   // left
   addTri(ivec3(3, 2, 6), 5, ivec3(2, 3, 0));
   addTri(ivec3(3, 6, 7), 5, ivec3(2, 0, 1));
}

Mesh::~Mesh() {
   
}

vec3 Mesh::getOrigin() const {
   return _bounds.center();
}

bbox Mesh::getBounds() const {
   return _bounds;
}

void Mesh::write(ostream &os, int indent) const {
   os << " Mesh" << endl;
}

void Mesh::transform(const trans &t) {
   _trans = t;
   updateBounds();
}

bool Mesh::getCollision(const ray3 &ray, Collision *collision) const {
   vector<MeshTri>::const_iterator tri;
   bool hit = false;
   ray3 invRay;
   vec3 normal;
   vec2 uv;
   double dist;
   
   invRay.start = _trans.invMtx * ray.start;
   invRay.dir = _trans.invMtx.toMat3() * ray.dir;
   
   for (tri = _tris.begin(); tri != _tris.end(); ++tri) {
      if (tri->getCollision(invRay, &dist, &normal, &uv) && dist < collision->distance) {
         collision->item = this;
         collision->distance = dist;
         collision->loc = ray.pointAt(dist);
         collision->offset = _trans.invMtx.toMat3().transposed() * tri->getNormal();
         collision->offset.normalize();
         collision->normal = _trans.invMtx.toMat3().transposed() * normal;
         collision->normal.normalize();
         collision->uv = uv;
         hit = true;
      }
   }
   
   return hit;
}

void Mesh::updateBounds() {
   vector<vec3>::iterator pt;
   
   _bounds = bbox();
   for (pt = _points.begin(); pt != _points.end(); ++pt)
      _bounds.addPt(_trans.mtx * *pt);
}