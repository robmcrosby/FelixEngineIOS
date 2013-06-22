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

using namespace std;

OpenGLDisplay::OpenGLDisplay(Host *host): _host(host), _curDrawType(DRAW_DEPTH) {
   host->addListener(this);
   
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_DEPTH_TEST);
}

OpenGLDisplay::~OpenGLDisplay() {
   OpenGLShader::ClearShaders();
   OpenGLTexture::ClearTextures();
}

void OpenGLDisplay::drawPasses() {
   glClearColor(0, 0, 1, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLDisplay::addPassUniform(const Uniform &uniform, int pass) {
   
}

void OpenGLDisplay::clearPassUniforms(int pass) {
   
}

Resource* OpenGLDisplay::getResource(const XMLTag &tag) {
   string name = tag.getAttribute("name");
   Resource *ret = NULL;
   
   if (tag == "Shader")
      ret = OpenGLShader::GetShader(name);
   else if (tag == "Texture")
      ret = OpenGLTexture::GetTexture(name);
   else if (tag == "Mesh")
      ret = OpenGLMesh::GetMesh(name);
   
   if (ret)
      ret->setToTag(tag);
   
   return ret;
}

const Shader* OpenGLDisplay::getShader(const std::string &name) {
   return OpenGLShader::GetShader(name);
}

const Texture* OpenGLDisplay::getTexture(const std::string &name) {
   return OpenGLTexture::GetTexture(name);
}

const Mesh* OpenGLDisplay::getMesh(const std::string &name) {
   return OpenGLMesh::GetMesh(name);
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