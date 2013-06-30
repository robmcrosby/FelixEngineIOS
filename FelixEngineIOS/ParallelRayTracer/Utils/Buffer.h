//
//  Buffer.h
//  FirstRays
//
//  Created by Robert Crosby on 3/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FirstRays_Buffer_h
#define FirstRays_Buffer_h

#include <vector>
#include "Vector.h"
#include "Color.h"

template <typename T>
struct Buffer2D {
   Buffer2D(ivec2 size = ivec2(1), T color = T()):
      _size(size),
      _data(size.x, std::vector<T>(size.y, color)) {}
   Buffer2D(int size, T color = T()):
      _size(size),
      _data(size, std::vector<T>(size, color)) {}
   Buffer2D(int width, int height, T color = T()):
      _size(width, height),
      _data(width, std::vector<T>(height, color)) {}
   
   std::vector<T>& operator[](int i) {return _data[i];}
   
   virtual void resize(ivec2 size, T color = T()) {
      _size = size;
      _data.resize(size.x, std::vector<T>(size.y, color));
      for (size_t i = 0; i < _data.size(); ++i) {
         _data.at(i).resize(size.y, color);
      }
   }
   virtual void resize(int size, T color = T())
      {resize(ivec2(size), color);}
   virtual void resize(int width, int height, T color = T())
      {resize(ivec2(width, height), color);}
   
   inline void clear(T color = T()) {
      for (size_t i = 0; i < _data.size(); ++i) {
         _data.at(i).clear();
         _data.at(i).resize(_size.y, color);
      }
   }
   inline T& nearestClamped(const vec2 &params) {
      ivec2 loc = params * _size + vec2(0.5f);
      loc.x = loc.x < 0 ? 0 : loc.x < _size.x ? loc.x : _size.x - 1;
      loc.y = loc.y < 0 ? 0 : loc.y < _size.y ? loc.y : _size.y - 1;
      return _data[loc.x][loc.y];
   }
   inline T& nearestRepeat(const vec2 &params) {
      ivec2 loc = params * _size + vec2(0.5f);
      loc.x %= _size.x;
      loc.y %= _size.y;
      loc.x += loc.x < 0 ? _size.x : 0;
      loc.y += loc.y < 0 ? _size.y : 0;
      return _data[loc.x][loc.y];
   }
   inline T *ptrAt(int u, int v) {return &_data[u][v];}
   inline T *ptrAt(ivec2 p)    {return ptrAt(p.x, p.y);}
   
   inline ivec2 size()   const {return _size;}
   inline int   width()  const {return _size.x;}
   inline int   height() const {return _size.y;}
   
protected:
   std::vector<std::vector<T> > _data;
   ivec2 _size;
};


#endif
