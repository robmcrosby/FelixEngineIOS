//
//  Pipeline.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/30/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Pipeline.h"
#include "FelixEngine.h"

using namespace std;
DEFINE_ENTITY_ID(Pipeline)


Pipeline::Pipeline(): _display(0) {
   _pipeline.push_back(new ClearContext());
   _pipeline.push_back(new DrawPass(MAIN_PASS));
   _pipeline.push_back(new DrawPass(SCREEN_PASS));
}

Pipeline::Pipeline(XMLTag *tag): Entity(tag), _display(0) {
   
}

Pipeline::~Pipeline() {
   list<PipeItem*>::iterator itr;
   for (itr = _pipeline.begin(); itr != _pipeline.end(); ++itr)
      delete *itr;
}

void Pipeline::load(HostDisplay *display) {
   list<PipeItem*>::iterator itr;
   
   _display = display;
   _display->addListener(this);
   for (itr = _pipeline.begin(); itr != _pipeline.end(); ++itr)
      (*itr)->load(_display);
}

void Pipeline::unload() {
   for (list<PipeItem*>::iterator itr = _pipeline.begin(); itr != _pipeline.end(); ++itr)
      (*itr)->unload();
   _display->removeListener(this);
   _display = NULL;
}

void Pipeline::exec() {
   list<PipeItem*>::iterator itr;
   for (itr = _pipeline.begin(); itr != _pipeline.end(); ++itr)
      (*itr)->exec();
}


void Pipeline::ClearContext::exec() {
   if (_display)
      _display->clearContext(clearColor);
}

void Pipeline::DrawPass::exec() {
   if (_display)
      _display->drawPass(pass);
}