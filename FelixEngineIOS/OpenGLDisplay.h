//
//  OpenGLDisplay.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLDisplay__
#define __FelixEngineIOS__OpenGLDisplay__

#include "FelixEngine.h"
#include "Drawable.h"

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
   
   virtual Resource* getResource(const XMLTag &tag);
   virtual const Shader* getShader(const std::string &name);
   virtual const Texture* getTexture(const std::string &name);
   virtual const Mesh* getMesh(const std::string &name);
   
   virtual void setCurShader(const Shader *sh);
   virtual void setCurUniforms(const Uniforms &uniforms);
   virtual void setCurAttributes(const Attributes &attributes);
   
   virtual void addPassUniform(const std::string &name, const Uniform &uniform, int pass);
   virtual void clearPassUniforms(int pass);
   
private:
   inline Uniforms* getPassUniforms(int pass);
   inline void setDrawType(DRAW_TYPE type);
   
   Host *_host;
   DRAW_TYPE _curDrawType;
   const Shader *_curShader;
   int _curPass;
   
   std::vector<std::set<const Drawable*, DrawCmp> > _passes;
   std::vector<Uniforms> _passUniforms;
};


#endif /* defined(__FelixEngineIOS__OpenGLDisplay__) */
