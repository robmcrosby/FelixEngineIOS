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
_host(host), _curDrawType(DRAW_DEPTH) {
   host->addListener(this);
   
   // set inital OpenGL settings
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_DEPTH_TEST);
   
   // get the final buff
   _finalBuff = OpenGLFrameBuff::GetFrameBuff(FINAL_FBO);
   _finalBuff->retain();
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
   else if (event == EVENT_RESIZE) {
      OpenGLFrameBuff::UpdateFrameBuffs();
   }
   
   Entity::notify(event);
}

/**
 * Clears the active color and depth buffers.
 * @param color Color to draw in the color buffer, the default is black.
 */
void OpenGLDisplay::clearContext(Color color) {
   setDrawType(DRAW_DEPTH);
   glClearColor(color.r, color.g, color.b, color.a);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Sets the current draw mode or type.
 * @param type Either blend or depth draw type enum.
 */
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

/**
 * Gets a pointer to Resource defined by a tag.
 * @param tag XMLTag that defines a Resource.
 * @return Resource pointer or NULL if it can not be determined from the tag.
 */
Resource* OpenGLDisplay::getResource(const XMLTag &tag) {
   string name = tag.getAttribute(ATT_NAME);
   Resource *ret = NULL;
   
   if (tag == SHADER_TAG)
      ret = OpenGLShader::GetShader(name);
   else if (tag == TEXTURE_TAG)
      ret = OpenGLTexture::GetTexture(name);
   else if (tag == MESH_TAG)
      ret = OpenGLMesh::GetMesh(name);
   else if (tag == FBO_TAG)
      ret = OpenGLFrameBuff::GetFrameBuff(name);
   
   if (ret)
      ret->setToTag(tag);
   
   return ret;
}

/**
 * Cleans up any resources not being currently used.
 */
void OpenGLDisplay::cleanUpResources() {
   OpenGLShader::CleanUpShaders();
   OpenGLTexture::CleanUpTextures();
   OpenGLMesh::CleanUpMeshes();
}

/**
 * Gets a Shader associated by a name, creates a new Shader if not avalible.
 * @param name string name of the Shader.
 * @return Shader pointer.
 */
const Shader* OpenGLDisplay::getShader(const string &name) {
   return OpenGLShader::GetShader(name);
}

/**
 * Gets a Texture associated by a name, creates a new Texture if not avalible.
 * @param name string name of the Texture.
 * @return Texture pointer.
 */
const Texture* OpenGLDisplay::getTexture(const string &name) {
   return OpenGLTexture::GetTexture(name);
}

/**
 * Gets a Mesh associated by a name, creates a new Mesh if not avalible.
 * @param name string name of the Mesh.
 * @return Mesh pointer.
 */
const Mesh* OpenGLDisplay::getMesh(const string &name) {
   return OpenGLMesh::GetMesh(name);
}

/**
 * Gets a FrameBuff associated by a name, creates a new FrameBuff if not avalible.
 * @param name string name of the FrameBuff.
 * @return FrameBuffer pointer.
 */
const FrameBuff* OpenGLDisplay::getFrameBuff(const string &name) {
   return OpenGLFrameBuff::GetFrameBuff(name);
}

/**
 * Sets the ResourceData with the targetName resource.
 * @param data ResourceData to be set to a resource in the display.
 */
void OpenGLDisplay::setResourceData(const ResourceData *data) {
   OpenGLShader::SetData(data);
   OpenGLTexture::SetData(data);
   OpenGLMesh::SetData(data);
   OpenGLFrameBuff::SetData(data);
}


