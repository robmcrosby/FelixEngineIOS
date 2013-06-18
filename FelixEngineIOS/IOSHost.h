//
//  IOSHost.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__IOSHost__
#define __FelixEngineIOS__IOSHost__

#include "FelixEngine.h"
#include <vector>


class IOSHost: public FelixHost {
public:
   IOSHost(DEV_TYPE dev, ivec2 size, float scale);
   virtual ~IOSHost();
   
   void lowMemory();
   void resize(int sizeX, int sizeY, float scale);
   void update(float td);
   void render();
   void touchDown(Moves moves);
   void touchUp(Moves moves);
   void touchMove(Moves moves);
   
   static IOSHost* CreateIOSHost(DEV_TYPE dev, int sizeX, int sizeY, float scale);
};

#endif /* defined(__FelixEngineIOS__IOSHost__) */
