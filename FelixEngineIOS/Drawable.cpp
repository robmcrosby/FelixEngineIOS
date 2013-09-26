//
//  Drawable.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/12/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Drawable.h"
#include "View.h"
#include "FelixEngine.h"

using namespace std;

Drawable::Drawable(XMLTag *tag): Entity(tag),
_passName(MAIN_PASS), _layer(0), _visible(true), _drawType(DRAW_DEPTH), _drawId(getNewId()), _curView(0) {
  
}

Drawable::~Drawable() {
  
}

/**
 * Sets the visibilty.
 * @param vis A boolean value, true for visible and false for not visible.
 */
void Drawable::setVisiblity(bool vis) {
  if (_curView && _visible != vis) {
    _curView->setChanged();
    _visible = vis;
  }
}

/**
 * Sets how this is drawn.
 * @param type Either a blend or depth DRAW_TYPE.
 */
void Drawable::setDrawType(DRAW_TYPE type) {
  if (_curView && _drawType != type) {
    _curView->setChanged();
    _drawType = type;
  }
}

/**
 * Sets the pass that this is associated with.
 * @param pass A string value for the associated pass.
 */
void Drawable::setPassName(const std::string &pass) {
  if (_curView && _passName != pass) {
    _curView->removeDrawable(this);
    _passName = pass;
    _curView->addDrawable(this);
  }
}

/**
 * Sets the layer value.
 * @param layer An int value for the numbered layer.
 */
void Drawable::setLayer(int layer) {
  if (_curView && _layer != layer) {
    _curView->removeDrawable(this);
    _layer = layer;
    _curView->addDrawable(this);
  }
}

/**
 *
 */
void Drawable::load() {
  View *parrentView = getParrentView();

  // update the current view.
  if (_curView != parrentView) {
    // remove from a previous view.
    if (_curView)
      _curView->removeDrawable(this);

    // add to the parrent view.
    if (parrentView)
      parrentView->addDrawable(this);

    _curView = parrentView;
  }

  Entity::load();
}

/**
 *
 */
void Drawable::unload() {
  if (_curView)
    _curView->removeDrawable(this);
  _curView = NULL;

  Entity::unload();
}

/**
 * Draws this to the screen if it is visible.
 */
void Drawable::draw() const {
  
}

/**
 * Gets the pass and layer from the tag.
 */
void Drawable::applyTag() {
  if (_tag) {
    // set pass
    if (_tag->hasAttribute("pass"))
      _passName = _tag->getAttribute("pass");
    
    // set the layer
    if (_tag->hasAttribute("layer"))
      sscanf(_tag->getAttribute("layer").c_str(), " %i", &_layer);
  }
}

/**
 * Adds self to the pass.
 *//*
void Drawable::notify(const Event &event) {
  if (event == EVENT_RENDER && isVisible()) {
    View *view = getView();
    if (view)
      view->addDrawable(this);
  }
  
  Entity::notify(event);
}*/

int Drawable::getNewId() {
  static int Count = 0;
  
  return (Count++);
}
