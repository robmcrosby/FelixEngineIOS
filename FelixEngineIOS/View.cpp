//
//  View.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/7/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "View.h"
#include "FelixEngine.h"


using namespace std;

DEFINE_ENTITY_ID(View)

/**
 *
 */
View::View(XMLTag *tag): Drawable(tag) {
  if (_tag)
    createChildren(_tag);
  clearPasses();
}

/**
 *
 */
View::~View() {
  
}

/**
 *
 */
void View::notify(const Event &event) {
  if (event == EVENT_RENDER) {
    View *view = getParrentView();
    if (view)
      view->addDrawable(this);
    
    emptyPasses();
    notifyListeners(event);
  }
  else
    Drawable::notify(event);
}

/**
 * Adds a Drawable to a pass.
 * @param drawable Drawable pointer.
 */
void View::addDrawable(const Drawable *drawable) {
  getPassDraws(drawable->getPassName())->insert(drawable);
}

/**
 *
 */
View* View::getView() {
  return this;
}

HostDisplay* View::getDisplay() {
  return Host::GetHost()->getDisplay();
}

/**
 *
 */
void View::draw() const {
  // run the view's pipeline.
  drawPass(MAIN_PASS);
}

/**
 * Empties or clears all of the Drawables from all of the passes.
 */
void View::emptyPasses() {
  for (Passes::iterator itr = _passes.begin(); itr != _passes.end(); ++itr)
    itr->second.draws.clear();
}

/**
 * Clears or removes all of the passes.
 */
void View::clearPasses() {
  _passes.clear();
  
  addPassUniform(VAR_PROJ_MTX, Uniform(VAL_MAT4X4, &_projMtx), MAIN_PASS);
  addPassUniform(VAR_VIEW_MTX, Uniform(VAL_MAT4X4, &_viewMtx), MAIN_PASS);
  _curPass = MAIN_PASS;
}

/**
 * 
 */
Pass* View::getPass(const std::string &pass) {
  if (_passes.find(pass) == _passes.end())
    _passes[pass] = Pass();
  return &_passes[pass];
}

/**
 *
 */
Draws* View::getPassDraws(const std::string &pass) {
  return &getPass(pass)->draws;
}

/**
 *
 */
Uniforms* View::getPassUniforms(const std::string &pass) {
  return &getPass(pass)->uniforms;
}

/**
 *
 */
const Pass* View::getPass(const std::string &pass) const {
  if (_passes.find(pass) == _passes.end())
    return NULL;
  return &_passes.at(pass);
}

/**
 *
 */
const Draws* View::getPassDraws(const std::string &pass) const {
  const Pass *passPtr = getPass(pass);
  if (passPtr)
    return &passPtr->draws;
  return NULL;
}

/**
 *
 */
const Uniforms* View::getPassUniforms(const std::string &pass) const {
  const Pass *passPtr = getPass(pass);
  if (passPtr)
    return &passPtr->uniforms;
  return NULL;
}