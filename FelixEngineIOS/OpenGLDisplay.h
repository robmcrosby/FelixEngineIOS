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
   
   virtual void render();
   
   virtual void clearContext(Color color);
   virtual void setDrawType(DRAW_TYPE type);
   
   virtual void drawPass(const std::string &pass);
   virtual void addToPass(const Drawable *drawable, const std::string &pass);
   virtual void emptyPasses();
   virtual void clearPasses();
   
   virtual Resource* getResource(const XMLTag &tag);
   virtual void cleanUpResources();
   virtual const Shader* getShader(const std::string &name);
   virtual const Texture* getTexture(const std::string &name);
   virtual const Mesh* getMesh(const std::string &name);
   virtual const FrameBuff* getFrameBuff(const std::string &name);
   
   virtual void setShaderData(const std::string &name, const ShaderData &data);
   virtual void setTextureData(const std::string &name, const TextureData &data);
   virtual void setMeshData(const std::string &name, const MeshData &data);
   virtual void setFrameBuffData(const std::string &name, const FrameBuffData &data);
   
   virtual void setCurShader(const Shader *sh);
   virtual void setCurUniforms(const Uniforms *uniforms);
   virtual void setCurAttributes(const Attributes *attributes);
   
   virtual void setCurPipeline(Pipeline *pipeline);
   
   virtual void addPassUniform(const std::string &name, const Uniform &uniform, const std::string &pass);
   virtual void removePassUniform(const std::string &name, const std::string &pass);
   virtual void clearPassUniforms(const std::string &pass);
   
   inline vec2 getScreenSize() const {return _host->getScreenSize() * _host->getScreenScale();}
   
private:
   
   /**
    Comparator for Drawables
    */
   struct DrawCmp {
      inline bool operator() (const Drawable *lhs, const Drawable *rhs) const {
         int l = lhs->getLayer(), r = rhs->getLayer();
         return l != r ? l < r : lhs < rhs;
      }
   };
   /**
    Collection of Drawables
   */
   typedef std::set<const Drawable*, DrawCmp> Draws;
   
   /**
    Holds the Drawables and Uniforms for a pass.
    */
   struct Pass {
      Draws draws;
      Uniforms uniforms;
   };
   
   /**
    Collection of passes
    */
   typedef std::map<std::string, Pass> Passes;
   
   inline Pass* getPass(const std::string &pass);
   inline Draws* getPassDraws(const std::string &pass);
   inline Uniforms* getPassUniforms(const std::string &pass);
   
   Host *_host;
   DRAW_TYPE _curDrawType;
   const Shader *_curShader;
   Pipeline _defPipeline;
   Pipeline *_curPipeline;
   std::string _curPass;
   
   FrameBuff *_curBuff;
   OpenGLFrameBuff *_finalBuff;
   
   Passes _passes;
};


#endif /* defined(__FelixEngineIOS__OpenGLDisplay__) */
