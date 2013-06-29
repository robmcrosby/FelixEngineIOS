//
//  Color.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/22/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_Color_h
#define FelixEngineIOS_Color_h

#include "Vector.h"

enum COLOR {
   ALPHA = 0x000000ff,
   RED   = 0xff000000,
   GREEN = 0x00ff0000,
   BLUE  = 0x0000ff00,
   WHITE = 0xffffff00,
};

enum COLOR_TYPE {
   COLOR_GRAY,
   COLOR_GRAY_ALPHA,
   COLOR_RGB,
   COLOR_RGBA,
};

struct Color: public Vector4<float> {
   Color(): Vector4(0, 0, 0, 1.0) {}
   Color(COLOR c) {setColor(c);}
   Color(unsigned int c) {setColor(c);}
   
   template <typename I>
   Color(const Vector4<I> &v) {r = v.x; g = v.y; b = v.z; a = v.w;}
   
   template <typename I>
   void operator=(const Vector4<I> v) {r = v.x; g = v.y; b = v.z; a = v.w;}
   
   inline void setColor(unsigned int c) {
      a = (c & 0xff) / 255.0f;
      c >>= 8;
      b = (c & 0xff) / 255.0f;
      c >>= 8;
      g = (c & 0xff) / 255.0f;
      c >>= 8;
      r = (c & 0xff) / 255.0f;
   }
};

#endif
