//
//  Resources.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/20/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Resources__
#define __FelixEngineIOS__Resources__

#include "FelixEngine.h"

/**
 * Initally only contains file paths to resources. After loading, this
 * contains resources associated with the file paths.
 */
class Resources: Entity {
public:
   Resources(XMLTag *tag, Entity *parrent);
   virtual ~Resources();
   
   virtual void handleEvent(const Event &event);
   
private:
   inline void load();
   inline void unload();
   
   std::list<ShaderFiles> _shaderFiles;
   std::list<std::string> _imageFiles;
   std::list<std::string> _meshFiles;
   std::set<Resource*> _resources;
   
   DECLARE_ENTITY_ID(Resources)
};

#endif /* defined(__FelixEngineIOS__Resources__) */
