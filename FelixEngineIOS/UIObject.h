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

enum Alignment {
  Top = 1,
  Right = 2,
  Bottom = 4,
  Left = 8,
};

enum Relation {
  None,
  Offset,
  Ratio,
};

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

  virtual void setParrent(Entity *parrent);

  virtual void load();
  virtual void unload();

  void addUIObject(UIObject *obj);
  void updateUIObject(UIObject *obj);
  void removeUIObject(UIObject *obj);

  virtual bool touchesBegin(const Touches &touches);
  virtual bool touchesEnd(const Touches &touches);
  virtual bool touchesMove(const Touches &touches);

  virtual void setTouchDeligate(TouchDeligate *deligate);

  inline const UIObject* getUIParrent() const;

  inline void setOffset(vec2 offset);
  inline void setSize(vec2 size);
  inline void setAlignment(int align);
  inline void setRelations(Relation width, Relation height);

  inline vec2 getUICenter() const;
  inline vec2 getUIUpper() const;
  inline vec2 getUILower() const;
  inline vec2 getUISize() const;

protected:
  inline void applyTag(const XMLTag &tag);
  inline Relation getRelation(const std::string &str);
  virtual void updateUI();

  vec2 _uiOffset; /**< Center offset. */
  vec2 _uiSize; /**< Size of the object's rectangle */
  int  _uiAlign; /**< Alignment with respect to the Parrent or View */
  Relation _uiRelationX; /**< Relation to the Parrent or View's width. */
  Relation _uiRelationY; /**< Relation to the Parrent or View's height. */

  std::set<UIObject*> *_uiSubObjects;

  UIObject *_uiParrent; /**< Parrent that contains this object. */
  TouchDeligate *_touchDeligate; /**< Alternate object to send touch calls to. */
};


void UIObject::setOffset(vec2 offset) {
  _uiOffset = offset;
}

void UIObject::setSize(vec2 size) {
  _uiSize = size;
}

void UIObject::setAlignment(int align) {
  _uiAlign = align;
}

void UIObject::setRelations(Relation width, Relation height) {
  _uiRelationX = width;
  _uiRelationY = height;
}

vec2 UIObject::getUICenter() const {
  const UIObject *parrent = getUIParrent();
  vec2 center = _uiOffset;

  if (parrent) {
    vec2 pSize = parrent->getUISize();
    vec2 size = getUISize();
    center += parrent->getUICenter();

    center.x += _uiAlign & Right ? (pSize.x - size.x)/2.0f : 0;
    center.x -= _uiAlign & Left ? (pSize.x - size.x)/2.0f : 0;
    center.y += _uiAlign & Top ? (pSize.y - size.y)/2.0f : 0;
    center.y -= _uiAlign & Bottom ? (pSize.y - size.y)/2.0f : 0;
  }

  return center;
}

vec2 UIObject::getUIUpper() const {
  return getUICenter() + getUISize()/2.0f;
}

vec2 UIObject::getUILower() const {
  return getUICenter() - getUISize()/2.0f;
}

vec2 UIObject::getUISize() const {
  const UIObject *parrent = getUIParrent();
  vec2 size = _uiSize;

  if (parrent) {
    vec2 pSize = parrent->getUISize();

    // Relation width
    if (_uiRelationX == Offset)
      size.x = pSize.x - 2.0f*size.x;
    else if (_uiRelationX == Ratio)
      size.x *= pSize.x;

    // Relation height
    if (_uiRelationY == Offset)
      size.y = pSize.y - 2.0f*size.y;
    else if (_uiRelationY == Ratio)
      size.y *= pSize.y;
  }

  return size;
}

const UIObject* UIObject::getUIParrent() const {
  return _uiParrent ? _uiParrent : (const UIObject*)getParrentView();
}

#endif /* defined(__FelixEngineIOS__UIObject__) */
