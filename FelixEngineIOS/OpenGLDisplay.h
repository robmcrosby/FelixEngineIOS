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

class OpenGLDisplay: public Entity, public HostDisplay {
public:
   OpenGLDisplay(Host *host);
   virtual ~OpenGLDisplay();
   
   virtual void notify(const Event &event);
   
   virtual void clearContext(Color color);
   virtual void setDrawType(DRAW_TYPE type);
   
   virtual Resource* getResource(const XMLTag &tag);
   virtual void cleanUpResources();
   virtual const Shader* getShader(const std::string &name);
   virtual const Texture* getTexture(const std::string &name);
   virtual const Mesh* getMesh(const std::string &name);
   virtual const FrameBuff* getFrameBuff(const std::string &name);
   
   virtual void setResourceData(const ResourceData *data);
   
   inline vec2 getScreenSize() const {return _host->getScreenSize() * _host->getScreenScale();}
   
private:
   Host *_host;
   DRAW_TYPE _curDrawType;
   
   //mat4 _defProjMtx;
   //mat4 _defViewMtx;
   
   OpenGLFrameBuff *_finalBuff;
};


#endif /* defined(__FelixEngineIOS__OpenGLDisplay__) */
