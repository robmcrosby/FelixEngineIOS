//
//  Pipeline.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Pipeline__
#define __FelixEngineIOS__Pipeline__


#include "Entity.h"
#include "Color.h"


#define CLEAR_CONTEXT_TAG "ClearContext"
#define DRAW_PASS_TAG     "DrawPass"
#define USE_FBO_TAG       "UseFBO"
#define DRAW_IMAGE_TAG    "DrawImage"

#define DEF_PIPELINE "DefaultPipeline"
#define DEF_PIPELINE_SHADER "PlaneTexture"


class PipeItem {
public:
  PipeItem(const std::string &type): itemTag(type) {}
  virtual ~PipeItem() {}
  virtual void run(const View &view) const = 0;

protected:
  XMLTag itemTag;
};

class ClearContext: public PipeItem {
public:
  ClearContext(const XMLTag &tag);
  ClearContext(const Color &color = Color());
  virtual void run(const View &view) const;
  void setClearColor(const Color &color);
  void setClearColor(const std::string &colorStr);
private:
  Color clearColor;
};

struct DrawPass: public PipeItem {
  DrawPass(const XMLTag &tag);
  DrawPass(const std::string &pass);
  virtual void run(const View &view) const;
  void setPass(const std::string &pass);
};

struct UseFBO: public PipeItem {
  UseFBO(const XMLTag &tag);
  UseFBO(const std::string &fbo);
  virtual void run(const View &view) const;
  void setFBO(const std::string &fbo);
};

struct DrawImage: public PipeItem {
  DrawImage(const XMLTag &tag);
  DrawImage(const std::string &texture, const std::string &shader);
  virtual void run(const View &view) const;
  void setShader(const std::string &shader);
  void addTexture(const std::string &texture);
  void clearTextures();
};



class Pipeline: public Entity {
public:
  Pipeline();
  Pipeline(XMLTag *tag);
  virtual ~Pipeline();

  void run(const View &view) const;
  void addPipeItem(PipeItem *item);
  void clearPipeLine();

private:
  inline void applyTag(const XMLTag &tag);

  std::list<PipeItem*> _pipeline;
  DECLARE_ENTITY_ID(Pipeline)
};

/*
class Pipeline: public Entity {
public:
   Pipeline();
   Pipeline(XMLTag *tag);
   virtual ~Pipeline();
   
   virtual void load(HostDisplay *display);
   virtual void unload();
   virtual void exec();

   class PipeItem {
   public:
      PipeItem(): _display(0) {}
      virtual ~PipeItem() {}
      
      virtual void load(HostDisplay *display) {_display = display;}
      virtual void unload() {_display = NULL;}
      virtual void exec() {}
      
   protected:
      HostDisplay *_display;
   };

   struct ClearContext: public PipeItem {
      ClearContext(const Color &c = Color()): clearColor(c) {}
      virtual void exec();
      
      Color clearColor;
   };
   

   struct DrawPass: public PipeItem {
      DrawPass(const std::string &p): pass(p) {}
      virtual void exec();
      
      std::string pass;
   };
   

   struct UseFBO: public PipeItem {
      UseFBO(const std::string &n): fboName(n), fbo(0) {}
      virtual void load(HostDisplay *display);
      virtual void unload();
      virtual void exec();
      
      std::string fboName;
      const FrameBuff *fbo;
   };
   
   struct DrawFull: public PipeItem {
      DrawFull(const XMLTag &tag);
      virtual void load(HostDisplay *display);
      virtual void unload();
      virtual void exec();
      
      const Mesh *plane;
      const Shader *shader;
      std::list<const Texture*> texList;
      Uniforms uniforms;
      XMLTag tag;
   };
   
private:
   inline void applyTag();
   
   std::list<PipeItem*> _pipeline;
   
   DECLARE_ENTITY_ID(Pipeline)
};*/


#endif /* defined(__FelixEngineIOS__Pipeline__) */
