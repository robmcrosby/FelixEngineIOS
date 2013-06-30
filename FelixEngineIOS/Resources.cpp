//
//  Resources.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/20/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "FelixEngine.h"
#include "Resources.h"

DEFINE_ENTITY_ID(Resources)

using namespace std;


Resources::Resources(XMLTag *tag): Entity(tag), _loaded(0) {
   Host *host = Host::GetHost();
   
   for (XMLTag::iterator itr = tag->begin(); itr != tag->end(); ++itr) {
      Resource *resource = host->getResource(**itr);
      if (resource) {
         resource->retain();
         _resources.insert(resource);
      }
   }
   
   // clean up any unused resources
   host->cleanUpResources();
}

Resources::~Resources() {
   unloadResources();
   releaseResources();
}

void Resources::handleEvent(const Event &event) {
   if (event == EVENT_LOAD && !_loaded)
      loadResources();
   else if (event == EVENT_UNLOAD && _loaded)
      unloadResources();
}

void Resources::loadResources() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->load();
   _loaded = true;
}

void Resources::unloadResources() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->unload();
   _loaded = false;
}

void Resources::releaseResources() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->release();
   _loaded = false;
   _resources.clear();
   
   // clean up any unused resources
   Host::GetHost()->cleanUpResources();
}

