//
//  OpenGLDisplay.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLDisplay.h"

#include "OpenGLIncludes.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLMesh.h"
#include "OpenGLFrameBuff.h"

using namespace std;

OpenGLDisplay::OpenGLDisplay(Host *host):
_host(host), _curDrawType(DRAW_DEPTH), _curShader(0), _curPipeline(0) {
   host->addListener(this);
   
   // set inital OpenGL settings
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_DEPTH_TEST);
   
   // set the default pipeline
   _curPipeline = &_defPipeline;
   _curPipeline->load(this);
   
   // get the final buff
   _finalBuff = OpenGLFrameBuff::GetFrameBuff(FINAL_FBO);
   _finalBuff->retain();
   _curBuff = _finalBuff;
   
   // initalize the default passes
   clearPasses();
}

OpenGLDisplay::~OpenGLDisplay() {
   OpenGLShader::ClearShaders();
   OpenGLTexture::ClearTextures();
   OpenGLMesh::ClearMeshes();
}

void OpenGLDisplay::notify(const Event &event) {
   if (event == EVENT_LOAD) {
      OpenGLMesh *plane = OpenGLMesh::GetMesh("plane");
      if (!plane->loaded()) {
         MeshData *planeData = Mesh::GetPlaneData();
         plane->setToData(*planeData);
         plane->retain();
         delete planeData;
      }
   }
   else if (event == EVENT_RESIZE)
      OpenGLFrameBuff::UpdateFrameBuffs();
   
   //HostDisplay::notify(event);
}

void OpenGLDisplay::render() {
   _finalBuff->updateFinal();
   _curPipeline->exec();
}

void OpenGLDisplay::clearContext(Color color) {
   setDrawType(DRAW_DEPTH);
   glClearColor(color.r, color.g, color.b, color.a);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLDisplay::setDrawType(DRAW_TYPE type) {
   if (_curDrawType != type) {
      if (type == DRAW_BLEND) {
         glDepthMask(GL_FALSE);
         glEnable(GL_BLEND);
      }
      else {
         glDepthMask(GL_TRUE);
         glDisable(GL_BLEND);
      }
      _curDrawType = type;
   }
}

void OpenGLDisplay::drawPass(const std::string &pass) {
   Draws::iterator drawable;
   Draws *draws = getPassDraws(pass);
   
   _curShader = NULL;
   _curPass = pass;
   
   for (drawable = draws->begin(); drawable != draws->end(); ++drawable)
      (*drawable)->draw();
}

void OpenGLDisplay::addToPass(const Drawable *drawable, const std::string &pass) {
   getPassDraws(pass)->insert(drawable);
}

void OpenGLDisplay::emptyPasses() {
   for (Passes::iterator itr = _passes.begin(); itr != _passes.end(); ++itr)
      itr->second.draws.clear();
}

void OpenGLDisplay::clearPasses() {
   _passes.clear();
   _passes[SCREEN_PASS] = Pass();
   _passes[MAIN_PASS] = Pass();
   _curPass = MAIN_PASS;
}

Resource* OpenGLDisplay::getResource(const XMLTag &tag) {
   string name = tag.getAttribute(ATT_NAME);
   Resource *ret = NULL;
   
   if (tag == "Shader")
      ret = OpenGLShader::GetShader(name);
   else if (tag == "Texture")
      ret = OpenGLTexture::GetTexture(name);
   else if (tag == "Mesh")
      ret = OpenGLMesh::GetMesh(name);
   else if (tag == "FrameBuff")
      ret = OpenGLFrameBuff::GetFrameBuff(name);
   
   if (ret)
      ret->setToTag(tag);
   
   return ret;
}

void OpenGLDisplay::cleanUpResources() {
   OpenGLShader::CleanUpShaders();
   OpenGLTexture::CleanUpTextures();
   OpenGLMesh::CleanUpMeshes();
}


const Shader* OpenGLDisplay::getShader(const string &name) {
   return OpenGLShader::GetShader(name);
}

const Texture* OpenGLDisplay::getTexture(const string &name) {
   return OpenGLTexture::GetTexture(name);
}

const Mesh* OpenGLDisplay::getMesh(const string &name) {
   return OpenGLMesh::GetMesh(name);
}

const FrameBuff* OpenGLDisplay::getFrameBuff(const string &name) {
   return OpenGLFrameBuff::GetFrameBuff(name);
}


void OpenGLDisplay::setShaderData(const string &name, const ShaderData &data) {
   OpenGLShader::GetShader(name)->setToData(data);
}

void OpenGLDisplay::setTextureData(const string &name, const TextureData &data) {
   OpenGLTexture::GetTexture(name)->setToData(data);
}

void OpenGLDisplay::setMeshData(const string &name, const MeshData &data) {
   OpenGLMesh::GetMesh(name)->setToData(data);
}

void OpenGLDisplay::setFrameBuffData(const string &name, const FrameBuffData &data) {
   OpenGLFrameBuff::GetFrameBuff(name)->setToData(data);
}


void OpenGLDisplay::setCurShader(const Shader *sh) {
   if (sh != _curShader) {
      _curShader = sh;
      if (_curShader)
         _curShader->setUniforms(getPassUniforms(_curPass));
   }
}

void OpenGLDisplay::setCurUniforms(const Uniforms *uniforms) {
   if (_curShader)
      _curShader->setUniforms(uniforms);
}

void OpenGLDisplay::setCurAttributes(const Attributes *attributes) {
   if (_curShader)
      _curShader->setAttributes(attributes);
}

void OpenGLDisplay::setCurPipeline(Pipeline *pipeline) {
   if (!pipeline)
      pipeline = &_defPipeline;
   
   if (_curPipeline != pipeline) {
      _curPipeline->unload();
      _curPipeline = pipeline;
      _curPipeline->load(this);
   }
}


void OpenGLDisplay::addPassUniform(const string &name, const Uniform &uniform, const std::string &pass) {
   getPassUniforms(pass)->addUniform(name, uniform);
}

void OpenGLDisplay::removePassUniform(const string &name, const std::string &pass) {
   getPassUniforms(pass)->remove(name);
}

void OpenGLDisplay::clearPassUniforms(const std::string &pass) {
   getPassUniforms(pass)->clear();
}

OpenGLDisplay::Pass* OpenGLDisplay::getPass(const std::string &pass) {
   if (_passes.find(pass) == _passes.end())
      _passes[pass] = Pass();
   return &_passes[pass];
}

OpenGLDisplay::Draws* OpenGLDisplay::getPassDraws(const std::string &pass) {
   return &getPass(pass)->draws;
}

Uniforms* OpenGLDisplay::getPassUniforms(const std::string &pass) {
   return &getPass(pass)->uniforms;
}