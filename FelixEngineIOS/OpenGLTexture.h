//
//  OpenGLTexture.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/21/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLTexture__
#define __FelixEngineIOS__OpenGLTexture__

#include "FelixEngine.h"
#include "OpenGLIncludes.h"

class OpenGLTexture: public Texture {
private:
   OpenGLTexture(const std::string &name);
   virtual ~OpenGLTexture();
   
public:
   static OpenGLTexture* GetTexture(const std::string &name);
   static void CleanUpTextures();
   static void ClearTextures();
   
   virtual void load();
   virtual void unload();
   
   virtual void setToData(const TextureData &data);
   virtual void use(int idx) const;
   
private:
   inline void loadData(const TextureData &data);
   inline void deleteTexture();
   
   HostFileSystem *_filesys;
   GLuint _texture;
   
   static std::map<std::string, OpenGLTexture*> Textures;
};

#endif /* defined(__FelixEngineIOS__OpenGLTexture__) */
