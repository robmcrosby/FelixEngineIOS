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
class View: public Drawable {
public:
  View(XMLTag *tag = NULL);
  virtual ~View();
  
  virtual void notify(const Event &event);
  virtual View* getView();
  virtual void draw() const;
  
  void addDrawable(const Drawable *drawable);
  
  inline void drawPass(const std::string &pass) const;
  inline void useShader(const Shader *sh);
  
  inline void addPassUniform(const std::string &key, const Uniform &uniform, const std::string &pass);
  inline void removePassUniform(const std::string &key, const std::string &pass);
  inline void clearPassUniforms(const std::string &pass);
  
protected:
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
void View::drawPass(const std::string &pass) const {
  const Draws *draws = getPassDraws(pass);
  
  if (draws) {
    // TODO: Set the pass uniforms to the shader.
    
    for (Draws::const_iterator drawable = draws->begin(); drawable != draws->end(); ++drawable)
      (*drawable)->draw();
  }
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
