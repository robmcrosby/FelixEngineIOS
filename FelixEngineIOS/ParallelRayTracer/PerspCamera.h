//
//  PerspCamera.h
//  RayTracer
//
//  Created by Robert Crosby on 4/14/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef RayTracer_PerspCamera_h
#define RayTracer_PerspCamera_h

#include "Camera.h"

#define DEF_DIST 1.0f

class PerspCamera: public Camera {
public:
   PerspCamera(vec3 loc, vec3 up, vec3 right, vec3 look);
   virtual ~PerspCamera();
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual std::string getName() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual vec3 vectorOn(const ray3 &ray) const;
   virtual vec3 vectorFrom(const vec3 &loc) const;
   virtual void transform(const trans &t);
   
   virtual void createRays(RayNode *ray, const vec2 &loc) const;
   virtual ray3 createRay(double u, double v) const;
   
   inline void setFocus(double f)   {_focus = f;}
   inline void setApature(double a) {_apature = a;}
   inline void setSamples(int s)    {_samples = s;}
   
protected:
   vec3 _loc;
   vec3 _size;
   trans _trans;
   
   double _focus;
   double _apature;
   int _samples;
};

#endif
