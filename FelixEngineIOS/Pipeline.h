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

#define PIPELINE_TAG      "Pipeline"
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
  Pipeline(const XMLTag &tag);
  virtual ~Pipeline();

  virtual void setName(const std::string &name);

  void run(const View &view) const;
  void addPipeItem(PipeItem *item);
  void clearPipeLine();

  static Pipeline* GetPipeline(const std::string &name);
  static Pipeline* GetDefaultPipeline();

private:
  Pipeline();
  inline void applyTag(const XMLTag &tag);

  std::list<PipeItem*> _pipeline;
  static std::map<std::string, Pipeline*> PipelineMap;

  DECLARE_ENTITY_ID(Pipeline)
};


#endif /* defined(__FelixEngineIOS__Pipeline__) */
