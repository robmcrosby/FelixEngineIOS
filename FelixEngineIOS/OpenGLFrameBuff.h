//
//  OpenGLFrameBuff.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 7/2/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLFrameBuff__
#define __FelixEngineIOS__OpenGLFrameBuff__

#include "FelixEngine.h"
#include "OpenGLIncludes.h"
#include "OpenGLTexture.h"

class OpenGLFrameBuff: public FrameBuff {
private:
   OpenGLFrameBuff(const std::string &name);
   virtual ~OpenGLFrameBuff();
   
public:
   static OpenGLFrameBuff* GetFrameBuff(const std::string &name);
   static void CleanUpFrameBuffs();
   static void ClearFrameBuffs();
   static void UpdateFrameBuffs();
   
   virtual void load();
   virtual void unload();
   
   virtual void setToData(const FrameBuffData &data);
   virtual ivec2 getSize() const;
   virtual void use() const;
   
   virtual void updateFinal();
   
private:
   inline void loadFinal();
   inline void loadFbo();
   
   inline void createColorRenderBuff();
   inline void createColorTexureBuff();
   inline void createDepthRenderBuff();
   inline void createDepthTextureBuff();
   
   inline void deleteFbo();
   inline void loadData(const FrameBuffData &data);
   inline void updateSize(const ivec2 &screen);
   inline ivec2 buffSize() const;
   
   HostDisplay *_display;
   ivec2 _screen;
   vec2 _size;
   unsigned int _flags;
   GLuint _fboId, _colorId, _depthId, _depthBuff;
   
   OpenGLTexture *_colorTex;
   OpenGLTexture *_depthTex;
   
   static std::map<std::string, OpenGLFrameBuff*> FrameBuffs;
};

#endif /* defined(__FelixEngineIOS__OpenGLFrameBuff__) */
