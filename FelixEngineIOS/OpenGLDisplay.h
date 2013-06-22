//
//  OpenGLDisplay.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLDisplay__
#define __FelixEngineIOS__OpenGLDisplay__

#include <set>
#include "FelixEngine.h"

struct DrawCmp {
   inline bool operator() (const Drawable *lhs, const Drawable *rhs) const {
      if(lhs->getLayer() != rhs->getLayer())
         return lhs->getLayer() < rhs->getLayer();
      return lhs < rhs;
   }
};

class OpenGLDisplay: public HostDisplay {
public:
   OpenGLDisplay(Host *host);
   virtual ~OpenGLDisplay();
   
   virtual void drawPasses();
   virtual void addPassUniform(const Uniform &uniform, int pass);
   virtual void clearPassUniforms(int pass);
   
   virtual Resource* getResource(const XMLTag &tag);
   virtual const Shader* getShader(const std::string &name);
   virtual const Texture* getTexture(const std::string &name);
   virtual const Mesh* getMesh(const std::string &name);
private:
   inline void setDrawType(DRAW_TYPE type);
   
   Host *_host;
   DRAW_TYPE _curDrawType;
   
   std::vector<std::set<const Drawable*, DrawCmp> > _passes;
};


#endif /* defined(__FelixEngineIOS__OpenGLDisplay__) */
