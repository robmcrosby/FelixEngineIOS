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

OpenGLTexture::OpenGLTexture(const string &name): Texture(XMLTag("Texture")), _texture(0) {
   _tag.setAttribute(ATT_NAME, name);
   Textures[name] = this;
   _filesys = Host::GetHost()->getFileSystem();
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

void OpenGLTexture::ClearTextures() {
   map<string, OpenGLTexture*>::iterator itr;
   for (itr = Textures.begin(); itr != Textures.end(); ++itr)
      delete itr->second;
   Textures.clear();
}

void OpenGLTexture::load() {
   Texture::load();
   
   if (!loaded() && _tag.hasAttribute("src")) {
      string texPath = "Textures/" + _tag.getAttribute("src");
      TextureData *data = _filesys->loadTexture(texPath);
      
      if (data) {
         loadData(*data);
         delete data;
      }
   }
}

void OpenGLTexture::unload() {
   Texture::unload();
   
   if (!getCount())
      deleteTexture();
}

void OpenGLTexture::setToData(const TextureData &data) {
   // set the src file for reloading
   if (data.file != "")
      _tag.setAttribute("src", data.file);
   
   // load the data
   loadData(data);
}

void OpenGLTexture::use(int idx) const {
   if (_texture) {
      glActiveTexture(GL_TEXTURE0 + idx);
      glBindTexture(GL_TEXTURE_2D, _texture);
   }
}

void OpenGLTexture::loadData(const TextureData &data) {
   glGenTextures(1, &_texture);
   glBindTexture(GL_TEXTURE_2D, _texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   GLint format = data.type == TEX_RGBA ? GL_RGBA : GL_RGB; //update for other types
   glTexImage2D(GL_TEXTURE_2D, 0, format, data.size.x, data.size.y, 0, format, GL_UNSIGNED_BYTE, data.data);
   glGenerateMipmap(GL_TEXTURE_2D);
   
   cout << "loaded texture: " << _tag.getAttribute(ATT_NAME) << endl;
   _loaded = true;
}

void OpenGLTexture::deleteTexture() {
   if (!loaded()) {
      glDeleteBuffers(1, &_texture);
      _texture = 0;
      _loaded = false;
      cout << "unloaded texture: " << _tag.getAttribute(ATT_NAME) << endl;
   }
}