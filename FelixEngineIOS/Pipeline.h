//
//  Pipeline.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Pipeline__
#define __FelixEngineIOS__Pipeline__

#include "Entity.h"

struct PipeItem {
   virtual ~PipeItem() {}
   virtual void load() {}
   virtual void unload() {}
   virtual void exec() {}
};

class Pipeline: public Entity {
public:
   Pipeline();
   Pipeline(XMLTag *tag);
   virtual ~Pipeline();
   
private:
   
};


#endif /* defined(__FelixEngineIOS__Pipeline__) */
