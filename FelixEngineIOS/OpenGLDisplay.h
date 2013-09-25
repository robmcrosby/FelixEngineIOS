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

class OpenGLFrameBuff;

class OpenGLDisplay: public HostDisplay {
public:
  OpenGLDisplay(Host *host);
  virtual ~OpenGLDisplay();
   
  virtual Resource* getResource(const XMLTag &tag);
  virtual void updateResources();
  virtual const Shader* getShader(const std::string &name);
  virtual const Texture* getTexture(const std::string &name);
  virtual const Mesh* getMesh(const std::string &name);
  virtual const FrameBuff* getFrameBuff(const std::string &name);
  virtual void updateFrameBuffs();
  virtual void setResourceData(const ResourceData *data);
   
private:
  Host *_host;
  DRAW_TYPE _curDrawType;
  OpenGLFrameBuff *_finalBuff;
};


#endif /* defined(__FelixEngineIOS__OpenGLDisplay__) */
