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

OpenGLDisplay::OpenGLDisplay(Host *host):
_host(host), _curDrawType(DRAW_DEPTH), _curShader(0), _curPass(0) {
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



void OpenGLDisplay::setCurShader(const Shader *sh) {
   if (sh != _curShader) {
      _curShader = sh;
      if (_curShader)
         _curShader->setUniforms(*getPassUniforms(_curPass));
   }
}

void OpenGLDisplay::setCurUniforms(const Uniforms &uniforms) {
   if (_curShader)
      _curShader->setUniforms(uniforms);
}

void OpenGLDisplay::setCurAttributes(const Attributes &attributes) {
   if (_curShader)
      _curShader->setAttributes(attributes);
}



void OpenGLDisplay::addPassUniform(const string &name, const Uniform &uniform, int pass) {
   Uniforms *passUnifs = getPassUniforms(pass);
   passUnifs->addUniform(name, uniform);
}

void OpenGLDisplay::clearPassUniforms(int pass) {
   Uniforms *passUnifs = getPassUniforms(pass);
   passUnifs->clear();
}

Uniforms* OpenGLDisplay::getPassUniforms(int pass) {
   if (pass >= _passUniforms.size())
      _passUniforms.resize(pass+1);
   return &_passUniforms.at(pass);
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