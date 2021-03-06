//
//  ObjViewer.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__ObjViewer__
#define __FelixEngineIOS__ObjViewer__

#include "FelixEngine.h"
#include "Matrix.h"

class ObjViewer: public Entity {
public:
   ObjViewer(const XMLTag &tag);
   virtual ~ObjViewer();
   
   //virtual void notify(const Event &event);
   
private:
   //Entity *bunny;
   Entity *plane;
   
   DECLARE_ENTITY_ID(ObjViewer)
};

#endif /* defined(__FelixEngineIOS__ObjViewer__) */
