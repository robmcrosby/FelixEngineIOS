//
//  Camera.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Camera__
#define __FelixEngineIOS__Camera__

#include "FelixEngine.h"

class Camera: public Entity {
public:
   Camera(XMLTag *tag, Entity *parrent);
   virtual ~Camera();
   
private:
   DECLARE_ENTITY_ID(Camera)
};

#endif /* defined(__FelixEngineIOS__Camera__) */
