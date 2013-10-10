//
//  UIElement.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_UIElement_h
#define FelixEngineIOS_UIElement_h

#include "Vector.h"
#include <vector>

/**
 * Holds move information for touches on the screen.
 *//*
struct Touch {
  Touch(float curX, float curY, float lastX, float lastY): cur(curX, curY), last(lastX, lastY) {}

  vec2 cur;
  vec2 last;
};
typedef std::vector<Touch> Touches;

struct TouchDeligate {
  virtual ~TouchDeligate() {}

  virtual bool touchesBegin(const Touches &touches) = 0;
  virtual bool touchesEnd(const Touches &touches) = 0;
  virtual bool touchesMove(const Touches &touches) = 0;
};

struct UIElement: public TouchDeligate {
  virtual ~UIElement() {}

  virtual void setTouchDeligate(TouchDeligate *deligate) = 0;
};*/

#endif
