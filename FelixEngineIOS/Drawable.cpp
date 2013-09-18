//
//  Drawable.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/12/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Drawable.h"
#include "View.h"

using namespace std;

Drawable::Drawable(XMLTag *tag): Entity(tag),
_passName(MAIN_PASS), _layer(0), _visible(true), _drawType(DRAW_DEPTH), _drawId(getNewId()) {
  
}

Drawable::~Drawable() {
  
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
 */
void Drawable::notify(const Event &event) {
  if (event == EVENT_RENDER && isVisible()) {
    View *view = getView();
    if (view)
      view->addDrawable(this);
  }
  
  Entity::notify(event);
}

int Drawable::getNewId() {
  static int Count = 0;
  
  return (Count++);
}
