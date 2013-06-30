//
//  PointLight.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "PointLight.h"

using namespace std;

PointLight::PointLight() {}

PointLight::PointLight(vec3 loc, col3 col):
   _loc(loc),
   _color(col) {}

PointLight::~PointLight() {
   
}

col3 PointLight::getColor() const {
   return _color;
}

ray3 PointLight::getRay(const vec3 &pt) const {
   return ray3(pt, _trans.mtx * _loc);
}

vec3 PointLight::getOrigin() const {
   return _trans.mtx * _loc;
}

bbox PointLight::getBounds() const {
   return _trans.mtx * _loc;
}

string PointLight::getName() const {
   return "Point Light";
}

void PointLight::write(ostream &os, int indent) const {
   os << " PointLight" << endl;
   os << "  loc:" << _trans.mtx * _loc << endl;
   os << "  color:" << _color << endl;
}

vec3 PointLight::vectorOn(const ray3 &ray) const {
   return _trans.mtx * _loc - ray.start;
}

vec3 PointLight::vectorFrom(const vec3 &loc) const {
   return _trans.mtx * _loc - loc;
}

void PointLight::transform(const trans &t) {
   _trans = t;
}