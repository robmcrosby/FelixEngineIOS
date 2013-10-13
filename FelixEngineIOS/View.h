//
//  View.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/7/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__View__
#define __FelixEngineIOS__View__

#include "UIObject.h"
#include "Matrix.h"

#define ATT_PIPELINE "pipeline"
#define ATT_FBO "fbo"

#define VIEW_NEAR -100
#define VIEW_FAR  100

class HostDisplay;
class Pipeline;

/**
 * Comparator for Drawables
 */
struct DrawCmp {
  inline bool operator() (const Drawable *lhs, const Drawable *rhs) const {
    return (*lhs < *rhs);
  }
};

/**
 * Collection of Drawables
 */
typedef std::set<const Drawable*, DrawCmp> Draws;

/**
 * Holds the Drawables and Uniforms for a pass.
 */
struct Pass {
  Draws draws;
  Uniforms uniforms;
};

typedef std::map<std::string, Pass> Passes;

/**
 * Captures the view enviroment and re-applies it when destructed.
 */
class ViewEnviroment {
public:
  ViewEnviroment();
  ~ViewEnviroment();
  
private:
  const Shader *_shader;
  const Uniforms *_uniforms;
  const FrameBuff *_frameBuff;
};

/**
 * View Class
 */
class View: public UIObject {
public:
  View(const XMLTag &tag);
  virtual ~View();

  virtual View* getView();
  virtual HostDisplay* getDisplay() const;
  virtual void draw() const;

  virtual void load();

  void setPipeline(Pipeline *pipeline);
  void setPipeline(const std::string &name);
  
  void addDrawable(const Drawable *drawable);
  void updateDrawable(const Drawable *drawable, const std::string &prevPass);
  void removeDrawable(const Drawable *drawable);
  
  inline void drawPass(const std::string &passName) const;

  inline void addPassUniform(const std::string &key, const Uniform &uniform, const std::string &pass);
  inline void removePassUniform(const std::string &key, const std::string &pass);
  inline void clearPassUniforms(const std::string &pass);
  inline void setChanged();
  
protected:
  virtual void updateUI();

  std::string _viewFboName;
  const FrameBuff *_viewFbo;
  mat4 _mainProjMtx;
  mat4 _viewProjMtx;
  
private:
  inline void applyTag(const XMLTag &tag);
  inline void emptyPasses();
  inline void clearPasses();
  
  inline Pass* getPass(const std::string &pass);
  inline Draws* getPassDraws(const std::string &pass);
  inline Uniforms* getPassUniforms(const std::string &pass);
  
  inline const Pass* getPass(const std::string &pass) const;
  inline const Draws* getPassDraws(const std::string &pass) const;
  inline const Uniforms* getPassUniforms(const std::string &pass) const;
  
  Passes _passes;
  bool _changed;
  const Pipeline *_activePipeline;
  
  DECLARE_ENTITY_ID(View)
};



/*
 * View's public inline implementations.
 */

/**
 * Draws all of the Drawables in a pass.
 * @param pass string name of a pass to be drawn.
 */
void View::drawPass(const std::string &passName) const {
  const Pass *pass = getPass(passName);
  
  if (pass) {
    Shader::SetActiveUniforms(&pass->uniforms);
    
    for (Draws::const_iterator drawable = pass->draws.begin(); drawable != pass->draws.end(); ++drawable)
      (*drawable)->draw();
  }
}

/**
 * Adds a Uniform key value pair to a collection of Uniforms associated with a pass.
 * @param key of the Uniform value.
 * @param uniform Uniform to be added.
 * @param pass string for the name of the pass.
 */
void View::addPassUniform(const std::string &key, const Uniform &uniform, const std::string &pass) {
  getPassUniforms(pass)->addUniform(key, uniform);
}

/**
 * Removes a Uniform value associated with the key on a pass.
 * @param name string name of the Uniform value.
 * @param pass string name of the pass.
 */
void View::removePassUniform(const std::string &key, const std::string &pass) {
  getPassUniforms(pass)->remove(key);
}

/**
 * Clears or removes all of the Uniform values for a pass.
 * @param pass string name of the pass.
 */
void View::clearPassUniforms(const std::string &pass) {
  getPassUniforms(pass)->clear();
}

/**
 * Updates the changed variable
 */
void View::setChanged() {
  if (!_changed) {
    _changed = true;

    View *curView = getCurView();
    if (curView)
      curView->setChanged();
  }
}

/**
 *
 */
const Pass* View::getPass(const std::string &pass) const {
  if (_passes.find(pass) == _passes.end())
    return NULL;
  return &_passes.at(pass);
}


#endif /* defined(__FelixEngineIOS__UIView__) */
