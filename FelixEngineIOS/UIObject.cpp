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

/**
 *
 */
UIObject::UIObject(const XMLTag &tag): Drawable(tag),
_uiSubObjects(0), _uiParrent(0), _touchDeligate(0), _uiSize(1), _uiAlign(0), _uiRelationX(None), _uiRelationY(None) {
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
    if (_uiParrent)
      _uiParrent->removeUIObject(this);

    // add to the new parrent.
    _uiParrent = dynamic_cast<UIObject*>(parrent);
    if (_uiParrent)
      _uiParrent->addUIObject(this);
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

  // Set the size.
  subtag = tag.getSubTag("size");
  if (subtag) {
    _uiSize = vec2::ParseFloat(subtag->getContents());

    if (subtag->hasAttribute("relationX"))
      _uiRelationX = getRelation(subtag->getAttribute("relationX"));
    if (subtag->hasAttribute("relationY"))
      _uiRelationY = getRelation(subtag->getAttribute("relationY"));
  }

  // Set the offset.
  subtag = tag.getSubTag("offset");
  if (subtag)
    _uiOffset = vec2::ParseFloat(subtag->getContents());

  // Set the alignment.
  subtag = tag.getSubTag("align");
  if (subtag) {
    _uiAlign = 0;
    _uiAlign |= subtag->getContents().find("Top") != string::npos ? Top : 0;
    _uiAlign |= subtag->getContents().find("Right") != string::npos ? Right : 0;
    _uiAlign |= subtag->getContents().find("Bottom") != string::npos ? Bottom : 0;
    _uiAlign |= subtag->getContents().find("Left") != string::npos ? Left : 0;
  }
}

/**
 * Gets the relation from a string.
 */
Relation UIObject::getRelation(const std::string &str) {
  Relation relation = None;
  if (str.find("offset") != string::npos)
    relation = Offset;
  else if (str.find("ratio") != string::npos)
    relation = Ratio;
  return relation;
}

/**
 * 
 */
void UIObject::updateUI() {

}