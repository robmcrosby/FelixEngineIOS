//
//  Drawable.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/12/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Drawable__
#define __FelixEngineIOS__Drawable__

#include "Entity.h"
#include "Resources.h"
#include "Transform.h"

#define APP_CONFIG "Config/main.xml" /**< Main configuration file for the engine to load. */
#define MAIN_PASS "mainPass" /**< Default general pass name. */
#define UI_PASS "uiPass" /**< Default pass name for drawing 2d elements. (hud, text, etc...) */
#define FINAL_FBO "final" /**< Name for the final system Frame Buffer. */

#define DEF_NEAR -10 /**< Default near value. */
#define DEF_FAR 10 /**< Default far value */


class HostDisplay;

/**
 * Defines a drawable item in the engine.
 */
class Drawable: public Entity {
public:
  Drawable(const XMLTag &tag);
  virtual ~Drawable();
  
  inline bool isVisible() const;
  inline DRAW_TYPE getDrawType() const;
  inline const std::string& getPassName() const;
  inline int getLayer() const;
  inline View* getCurView();
  
  void setVisiblity(bool vis);
  void setDrawType(DRAW_TYPE type);
  void setPassName(const std::string &pass);
  void setLayer(int layer);

  virtual void load();
  virtual void unload();

  virtual void draw() const;
  
  inline bool operator<(const Drawable &rhs) const;
  
protected:
  HostDisplay  *_hostDisplay;

  mat3 _textureMtx;
  Transform _transform;
  Uniforms _uniforms;
  
private:
  inline void applyTag(const XMLTag &tag);
  inline int  getNewId();
  inline int  getLayer();
  
  bool        _visible;
  DRAW_TYPE   _drawType;
  std::string _passName;
  int         _layer;
  int         _drawId;
  View        *_curView;
};



/* Drawable Inline Methods */

/**
 * Determines if this is visible.
 * @return true if visible or false if not.
 */
bool Drawable::isVisible() const {
  return _visible;
}

/**
 * Determines how to draw this.
 * @return Either blend or depth DRAW_TYPE.
 */
DRAW_TYPE Drawable::getDrawType() const {
  return _drawType;
}

/**
 * Determines what pass this belongs to.
 * @return string name of the pass.
 */
const std::string& Drawable::getPassName() const {
  return _passName;
}

/**
 * Determines the layer which is used to sort Drawables for drawing.
 * @return int layer.
 */
int Drawable::getLayer() const {
  return _layer;
}

/**
 * Gets the current view.
 */
View* Drawable::getCurView() {
  return _curView;
}


/**
 * Used to sort Drawables in the order to be drawn.
 *
 */
bool Drawable::operator<(const Drawable &rhs) const {
  if (_layer != rhs._layer)
    return (rhs._layer < _layer);
  return (rhs._drawId < _drawId);
}

#endif /* defined(__FelixEngineIOS__Drawable__) */
