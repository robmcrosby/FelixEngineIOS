//
//  OpenGLTexture.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/21/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLTexture.h"

using namespace::std;

map<string, OpenGLTexture*> OpenGLTexture::Textures;

OpenGLTexture::OpenGLTexture(const string &name): Texture(XMLTag("Texture")) {
   _tag.setAttribute("name", name);
   Textures[name] = this;
   cout << "create texture" << endl;
}

OpenGLTexture::~OpenGLTexture() {
   unload();
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
   cout << "load texture" << endl;
}

void OpenGLTexture::unload() {
   cout << "unload texture" << endl;
}

void OpenGLTexture::use(int idx) const {
   
}