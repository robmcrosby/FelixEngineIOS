//
//  OpenGLSLMesh.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLMesh__
#define __FelixEngineIOS__OpenGLMesh__

#include "FelixEngine.h"
#include "OpenGLIncludes.h"

class OpenGLMesh: public Mesh {
private:
   OpenGLMesh(const std::string &name);
   virtual ~OpenGLMesh();
   
public:
   static OpenGLMesh* GetMesh(const std::string &name);
   static void ClearMeshes();
   
   virtual void load();
   virtual void unload();
   
   virtual void use() const;
   virtual void render() const;
   
private:
   
   static std::map<std::string, OpenGLMesh*> Meshes;
};

#endif /* defined(__FelixEngineIOS__OpenGLMesh__) */
