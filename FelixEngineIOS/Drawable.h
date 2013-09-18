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

#define APP_CONFIG "Config/main.xml" /**< Main configuration file for the engine to load. */
#define MAIN_PASS "main" /**< Default general pass name. */
#define SCREEN_PASS "screen" /**< Default pass name for drawing 2d elements. (hud, text, etc...) */
#define FINAL_FBO "final" /**< Name for the final system Frame Buffer. */

#define DEF_NEAR -10 /**< Default near value. */
#define DEF_FAR 10 /**< Default far value */


/**
 * Defines a drawable item in the engine.
 */
class Drawable: public Entity {
public:
  Drawable(XMLTag *tag = NULL);
  virtual ~Drawable();
  
  inline bool isVisible() const;
  inline DRAW_TYPE getDrawType() const;
  inline const std::string& getPassName() const;
  inline int getLayer() const;
  
  inline void setVisiblity(bool vis);
  inline void setDrawType(DRAW_TYPE type);
  inline void setPassName(const std::string &pass);
  inline void setLayer(int layer);
  
  virtual void draw() const;
  
  virtual void notify(const Event &event);
  
  inline bool operator<(const Drawable &rhs) const;
  
protected:
  //HostDisplay *_display; // remove when views implemented.
  
private:
  inline void applyTag();
  inline int  getNewId();
  
  bool        _visible;
  DRAW_TYPE   _drawType;
  std::string _passName;
  int         _layer;
  int         _drawId;
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
 * Sets the visibilty.
 * @param vis A boolean value, true for visible and false for not visible.
 */
void Drawable::setVisiblity(bool vis) {
  _visible = vis;
}

/**
 * Sets how this is drawn.
 * @param type Either a blend or depth DRAW_TYPE.
 */
void Drawable::setDrawType(DRAW_TYPE type) {
  _drawType = type;
}

/**
 * Sets the pass that this is associated with.
 * @param pass A string value for the associated pass.
 */
void Drawable::setPassName(const std::string &pass) {
  _passName = pass;
}

/**
 * Sets the layer value.
 * @param layer An int value for the numbered layer.
 */
void Drawable::setLayer(int layer) {
  _layer = layer;
}


/**
 * Used to sort Drawables in the order to be drawn.
 *
 */
bool Drawable::operator<(const Drawable &rhs) const {
  if (_layer != rhs._layer)
    return (_layer < rhs._layer);
  return (_drawId < rhs._drawId);
}

#endif /* defined(__FelixEngineIOS__Drawable__) */
