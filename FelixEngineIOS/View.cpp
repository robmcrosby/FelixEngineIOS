//
//  View.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/7/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "View.h"
#include "FelixEngine.h"
#include "Pipeline.h"

using namespace std;

DEFINE_ENTITY_ID(View)

/**
 * Captures the current view enviroment.
 */
ViewEnviroment::ViewEnviroment() {
  _frameBuff = FrameBuff::GetActiveFBO();
  _shader = Shader::GetActiveShader();
  _uniforms = Shader::GetActiveUniforms();
}

/**
 * Re-applies the saved enviroment.
 */
ViewEnviroment::~ViewEnviroment() {
  if (_frameBuff)
    _frameBuff->use();
  if (_shader)
    _shader->use();
  Shader::SetActiveUniforms(_uniforms);
}


/**
 *
 */
View::View(const XMLTag &tag): UIObject(tag),
_viewFboName(FINAL_FBO), _viewFbo(0), _activePipeline(Pipeline::GetDefaultPipeline()) {
  createChildren(tag);
  applyTag(tag);
  clearPasses();
}

/**
 *
 */
View::~View() {
  
}

/**
 * Adds a Drawable to a pass.
 * @param drawable Drawable pointer.
 */
void View::addDrawable(const Drawable *drawable) {
  getPassDraws(drawable->getPassName())->insert(drawable);
  setChanged();
}

/**
 *
 */
void View::removeDrawable(const Drawable *drawable) {
  getPassDraws(drawable->getPassName())->erase(drawable);
  setChanged();
}

/**
 *
 */
View* View::getView() {
  return this;
}

/**
 *
 */
HostDisplay* View::getDisplay() const {
  Host *host = Host::GetHost();
  return host ? host->getDisplay() : NULL;
}

/**
 *
 */
void View::draw() const {
  // saves the previous enviroment and restores it when out of scope.
  ViewEnviroment parrentEnv;
  
  // run the view's pipeline.
  if (_activePipeline)
    _activePipeline->run(*this);
}

/**
 *
 */
void View::load() {
  // Get the view's FBO
  _viewFbo = Host::GetHost()->getDisplay()->getFrameBuff(_viewFboName);

  UIObject::load();
}

/**
 *
 */
void View::setPipeline(Pipeline *pipeline) {
  _activePipeline = pipeline;
}

/**
 *
 */
void View::setPipeline(const string &name) {
  _activePipeline = Pipeline::GetPipeline(name);
}

/**
 *
 */
void View::updateUI() {
  if (_viewFbo) {
    _uiSize = _viewFbo->getSize();

    // update the view projection matrix
    _uiProjMtx = mat4::Ortho(-_uiSize.x/2.0f, _uiSize.x/2.0f, -_uiSize.y/2.0f, _uiSize.y/2.0f, VIEW_NEAR, VIEW_FAR);
  }
}

/**
 *
 */
void View::applyTag(const XMLTag &tag) {
  if (tag.hasAttribute(ATT_PIPELINE))
    setPipeline(tag.getAttribute(ATT_PIPELINE));
  if (tag.hasAttribute(ATT_FBO))
    _viewFboName = tag.getAttribute(ATT_FBO);
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
  
  addPassUniform(VAR_PROJ_MTX, Uniform(VAL_MAT4X4, &_mainProjMtx), MAIN_PASS);
  addPassUniform(VAR_VIEW_MTX, Uniform(VAL_MAT4X4, &_mainViewMtx), MAIN_PASS);
  addPassUniform(VAR_PROJ_MTX, Uniform(VAL_MAT4X4, &_uiProjMtx), UI_PASS);
  addPassUniform(VAR_VIEW_MTX, Uniform(VAL_MAT4X4, &_uiViewMtx), UI_PASS);
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