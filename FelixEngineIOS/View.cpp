//
//  View.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/7/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "View.h"


using namespace std;

DEFINE_ENTITY_ID(View)

/**
 *
 */
View::View(XMLTag *tag): Drawable(tag) {
  cout << "Created View: " << getName() << endl;
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
  
  if (event == EVENT_TOUCH_DOWN) {
    const Moves *moves = (const Moves*)event.data;
    
    cout << "Handle touch down event: " << moves->front().cur << endl;
  }
  else {
    Entity::notify(event);
  }
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
  
  //addPassUniform(VAR_PROJ_MTX, Uniform(VAL_MAT4X4, &_defProjMtx), MAIN_PASS);
  //addPassUniform(VAR_VIEW_MTX, Uniform(VAL_MAT4X4, &_defViewMtx), MAIN_PASS);
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