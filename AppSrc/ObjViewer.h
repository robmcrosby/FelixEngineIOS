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

class ObjViewer: public FelixEntity {
public:
   ObjViewer(XMLTag *tag, FelixEntity *parent);
   virtual ~ObjViewer();
   
private:
   DECLARE_ENTITY_ID(ObjViewer)
};

#endif /* defined(__FelixEngineIOS__ObjViewer__) */
