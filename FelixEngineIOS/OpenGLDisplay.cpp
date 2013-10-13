//
//  OpenGLDisplay.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLDisplay.h"

#include "Drawable.h"
#include "OpenGLIncludes.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLMesh.h"
#include "OpenGLFrameBuff.h"

using namespace std;

OpenGLDisplay::OpenGLDisplay(ivec2 size, float scale): View(XMLTag(GL_DISPLAY_TAG)),
_screenSize(size), _screenScale(scale), _curDrawType(DRAW_DEPTH) {

  // set inital OpenGL settings
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);

  // get the final buff
  _finalFbo = OpenGLFrameBuff::GetFrameBuff(FINAL_FBO);
  _finalFbo->retain();
}

OpenGLDisplay::~OpenGLDisplay() {
  OpenGLShader::ClearShaders();
  OpenGLTexture::ClearTextures();
  OpenGLMesh::ClearMeshes();
}

bool OpenGLDisplay::touchesBegin(const Touches &touches) {
  return View::touchesBegin(touches);
}

bool OpenGLDisplay::touchesEnd(const Touches &touches) {
  return View::touchesEnd(touches);
}

bool OpenGLDisplay::touchesMove(const Touches &touches) {
  return View::touchesMove(touches);
}

void OpenGLDisplay::resize(int sizeX, int sizeY, float scale) {
  _screenSize.x = sizeX;
  _screenSize.y = sizeY;
  _screenScale = scale;

  OpenGLFrameBuff::UpdateFrameBuffs();
  load();
}

/**
 * Gets the current screen size of the Host.
 * @return ivec2
 */
ivec2 OpenGLDisplay::getScreenSize()  const {
  return _screenSize;
}

/**
 * Gets the scale of the screen. (2.0f for Retina and 1.0f for Non-Retina Displays)
 * @return float scale.
 */
float OpenGLDisplay::getScreenScale() const {
  return _screenScale;
}

void OpenGLDisplay::render() {
  _finalFbo->updateFinal();
  draw();
}

void OpenGLDisplay::clearContext(const Color &color) const {
  OpenGLFrameBuff::ClearCurrentFBO(color);
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
void OpenGLDisplay::updateResources() {

  OpenGLMesh *plane = OpenGLMesh::GetMesh("plane");
  if (!plane->loaded()) {
    MeshData *planeData = Mesh::GetPlaneData();
    plane->setToData(*planeData);
    plane->retain();
    delete planeData;
  }

  OpenGLFrameBuff::UpdateFrameBuffs();

  //OpenGLShader::CleanUpShaders();
  //OpenGLTexture::CleanUpTextures();
  //OpenGLMesh::CleanUpMeshes();
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
 * Updates the frame buffers dependent on the screen size.
 */
void OpenGLDisplay::updateFrameBuffs() {
  OpenGLFrameBuff::UpdateFrameBuffs();
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


