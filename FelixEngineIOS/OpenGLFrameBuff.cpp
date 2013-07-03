//
//  OpenGLFrameBuff.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 7/2/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLFrameBuff.h"

using namespace std;

OpenGLFrameBuff::OpenGLFrameBuff(const std::string &name): FrameBuff(XMLTag("FrameBuff")),
_flags(0), _fboId(0), _colorId(0), _depthId(0) {
   _tag.setAttribute(ATT_NAME, name);
   _display = Host::GetHost()->getDisplay();
   FrameBuffs[name] = this;
}

OpenGLFrameBuff::~OpenGLFrameBuff() {
   
}

OpenGLFrameBuff* OpenGLFrameBuff::GetFrameBuff(const std::string &name) {
   if (FrameBuffs.find(name) == FrameBuffs.end())
      return new OpenGLFrameBuff(name);
   return FrameBuffs[name];
}

void OpenGLFrameBuff::CleanUpFrameBuffs() {
   map<string, OpenGLFrameBuff*> fbos = FrameBuffs;
   map<string, OpenGLFrameBuff*>::iterator itr;
   
   for (itr = fbos.begin(); itr != fbos.end(); ++itr) {
      if (!itr->second->getCount())
         delete itr->second;
   }
}

void OpenGLFrameBuff::ClearFrameBuffs() {
   map<string, OpenGLFrameBuff*> fbos = FrameBuffs;
   map<string, OpenGLFrameBuff*>::iterator itr;
   
   for (itr = fbos.begin(); itr != fbos.end(); ++itr)
      delete itr->second;
}

void OpenGLFrameBuff::load() {
   
}

void OpenGLFrameBuff::unload() {
   
}

void OpenGLFrameBuff::setToData(const FrameBuffData &data) {
   
}

void OpenGLFrameBuff::use() const {
   
}

void OpenGLFrameBuff::loadData(const MeshData &data) {
   
}