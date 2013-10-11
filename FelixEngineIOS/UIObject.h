//
//  UIObject.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 10/5/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__UIObject__
#define __FelixEngineIOS__UIObject__

#include "Drawable.h"
#include <set>
#include <vector>

/**
 * Holds move information for touches on the screen.
 */
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

class UIObject;

typedef std::set<UIObject*>::iterator UIObjectIterator;

class UIObjectMap {
public:
  UIObjectMap();

  void addUIObject(UIObject *obj);
  void updateUIObject(UIObject *obj);
  void removeUIObject(UIObject *obj);

  void selectUIObjectsAtPt(vec2 point);

  UIObjectIterator begin();
  UIObjectIterator end();

private:
  std::set<UIObject*> _map; /**< Replace with a 2d spacel data structure. */
};



class UIObject: public Drawable, public TouchDeligate {
public:
  UIObject(const XMLTag &tag);
  virtual ~UIObject();

  virtual void draw() const;

  void addUIObject(UIObject *obj);
  void updateUIObject(UIObject *obj);
  void removeUIObject(UIObject *obj);

  virtual bool touchesBegin(const Touches &touches);
  virtual bool touchesEnd(const Touches &touches);
  virtual bool touchesMove(const Touches &touches);

  virtual void setTouchDeligate(TouchDeligate *deligate);

protected:
  vec2 _uiLocation; /**< Top left location of the object's rectangle */
  vec2 _uiSize; /**< Size of the object's rectangle */

  std::set<UIObject*> *_uiSubObjects;

  UIObject *_uiParrent; /**< Parrent that contains this object. */
  TouchDeligate *_touchDeligate; /**< Alternate object to send touch calls to. */

  DECLARE_ENTITY_ID(UIObject)
};

#endif /* defined(__FelixEngineIOS__UIObject__) */
