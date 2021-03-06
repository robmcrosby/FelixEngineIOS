//
//  Resources.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/20/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "FelixEngine.h"
#include "Resources.h"

DEFINE_ENTITY_ID(Resources)

using namespace std;


const Uniforms* Shader::ActiveUniforms = NULL;
const Shader*   Shader::ActiveShader = NULL;

const FrameBuff* FrameBuff::ActiveFBO = NULL;
DRAW_TYPE FrameBuff::ActiveDrawType = DRAW_DEPTH;


Resources::Resources(const XMLTag &tag): Entity(tag), _loaded(0) {
   Host *host = Host::GetHost();
   
   for (XMLTag::const_iterator itr = tag.begin(); itr != tag.end(); ++itr) {
      Resource *resource = host->getResource(**itr);
      if (resource) {
         resource->retain();
         _resources.insert(resource);
      }
   }
   
   // clean up any unused resources
   host->cleanUpResources();
}

Resources::~Resources() {
   unloadResources();
   releaseResources();
}

void Resources::load() {
  loadResources();
}

void Resources::unload() {
  unloadResources();
}

void Resources::loadResources() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->load();
   _loaded = true;
}

void Resources::unloadResources() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->unload();
   _loaded = false;
}

void Resources::releaseResources() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->release();
   _loaded = false;
   _resources.clear();
   
   // clean up any unused resources
   Host::GetHost()->cleanUpResources();
}

void Shader::use() const {
  if (this != ActiveShader) {
    ActiveShader = this;
    
    if (ActiveUniforms)
      setUniforms(ActiveUniforms);
  }
}

const Shader* Shader::GetActiveShader() {
  return ActiveShader;
}

const Uniforms* Shader::GetActiveUniforms() {
  return ActiveUniforms;
}

void Shader::SetActiveUniforms(const Uniforms *uniforms) {
  ActiveUniforms = uniforms;
  
  if (ActiveShader && ActiveUniforms)
    ActiveShader->setUniforms(ActiveUniforms);
}

void FrameBuff::use() const {
  ActiveFBO = this;
}

const FrameBuff* FrameBuff::GetActiveFBO() {
  return ActiveFBO;
}

unsigned int Texture::ParseFilters(const std::string &filtStr) {
   unsigned int filters = filtStr.find("near_nearest") != string::npos ? FILT_MIN_NEAR : FILT_MIN_LINE;
   filters |= filtStr.find("mip_nearest") != string::npos ? FILT_MIP_NEAR : filtStr.find("mip") != string::npos ? FILT_MIP_LINE : 0;
   filters |= filtStr.find("far_nearest") != string::npos ? FILT_MAG_NEAR : FILT_MAG_LINE;
   return filters;
}


MeshData* Mesh::GetPlaneData() {
   MeshData *data = new MeshData();
   float vertices[] {
      0.5f, -0.5f, 0,   1.0f, 0.0f,   0, 0, 1.0f,
      0.5f,  0.5f, 0,   1.0f, 1.0f,   0, 0, 1.0f,
      -0.5f, -0.5f, 0,   0.0f, 0.0f,   0, 0, 1.0f,
      -0.5f,  0.5f, 0,   0.0f, 1.0f,   0, 0, 1.0f
   };
   
   data->vertices.insert(data->vertices.begin(), vertices, vertices+32);
   data->primType = PRIM_TRI_STRIP;
   data->attributes.addAttribute("Pos", 3, 0);
   data->attributes.addAttribute("Tex", 2, 3);
   data->attributes.addAttribute("Norm", 3, 5);
   
   return data;
};
