//
//  Triangle.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__Triangle__
#define __RayTracer__Triangle__

#include "Traceable.h"

class Triangle: public Traceable {
public:
   Triangle();
   Triangle(const vec3 *pts, Material mat);
   Triangle(const vec3 *pts, const vec2 *uvs, Material mat);
   Triangle(const vec3 *pts, const vec3 *norms, Material mat);
   Triangle(const vec3 *pts, const vec3 *norms, const vec2 *uvs, Material mat);
   virtual ~Triangle();
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual void transform(const trans &t);
   
   virtual bool getCollision(const ray3 &ray, Collision *collision) const;
   virtual const Material* getMaterial() const {return &_material;}
   
protected:
   inline vec3 getMean() const;
   inline void setPoints(const vec3 *pts);
   inline void setNormals(const vec3 *norms);
   inline void setUVs(const vec2 *uvs);
   inline void setNormal();
   
   vec3 *_points;
   vec3 *_normals;
   vec2 *_uvs;
   vec3 _normal;
   Material _material;
   trans _trans;
};

#endif /* defined(__RayTracer__Triangle__) */
