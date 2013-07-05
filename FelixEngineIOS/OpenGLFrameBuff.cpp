//
//  OpenGLFrameBuff.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 7/2/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLFrameBuff.h"

using namespace std;

map<string, OpenGLFrameBuff*> OpenGLFrameBuff::FrameBuffs;


OpenGLFrameBuff::OpenGLFrameBuff(const std::string &name): FrameBuff(XMLTag("FrameBuff")),
_flags(0), _fboId(0), _colorId(0), _depthId(0), _colorTex(0) {
   _tag.setAttribute(ATT_NAME, name);
   _display = Host::GetHost()->getDisplay();
   FrameBuffs[name] = this;
   
   if (name == FINAL_FBO)
      loadFinal();
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

void OpenGLFrameBuff::UpdateFrameBuffs() {
   map<string, OpenGLFrameBuff*>::iterator itr;
   ivec2 screen = Host::GetHost()->getScreenSize() * Host::GetHost()->getScreenScale();
   
   for (itr = FrameBuffs.begin(); itr != FrameBuffs.end(); ++itr)
      itr->second->updateSize(screen);
}

void OpenGLFrameBuff::load() {
   if (!loaded()) {
      FrameBuffData data;
      data.size = _tag.hasAttribute("size") ? vec2::ParseFloat(_tag.getAttribute("size")) : vec2(1);
      data.flags = FBO_FIXED_SIZE | FBO_COLOR_COMP | FBO_DEPTH_COMP | FBO_COLOR_TEX;
      data.colorTex = _tag.getAttribute("color");
      
      loadData(data);
   }
}

void OpenGLFrameBuff::unload() {
   if (loaded())
      unload();
}

void OpenGLFrameBuff::setToData(const FrameBuffData &data) {
   
}

void OpenGLFrameBuff::use() const {
   if (loaded()) {
      ivec2 size = buffSize();
      
      glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
      glViewport(0, 0, size.x, size.y);
   }
}

void OpenGLFrameBuff::updateFinal() {
   GLint tmp;
   glGetIntegerv(GL_FRAMEBUFFER_BINDING, &tmp);
   _fboId = tmp;
}

void OpenGLFrameBuff::loadFinal() {
   _screen = Host::GetHost()->getScreenSize() * Host::GetHost()->getScreenScale();
   _size = 1;
   _flags = FBO_FINAL;
   _loaded = true;
}

void OpenGLFrameBuff::loadData(const FrameBuffData &data) {
   _screen = Host::GetHost()->getScreenSize() * Host::GetHost()->getScreenScale();
   _size = data.size;
   _flags = data.flags;
   
   if (_flags & FBO_COLOR_TEX) {
      _colorTex = OpenGLTexture::GetTexture(data.colorTex);
      _colorTex->retain();
   }
   
   loadFbo();
}

void OpenGLFrameBuff::updateSize(const ivec2 &screen) {
   if (!(screen == _screen)) {
      _screen = screen;
      if (loaded() && !(_flags & FBO_FIXED_SIZE) && !(_flags & FBO_FINAL)) {
         unloadFbo();
         loadFbo();
      }
   }
}

ivec2 OpenGLFrameBuff::getSize() const {
   if (_flags & FBO_FINAL)
      return Host::GetHost()->getScreenSize();
   return buffSize();
}

ivec2 OpenGLFrameBuff::buffSize() const {
   return _flags & FBO_FIXED_SIZE ? ivec2(_size) : ivec2(_screen*_size);
}

void OpenGLFrameBuff::loadFbo() {
   ivec2 size = buffSize();
   GLint tmp;
   
   glGetIntegerv(GL_FRAMEBUFFER_BINDING, &tmp);
   
   glGenFramebuffers(1, &_fboId);
   glGenTextures(1, &_colorId);
   glGenRenderbuffers(1, &_depthId);
   
   glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
   
   glBindTexture(GL_TEXTURE_2D, _colorId);
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorId, 0);
   
   glBindRenderbuffer(GL_RENDERBUFFER, _depthId);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, size.x, size.y);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthId);
   
   // FBO status check
   GLenum status;
   status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   switch(status) {
      case GL_FRAMEBUFFER_COMPLETE:
         cout << "fbo complete\n";
         break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
         cout << "fbo unsupported\n";
         break;
      default:
         cout << "Framebuffer Error\n";
         break;
   }
   
   if (_colorTex)
      _colorTex->setToId(_colorId);
   
   glBindFramebuffer(GL_FRAMEBUFFER, tmp);
   
   _loaded = true;
   cout << "loaded fbo: " << _tag.getAttribute("name") << endl;
}

void OpenGLFrameBuff::unloadFbo() {
   glDeleteFramebuffers(1, &_fboId);
   glDeleteTextures(1, &_colorId);
   glDeleteRenderbuffers(1, &_depthId);
   
   _loaded = false;
}