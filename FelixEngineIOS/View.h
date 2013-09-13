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
 * View Class
 */
class View: Drawable {
public:
  View(XMLTag *tag);
  virtual ~View();
  
  virtual void notify(const Event &event);
  
  inline void drawPass(const std::string &pass);
  inline void addDrawable(const Drawable *drawable);
  inline void useShader(const Shader *sh);
  
  inline void addPassUniform(const std::string &key, const Uniform &uniform, const std::string &pass);
  inline void removePassUniform(const std::string &key, const std::string &pass);
  inline void clearPassUniforms(const std::string &pass);
  
private:
  inline void emptyPasses();
  inline void clearPasses();
  
  inline Pass* getPass(const std::string &pass);
  inline Draws* getPassDraws(const std::string &pass);
  inline Uniforms* getPassUniforms(const std::string &pass);
  
  Passes        _passes;
  std::string   _curPass;
  const Shader  *_curShader;
  
  DECLARE_ENTITY_ID(View)
};



/*
 * View's public inline implementations.
 */

/**
 * Draws all of the Drawables in a pass.
 * @param pass string name of a pass to be drawn.
 */
void View::drawPass(const std::string &pass) {
  Draws::iterator drawable;
  Draws *draws = getPassDraws(pass);
  
  _curShader = NULL;
  _curPass = pass;
  
  for (drawable = draws->begin(); drawable != draws->end(); ++drawable)
    (*drawable)->draw();
}

/**
 * Adds a Drawable to a pass.
 * @param drawable Drawable pointer.
 */
void View::addDrawable(const Drawable *drawable) {
  getPassDraws(drawable->getPassName())->insert(drawable);
}

/**
 * Sets the shader to apply pass attributes.
 * @param sh Pointer to a Shader.
 */
void View::useShader(const Shader *sh) {
  if (sh != _curShader) {
    _curShader = sh;
    if (_curShader)
      _curShader->setUniforms(getPassUniforms(_curPass));
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


#endif /* defined(__FelixEngineIOS__UIView__) */