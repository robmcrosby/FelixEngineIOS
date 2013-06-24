//
//  IOSFileSystem.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__IOSFileSystem__
#define __FelixEngineIOS__IOSFileSystem__

#include "FelixEngine.h"

class IOSFileSystem: public HostFileSystem {
public:
   IOSFileSystem(Host *host);
   virtual ~IOSFileSystem() {}
   
   virtual std::string loadTxt(const std::string &path) const;
   virtual XMLTag* loadXML(const std::string &path) const;
   virtual TextureData* loadTexture(const std::string &path) const;
   virtual bool loadMeshes(const std::string &path, std::map<std::string, MeshData*> *data) const;
protected:
   Host *_host;
   std::string _basePath;
};

#endif /* defined(__FelixEngineIOS__IOSFileSystem__) */
