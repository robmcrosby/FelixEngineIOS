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

class HostDisplay;

/**
 Defines a render pipeline with a list of PipeItems.
 */
class Pipeline: public Entity {
public:
   Pipeline();
   Pipeline(XMLTag *tag);
   virtual ~Pipeline();
   
   virtual void notify(const Event &event);
   virtual void load(HostDisplay *display);
   virtual void unload();
   virtual void exec();
   
   /**
    Base class for all Pipeline items
    */
   class PipeItem {
   public:
      PipeItem(): _display(0) {}
      virtual ~PipeItem() {}
      
      virtual void load(HostDisplay *display) {_display = display;}
      virtual void unload() {_display = NULL;}
      virtual void update() {}
      virtual void exec() {}
      
   protected:
      HostDisplay *_display;
   };
   
   /**
    Item for clearing a context with a color
    */
   struct ClearContext: public PipeItem {
      ClearContext(const Color &c = Color()): clearColor(c) {}
      virtual void exec();
      Color clearColor;
   };
   
   /**
    Item for drawing a specified pass
    */
   struct DrawPass: public PipeItem {
      DrawPass(const std::string &p): pass(p) {}
      virtual void exec();
      std::string pass;
   };
   
private:
   std::list<PipeItem*> _pipeline;
   HostDisplay *_display;
   
   DECLARE_ENTITY_ID(Pipeline)
};


#endif /* defined(__FelixEngineIOS__Pipeline__) */
