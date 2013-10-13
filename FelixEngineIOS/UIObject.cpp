//
//  UIObject.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 10/5/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "UIObject.h"
#include "FelixEngine.h"

using namespace std;

DEFINE_ENTITY_ID(UIObject)

/**
 *
 */
UIObject::UIObject(const XMLTag &tag): Drawable(tag), _uiSubObjects(0), _uiParrent(0), _touchDeligate(0), _uiSize(1) {
  applyTag(tag);
}

/**
 *
 */
UIObject::~UIObject() {

}

/**
 *
 */
void UIObject::setParrent(Entity *parrent) {
  if (parrent != getParrent()) {
    // remove from the old parrent.
    UIObject *uiParrent = dynamic_cast<UIObject*>(getParrent());
    if (uiParrent)
      uiParrent->removeUIObject(this);

    // add to the new parrent.
    uiParrent = dynamic_cast<UIObject*>(parrent);
    if (uiParrent)
      uiParrent->addUIObject(this);
  }

  Drawable::setParrent(parrent);
}

/**
 *
 */
void UIObject::load() {
  updateUI();
  Drawable::load();
}

/**
 *
 */
void UIObject::unload() {
  Drawable::unload();
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

  //if (_touchDeligate)
  //  hit = _touchDeligate->touchesBegin(touches);
  //cout << "Touches Begin" << endl;
  return hit;
}

/**
 *
 */
bool UIObject::touchesEnd(const Touches &touches) {
  bool hit = false;

  //cout << "Touches End" << endl;
  return hit;
}

/**
 *
 */
bool UIObject::touchesMove(const Touches &touches) {
  bool hit = false;

  //cout << "Touches Move" << endl;
  return hit;
}

/**
 *
 */
void UIObject::setTouchDeligate(TouchDeligate *deligate) {
  _touchDeligate = deligate;
}

/**
 *
 */
void UIObject::applyTag(const XMLTag &tag) {
  const XMLTag *subtag;

  subtag = tag.getSubTag("size");
  if (subtag)
    _uiSize = vec2::ParseFloat(subtag->getContents());

  subtag = tag.getSubTag("offset");
  if (subtag)
    _uiOffset = vec2::ParseFloat(subtag->getContents());
}

/**
 *
 */
void UIObject::updateUI() {
  _transform.setScale(vec3(_uiSize, 1));
  _transform.setPos(vec3(_uiOffset, 0));
}