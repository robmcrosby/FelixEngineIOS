//
//  Traceable.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef RayTracer_Traceable_h
#define RayTracer_Traceable_h

#include "Item.h"
#include "ImageBuffer.h"

class Traceable;
class Collision;

struct Material {
   Material(): img(0), amb(0), dif(0), spc(0), flt(0), tra(0), rou(0), ref(0), ior(1.0) {}
   
   inline void write(std::ostream &os) const {
      os << "color:" << color;
      os << " amb:" << amb;
      os << " dif:" << dif;
      os << " spc:" << spc;
      os << " rough: " << rou;
   }
   inline col3 getColor(vec2 uv = 0) const {
      if (img)
         return img->getCol3(uv);
      return color;
   }
   inline col3 getAmbient(vec2 uv = 0)  const {return getColor(uv) * amb;}
   inline col3 getDiffuse(vec2 uv = 0)  const {return getColor(uv) * dif;}
   inline col3 getSpecular(vec2 uv = 0) const {return getColor(uv) * spc;}
   inline double getSmooth() const {return rou < 0.001 ? 1000 : 1.0/rou;}
   
   col3 color;
   ImageBuffer *img;
   double amb; //ambient
   double dif; //diffuse
   double spc; //specular
   double flt; //filter
   double tra; //transmit
   double rou; //roughness
   double ref; //reflection
   double fra; //refraction
   double ior; //index of refraction
};

struct Collision {
   Collision(): distance(std::numeric_limits<double>::max()), item(NULL) {}
   Collision(const ray3 &ray, const Item *item = NULL): distance(ray.length), item(item) {
      loc = ray.pointAt(ray.length);
   }
   bool operator<(const Collision &rhs) const {
      return distance < rhs.distance;
   }
      
   vec3 loc;
   vec3 normal;
   vec3 offset;
   vec2 uv;
   double distance;
   const Item *item;
};

struct Traceable: public Item {
   virtual ~Traceable() {}
   virtual bool getCollision(const ray3 &ray, Collision *collision) const = 0;
   virtual const Material* getMaterial() const = 0;
};

#endif
