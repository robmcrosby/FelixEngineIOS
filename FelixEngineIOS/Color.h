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
   BLACK = 0x000f,
   WHITE = 0xffff,
   RED = 0xf00f,
   GREEN = 0x0f0f,
   BLUE = 0x00ff,
   ALPHA = 0x000f,
}

enum COLOR_TYPE {
   COLOR_GRAY,
   COLOR_GRAY_ALPHA,
   COLOR_RGB,
   COLOR_RGBA,
};

struct Color: public Vector4<float> {
   Color(): Vector4(0, 0, 0, 1.0) {}
}

#endif
