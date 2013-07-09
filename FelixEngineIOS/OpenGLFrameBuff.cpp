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
_flags(0), _fboId(0), _colorId(0), _depthId(0), _colorTex(0), _depthTex(0) {
   _tag.setAttribute(ATT_NAME, name);
   _display = Host::GetHost()->getDisplay();
   FrameBuffs[name] = this;
   
   if (name == FINAL_FBO)
      loadFinal();
}

OpenGLFrameBuff::~OpenGLFrameBuff() {
   unload();
   if (_colorTex)
      _colorTex->release();
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
      
      // flags
      if (_tag.hasAttribute("flags")) {
         string flagStr = _tag.getAttribute("flags");
         data.flags = flagStr.find("fixed_size") != string::npos ? FBO_FIXED_SIZE : 0;
         data.flags |= flagStr.find("color_comp") != string::npos ? FBO_COLOR_COMP : 0;
         data.flags |= flagStr.find("depth_comp") != string::npos ? FBO_DEPTH_COMP : 0;
      }
      else {
         data.flags = FBO_COLOR_COMP | FBO_DEPTH_COMP | FBO_COLOR_TEX;
         data.colorTex = getName();
      }
      
      // buffer size
      data.size = _tag.hasAttribute("size") ? vec2::ParseFloat(_tag.getAttribute("size")) : vec2(1);
      
      // color texture
      if (_tag.hasAttribute("color")) {
         data.colorTex = _tag.getAttribute("color");
         data.flags |= FBO_COLOR_COMP | FBO_COLOR_TEX;
      }
      
      // depth texture
      if (_tag.hasAttribute("depth")) {
         data.depthTex = _tag.getAttribute("depth");
         data.flags |= FBO_DEPTH_COMP | FBO_DEPTH_TEX;
      }
      
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
   retain();
}

void OpenGLFrameBuff::loadData(const FrameBuffData &data) {
   _screen = Host::GetHost()->getScreenSize() * Host::GetHost()->getScreenScale();
   _size = data.size;
   _flags = data.flags;
   
   if (_flags & FBO_COLOR_TEX) {
      _colorTex = OpenGLTexture::GetTexture(data.colorTex);
      _colorTex->retain();
   }
   if (_flags & FBO_DEPTH_TEX) {
      _depthTex = OpenGLTexture::GetTexture(data.depthTex);
      _depthTex->retain();
   }
   
   loadFbo();
} 

void OpenGLFrameBuff::updateSize(const ivec2 &screen) {
   if (!(screen == _screen)) {
      _screen = screen;
      if (loaded() && !(_flags & FBO_FIXED_SIZE) && !(_flags & FBO_FINAL)) {
         deleteFbo();
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
   GLint tmp;
   
   glGetIntegerv(GL_FRAMEBUFFER_BINDING, &tmp);
   
   glGenFramebuffers(1, &_fboId);
   glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
   
   // create color comp/texture.
   if (_flags & FBO_COLOR_COMP) {
      if (_colorTex) {
         createColorTexureBuff();
         _colorTex->setToId(_colorId);
      }
      else
         createColorRenderBuff();
   }
   
   // create depth comp/texture.
   if (_flags & FBO_DEPTH_COMP) {
      if (_depthTex) {
         createDepthTextureBuff();
         _depthTex->setToId(_depthId);
      }
      else
         createDepthRenderBuff();
   }
   
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
   
   glBindFramebuffer(GL_FRAMEBUFFER, tmp);
   
   _loaded = true;
   cout << "loaded fbo: " << _tag.getAttribute("name") << endl;
}

void OpenGLFrameBuff::createColorRenderBuff() {
   ivec2 size = buffSize();
   glGenRenderbuffers(1, &_colorId);
   
   glBindRenderbuffer(GL_RENDERBUFFER, _colorId);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, size.x, size.y);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _colorId);
}

void OpenGLFrameBuff::createColorTexureBuff() {
   ivec2 size = buffSize();
   
   cout << size << endl;
   
   glGenTextures(1, &_colorId);
   glBindTexture(GL_TEXTURE_2D, _colorId);
   
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   if (!size.isPowerOfTwo()) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   }
   
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorId, 0);
}

void OpenGLFrameBuff::createDepthRenderBuff() {
   ivec2 size = buffSize();
   glGenRenderbuffers(1, &_depthId);
   
   glBindRenderbuffer(GL_RENDERBUFFER, _depthId);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, size.x, size.y);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthId);
}

void OpenGLFrameBuff::createDepthTextureBuff() {
   ivec2 size = buffSize();
   
   glGenRenderbuffers(1, &_depthBuff);
   glBindRenderbuffer(GL_RENDERBUFFER, _depthBuff);
   
   glGenTextures(1, &_depthId);
   glBindTexture(GL_TEXTURE_2D, _depthId);
   
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   if (!size.isPowerOfTwo()) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   }
   
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthId, 0);
}

void OpenGLFrameBuff::deleteFbo() {
   if (loaded()) {
      // delete the color buffer
      if (_colorTex)
         _colorTex->unload();
      else if (_colorId)
         glDeleteRenderbuffers(1, &_colorId);
      _colorId = 0;
      
      // delete the depth buffer
      if (_depthTex)
         _depthTex->unload();
      else
         glDeleteRenderbuffers(1, &_depthId);
      _depthId = 0;
      
      // delte the frame buffer
      glDeleteFramebuffers(1, &_fboId);
      _fboId = 0;
      
      _loaded = false;
   }
}