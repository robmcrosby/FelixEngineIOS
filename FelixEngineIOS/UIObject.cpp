//
//  UIObject.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 10/5/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "UIObject.h"

using namespace std;

DEFINE_ENTITY_ID(UIObject)

/**
 *
 */
UIObject::UIObject(XMLTag *tag): Drawable(tag), _uiSubObjects(0), _uiParrent(0), _touchDeligate(0) {

}

/**
 *
 */
UIObject::~UIObject() {

}

/**
 *
 */
void UIObject::draw() const {
  // Do nothing for now.
}

/**
 * Adds a sub object.
 */
void UIObject::addUIObject(UIObject *obj) {
  if (!_uiSubObjects)
    _uiSubObjects = new set<UIObject*>();

  _uiSubObjects->insert(obj);
}

/**
 *
 */
void UIObject::updateUIObject(UIObject *obj) {
  
}

/**
 *
 */
void UIObject::removeUIObject(UIObject *obj) {
  if (_uiSubObjects) {
    _uiSubObjects->erase(obj);
  }
}

/**
 *
 */
bool UIObject::touchesBegin(const Touches &touches) {
  bool hit = false;

  if (_touchDeligate)
    hit = _touchDeligate->touchesBegin(touches);

  

  cout << "Touches Begin" << endl;
  return true;
}

/**
 *
 */
bool UIObject::touchesEnd(const Touches &touches) {
  cout << "Touches End" << endl;
  return true;
}

/**
 *
 */
bool UIObject::touchesMove(const Touches &touches) {
  cout << "Touches Move" << endl;
  return true;
}

/**
 *
 */
void UIObject::setTouchDeligate(TouchDeligate *deligate) {
  _touchDeligate = deligate;
}