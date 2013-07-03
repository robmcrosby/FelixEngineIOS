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

class OpenGLFrameBuff: public FrameBuff {
private:
   OpenGLFrameBuff(const std::string &name);
   virtual ~OpenGLFrameBuff();
   
public:
   static OpenGLFrameBuff* GetFrameBuff(const std::string &name);
   static void CleanUpFrameBuffs();
   static void ClearFrameBuffs();
   
   virtual void load();
   virtual void unload();
   
   virtual void setToData(const FrameBuffData &data);
   
   virtual void use() const;
   
private:
   inline void loadData(const MeshData &data);
   
   HostDisplay *_display;
   vec2 _size;
   unsigned int _flags;
   GLuint _fboId, _colorId, _depthId;
   
   static std::map<std::string, OpenGLFrameBuff*> FrameBuffs;
};

#endif /* defined(__FelixEngineIOS__OpenGLFrameBuff__) */
