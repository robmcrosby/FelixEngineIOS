//
//  OpenGLESDisplay.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLESDisplay__
#define __FelixEngineIOS__OpenGLESDisplay__

#include <set>
#include "FelixEngine.h"

struct DrawCmp {
   inline bool operator() (const DrawableEntity *lhs, const DrawableEntity *rhs) const {
      if(lhs->getLayer() != rhs->getLayer())
         return lhs->getLayer() < rhs->getLayer();
      return lhs < rhs;
   }
};

class OpenGLESDisplay: public FelixHostDisplay {
public:
   OpenGLESDisplay(FelixHost *host);
   virtual ~OpenGLESDisplay() {}
   
   virtual void drawPasses();
   
private:
   inline void setDrawType(DRAW_TYPE type);
   
   FelixHost *_host;
   DRAW_TYPE _curDrawType;
   
   std::vector<std::set<const DrawableEntity*, DrawCmp> > _passes;
};


#endif /* defined(__FelixEngineIOS__OpenGLESDisplay__) */
