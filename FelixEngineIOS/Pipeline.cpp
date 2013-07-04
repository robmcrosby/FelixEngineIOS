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
   applyTag();
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

void Pipeline::applyTag() {
   if (_tag) {
      XMLTag::iterator itr;
      for (itr = _tag->begin(); itr != _tag->end(); ++itr) {
         XMLTag &subTag = **itr;
         
         if (subTag == "clear")
            _pipeline.push_back(new ClearContext(vec4::ParseFloat(subTag.getContents())));
         else if (subTag == "pass")
            _pipeline.push_back(new DrawPass(subTag.getContents()));
         else if (subTag == "frame")
            _pipeline.push_back(new UseFBO(subTag.getContents()));
      }
   }
}




void Pipeline::ClearContext::exec() {
   if (_display)
      _display->clearContext(clearColor);
}

void Pipeline::DrawPass::exec() {
   if (_display)
      _display->drawPass(pass);
}

void Pipeline::UseFBO::load(HostDisplay *display) {
   PipeItem::load(display);
   fbo = display->getFrameBuff(fboName);
}

void Pipeline::UseFBO::unload() {
   PipeItem::unload();
   fbo = NULL;
}

void Pipeline::UseFBO::exec() {
   if (fbo)
      fbo->use();
}

