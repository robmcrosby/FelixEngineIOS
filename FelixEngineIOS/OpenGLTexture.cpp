//
//  OpenGLTexture.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/21/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLTexture.h"
#include "ResourceData.h"

using namespace::std;

map<string, OpenGLTexture*> OpenGLTexture::Textures;

OpenGLTexture::OpenGLTexture(const string &name): Texture(XMLTag(TEXTURE_TAG)),
_texId(0), _format(TEX_RGBA), _filters(0), _fboTex(0) {
   _tag.setAttribute(ATT_NAME, name);
   _filesys = Host::GetHost()->getFileSystem();
   Textures[name] = this;
}

OpenGLTexture::~OpenGLTexture() {
   Textures.erase(_tag.getAttribute(ATT_NAME));
   deleteTexture();
}

OpenGLTexture* OpenGLTexture::GetTexture(const string &name) {
   if (Textures.find(name) == Textures.end())
      return new OpenGLTexture(name);
   return Textures[name];
}

void OpenGLTexture::CleanUpTextures() {
   map<string, OpenGLTexture*> textures = Textures;
   map<string, OpenGLTexture*>::iterator itr;
   
   for (itr = textures.begin(); itr != textures.end(); ++itr) {
      if (!itr->second->getCount())
         delete itr->second;
   }
}

void OpenGLTexture::ClearTextures() {
   map<string, OpenGLTexture*>::iterator itr;
   for (itr = Textures.begin(); itr != Textures.end(); ++itr)
      delete itr->second;
   Textures.clear();
}

void OpenGLTexture::SetData(const ResourceData *data) {
   const TextureData *tData = dynamic_cast<const TextureData*>(data);
   if (tData)
      GetTexture(tData->targetName)->setToData(*tData);
}

void OpenGLTexture::load() {
   if (!loaded() && _tag.hasAttribute("src")) {
      string texPath = "Textures/" + _tag.getAttribute("src");
      TextureData *data = _filesys->loadTexture(texPath);
      data->filters = Texture::ParseFilters(_tag.getAttribute("filters"));
      
      if (data) {
         loadData(*data);
         delete data;
      }
   }
}

void OpenGLTexture::unload() {
   deleteTexture();
}

void OpenGLTexture::setToData(const TextureData &data) {
   _fboTex = !data.data;
   
   if (!loaded() || _fboTex) {
      // set the src file for reloading
      if (data.file != "")
         _tag.setAttribute("src", data.file);
      
      // load the data
      loadData(data);
   }
}

void OpenGLTexture::use(int idx) const {
   if (_texId) {
      glActiveTexture(GL_TEXTURE0 + idx);
      glBindTexture(GL_TEXTURE_2D, _texId);
   }
}

void OpenGLTexture::setToId(GLuint tex) {
   _texId = tex;
   _loaded = true;
}

void OpenGLTexture::loadData(const TextureData &data) {
   _filters = data.filters;
   _format = data.format;
   _size = data.size;
   
   // enforce no mip maps for fbo textures.
   if (_fboTex)
      _filters &= ~(FILT_MIP_NEAR | FILT_MIP_LINE);
   
   createTexture(data.data);
}

void OpenGLTexture::createTexture(void *data) {
   GLint format = _format == TEX_DEPTH ? GL_DEPTH_COMPONENT : _format == TEX_RGBA ? GL_RGBA : GL_RGB;
   GLenum type = _format == TEX_DEPTH ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;
   
   // delete the previous texture.
   if (_texId)
      deleteTexture();
   
   // generate and bind the texture id.
   glGenTextures(1, &_texId);
   glBindTexture(GL_TEXTURE_2D, _texId);
   
   // setup for any non-powers of two textures.
   if (!_size.isPowerOfTwo()) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      _filters &= ~(FILT_MIP_NEAR | FILT_MIP_LINE); // remove any mip map filters
   }
   
   // set texture filters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMin(_filters));
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getMag(_filters));
   
   // create the texture
   glTexImage2D(GL_TEXTURE_2D, 0, format, _size.x, _size.y, 0, format, type, data);
   
   // attach texture to the active frame buffer.
   if (_fboTex) {
      GLenum attachment = _format == TEX_DEPTH ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0;
      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _texId, 0);
   }
   
   // generate mip maps
   if (_filters & (FILT_MIP_NEAR | FILT_MIP_LINE))
      glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   
   _loaded = true;
   cout << "loaded texture: " << getName() << endl;
}

void OpenGLTexture::deleteTexture() {
   if (_texId) {
      if (!_fboTex)
         glDeleteBuffers(1, &_texId);
      _texId = 0;
      _loaded = false;
      cout << "unloaded texture: " << _tag.getAttribute(ATT_NAME) << endl;
   }
}

GLint OpenGLTexture::getMin(unsigned int filters) {
   if (filters & FILT_MIP_NEAR)
      return filters & FILT_MIN_NEAR ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_NEAREST;
   if (filters & FILT_MIP_LINE)
      return filters & FILT_MIN_NEAR ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
   return filters & FILT_MIN_NEAR ? GL_NEAREST : GL_LINEAR;
}

GLint OpenGLTexture::getMag(unsigned int filters) {
   return filters & FILT_MAG_NEAR ? GL_NEAREST : GL_LINEAR;
}