//
//  DisplayPipeline.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_DisplayPipeline_h
#define FelixEngineIOS_DisplayPipeline_h

#include "FelixEngine.h"

struct PipeItem {
   virtual void use() const = 0;
   virtual void discard() const = 0;
   virtual void draw() = 0;
};

/*
class PipeLine: public  {
public:
   PipeLine(XMLTag *tag, IScene *scene);
   virtual ~PipeLine();
   
   virtual void use() const;
   virtual void discard() const;
   virtual void draw() const;
protected:
   std::list<PipeItem *> pipeLine_;
private:
   DECLARE_ITEM_ID(PipeLine)
};*/

#endif
