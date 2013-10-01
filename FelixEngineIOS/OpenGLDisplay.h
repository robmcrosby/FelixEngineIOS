//
//  OpenGLDisplay.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLDisplay__
#define __FelixEngineIOS__OpenGLDisplay__

#include <map>
#include "FelixEngine.h"
#include "View.h"

class OpenGLFrameBuff;

class OpenGLDisplay: public View, public HostDisplay {
public:
  OpenGLDisplay(ivec2 size, float scale);
  virtual ~OpenGLDisplay();

  virtual void resize(int sizeX, int sizeY, float scale);
  virtual ivec2 getScreenSize() const;
  virtual float getScreenScale() const;

  virtual void render();
   
  virtual Resource* getResource(const XMLTag &tag);
  virtual void updateResources();
  virtual const Shader* getShader(const std::string &name);
  virtual const Texture* getTexture(const std::string &name);
  virtual const Mesh* getMesh(const std::string &name);
  virtual const FrameBuff* getFrameBuff(const std::string &name);
  virtual void updateFrameBuffs();
  virtual void setResourceData(const ResourceData *data);
   
private:
  ivec2 _screenSize; /**< screen size */
  float _screenScale; /**< screen scale */

  DRAW_TYPE _curDrawType;
  OpenGLFrameBuff *_finalFbo;
};


#endif /* defined(__FelixEngineIOS__OpenGLDisplay__) */
