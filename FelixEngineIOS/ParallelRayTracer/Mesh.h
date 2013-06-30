//
//  Mesh.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/6/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__Mesh__
#define __ParallelRayTracer__Mesh__

#include "Traceable.h"

class MeshTri {
public:
   MeshTri(): _points(0), _normals(0), _uvs(0) {}
   
   inline bool getCollision(const ray3 &ray, double *dist, vec3 *normal, vec2 *uv) const;
   inline vec3 getNormal() const {return _normal;}
   
   void setPoints(const vec3 *points, const ivec3 &idxs);
   void setNormals(const vec3 *normals, const ivec3 &idxs);
   void setUVs(const vec2 *uvs, const ivec3 &idxs);
protected:
   inline void setNormal();
   
   Vector3<const vec3*> _points;
   Vector3<const vec3*> _normals;
   Vector3<const vec2*> _uvs;
   vec3 _normal;
   vec3 _edge1;
   vec3 _edge2;
};

class Mesh: public Traceable {
public:
   Mesh() {}
   Mesh(const Material &mat): _mat(mat) {}
   Mesh(const vec3 &low, const vec3 &high, const Material &mat);
   virtual ~Mesh();
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual void transform(const trans &t);
   
   virtual bool getCollision(const ray3 &ray, Collision *collision) const;
   virtual const Material* getMaterial() const {return &_mat;}
   
   inline void addPoint(const vec3 &pt) {
      _points.push_back(pt);
      _bounds.addPt(pt);
   }
   inline void addNormal(const vec3 &norm) {_normals.push_back(norm);}
   inline void addUV(const vec2 &uv) {_uvs.push_back(uv);}
   inline void addTri(const ivec3 &pts, const ivec3 &norms, const ivec3 &uvs) {
      _tris.push_back(MeshTri());
      _tris.back().setPoints(&_points[0], pts);
      _tris.back().setNormals(&_normals[0], norms);
      _tris.back().setUVs(&_uvs[0], uvs);
   }
protected:
   inline void updateBounds();
   
   std::vector<vec3> _points;
   std::vector<vec3> _normals;
   std::vector<vec2> _uvs;
   std::vector<MeshTri> _tris;
   
   Material _mat;
   trans _trans;
   bbox _bounds;
};

#endif /* defined(__ParallelRayTracer__Mesh__) */
