//
//  Pipeline.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Pipeline.h"
#include "FelixEngine.h"
#include "Resources.h"
#include "View.h"

using namespace std;


// ######################################################################
//                            PipeLine
// ######################################################################

DEFINE_ENTITY_ID(Pipeline)

map<string, Pipeline*> Pipeline::PipelineMap;

Pipeline::Pipeline(): Entity(XMLTag(PIPELINE_TAG)) {
  addPipeItem(new DrawPass(MAIN_PASS));
  addPipeItem(new DrawPass(UI_PASS));
  setName(DEF_PIPELINE);
}

Pipeline::Pipeline(const XMLTag &tag): Entity(tag) {
  applyTag(tag);
}

Pipeline::~Pipeline() {
  clearPipeLine();
}

void Pipeline::setName(const std::string &name) {
  if (name != getName())
    PipelineMap.erase(getName());
  if (name != "")
    PipelineMap[name] = this;

  Entity::setName(name);
}

void Pipeline::run(const View &view) const {
  list<PipeItem*>::const_iterator itr;
  for (itr = _pipeline.begin(); itr != _pipeline.end(); ++itr)
    (*itr)->run(view);
}

void Pipeline::addPipeItem(PipeItem *item) {
  if (item)
    _pipeline.push_back(item);
}

void Pipeline::clearPipeLine() {
  list<PipeItem*>::iterator itr;
  for (itr = _pipeline.begin(); itr != _pipeline.end(); ++itr)
    delete *itr;
  _pipeline.clear();
}

Pipeline* Pipeline::GetPipeline(const string &name) {
  map<string, Pipeline*>::iterator itr;
  itr = PipelineMap.find(name);
  if (itr != PipelineMap.end())
    return itr->second;

  cerr << "Could not find Pipeline with name: " << name << endl;
  return NULL;
}

Pipeline* Pipeline::GetDefaultPipeline() {
  static Pipeline defaultPipeline;
  return &defaultPipeline;
}


void Pipeline::applyTag(const XMLTag &tag) {
  XMLTag::const_iterator itr;

  // Create the pipe.
  for (itr = tag.begin(); itr != tag.end(); ++itr) {
    if (**itr == CLEAR_CONTEXT_TAG)
      addPipeItem(new ClearContext(**itr));
    else if (**itr == DRAW_PASS_TAG)
      addPipeItem(new DrawPass(**itr));
    else if (**itr == USE_FBO_TAG)
      addPipeItem(new UseFBO(**itr));
    else if (**itr == DRAW_IMAGE_TAG)
      addPipeItem(new DrawImage(**itr));
  }

  // Set the name if avalible.
  if (tag.hasAttribute(ATT_NAME))
    setName(tag.getAttribute(ATT_NAME));
}



// ######################################################################
//                         ClearContext
// ######################################################################

ClearContext::ClearContext(const XMLTag &tag): PipeItem(CLEAR_CONTEXT_TAG) {
  setClearColor(tag.getContents());
}

ClearContext::ClearContext(const Color &color): PipeItem(CLEAR_CONTEXT_TAG) {
  setClearColor(color);
}

void ClearContext::run(const View &view) const {
  view.getDisplay()->clearContext(clearColor);
}

void ClearContext::setClearColor(const Color &color) {
  clearColor = color;
  itemTag.setContents(vec4::ToString(clearColor));
}

void ClearContext::setClearColor(const string &colorStr) {
  setClearColor(vec4::ParseFloat(colorStr));
}


// ######################################################################
//                            DrawPass
// ######################################################################

DrawPass::DrawPass(const XMLTag &tag): PipeItem(DRAW_PASS_TAG) {
  setPass(tag.getContents());
}

DrawPass::DrawPass(const std::string &pass): PipeItem(DRAW_PASS_TAG) {
  setPass(pass);
}

void DrawPass::run(const View &view) const {
  view.drawPass(itemTag.getContents());
}

void DrawPass::setPass(const std::string &pass) {
  itemTag.setContents(pass);
}


// ######################################################################
//                               UseFBO
// ######################################################################

UseFBO::UseFBO(const XMLTag &tag): PipeItem(USE_FBO_TAG) {
  setFBO(tag.getContents());
}

UseFBO::UseFBO(const std::string &fbo): PipeItem(USE_FBO_TAG) {
  setFBO(fbo);
}

void UseFBO::run(const View &view) const {
  view.getDisplay()->getFrameBuff(itemTag.getContents())->use();
}

void UseFBO::setFBO(const std::string &fbo) {
  itemTag.setContents(fbo);
}


// ######################################################################
//                            DrawImage
// ######################################################################

DrawImage::DrawImage(const XMLTag &tag): PipeItem(DRAW_IMAGE_TAG) {
  const XMLTag *subTag;

  // Get the shader name
  if ((subTag = tag.getSubTag(SHADER_TAG)))
    setShader(subTag->getContents());
  else
    setShader(DEF_PIPELINE_SHADER);

  // Get the textures
  if ((subTag = tag.getSubTag(TEXTURES_TAG))) {
    XMLTag::const_iterator itr;
    for (itr = subTag->begin(); itr != subTag->end(); ++itr)
      addTexture((*itr)->getContents());
  }
  else if ((subTag = tag.getSubTag(TEXTURE_TAG)))
    addTexture(subTag->getContents());
}

DrawImage::DrawImage(const string &texture, const string &shader): PipeItem(DRAW_IMAGE_TAG) {
  setShader(shader);
  addTexture(texture);
}

// TODO: when view has const getMesh and getShader methods.
void DrawImage::run(const View &view) const {

}

void DrawImage::setShader(const std::string &shader) {
  itemTag.getSubTag(SHADER_TAG)->setContents(shader);
}

void DrawImage::addTexture(const std::string &texture) {
  XMLTag *textures = itemTag.getSubTag(TEXTURES_TAG);
  textures->addSubTag(new XMLTag(TEXTURE_TAG, texture));
}

void DrawImage::clearTextures() {
  itemTag.getSubTag(TEXTURES_TAG)->clearSubTags();
}


