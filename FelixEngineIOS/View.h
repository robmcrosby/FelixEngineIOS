//
//  View.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/7/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__View__
#define __FelixEngineIOS__View__

#include "Drawable.h"
#include "UIElement.h"

class HostDisplay;

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
class View: public Drawable, public TouchDeligate {
public:
  View(XMLTag *tag = NULL);
  virtual ~View();

  virtual View* getView();
  virtual HostDisplay* getDisplay();
  virtual void draw() const;
  
  void addDrawable(const Drawable *drawable);
  void updateDrawable(const Drawable *drawable, const std::string &prevPass);
  void removeDrawable(const Drawable *drawable);

  void addUIElement(UIElement *element);
  void updateUIElement(UIElement *element);
  void removeUIElement(UIElement *element);

  virtual bool touchesBegin(const Touches &touches);
  virtual bool touchesEnd(const Touches &touches);
  virtual bool touchesMove(const Touches &touches);
  
  inline void drawPass(const std::string &passName) const;
  
  inline void addPassUniform(const std::string &key, const Uniform &uniform, const std::string &pass);
  inline void removePassUniform(const std::string &key, const std::string &pass);
  inline void clearPassUniforms(const std::string &pass);
  inline void setChanged();
  
protected:
  FrameBuff *_viewFbo;
  mat4 _projMtx;
  mat4 _viewMtx;
  
private:
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


#endif /* defined(__FelixEngineIOS__UIView__) */
