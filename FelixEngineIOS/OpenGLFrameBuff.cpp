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


OpenGLFrameBuff::OpenGLFrameBuff(const std::string &name): FrameBuff(XMLTag(FBO_TAG)),
_flags(0), _fboId(0), _colorId(0), _depthId(0), _colorTex(0) {
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
      
      // filters are shared for now
      data.filters = Texture::ParseFilters(_tag.getAttribute("filters"));
      
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
      deleteFbo();
}

void OpenGLFrameBuff::setToData(const FrameBuffData &data) {
   loadData(data);
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
   _filters = data.filters;
   
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
   GLint curFbo;
   
   glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFbo);
   
   glGenFramebuffers(1, &_fboId);
   glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
   
   if (_colorTex) {
      TextureData data;
      data.format = TEX_RGBA;
      data.filters = _filters;
      data.size = buffSize();
      _colorTex->setToData(data);
   }
   else if (_flags & FBO_COLOR_COMP)
      createColorRenderBuff();
   
   if (_depthTex) {
      TextureData data;
      data.format = TEX_DEPTH;
      data.filters = _filters;
      data.size = buffSize();
      _depthTex->setToData(data);
   }
   else if (_flags & FBO_DEPTH_COMP)
      createDepthRenderBuff();
   
   // FBO status check
   GLenum status;
   status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   switch(status) {
      case GL_FRAMEBUFFER_COMPLETE:
         cout << "loaded fbo: " << getName() << endl;
         break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
         cout << "Unsupported fbo: " << getName() << endl;
         break;
      default:
         cout << "Framebuffer Error\n";
         break;
   }
   
   glBindFramebuffer(GL_FRAMEBUFFER, curFbo);
   
   _loaded = true;
}

void OpenGLFrameBuff::createColorRenderBuff() {
   ivec2 size = buffSize();
   glGenRenderbuffers(1, &_colorId);
   
   glBindRenderbuffer(GL_RENDERBUFFER, _colorId);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, size.x, size.y);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _colorId);
}

void OpenGLFrameBuff::createDepthRenderBuff() {
   ivec2 size = buffSize();
   glGenRenderbuffers(1, &_depthId);
   
   glBindRenderbuffer(GL_RENDERBUFFER, _depthId);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, size.x, size.y);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthId);
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