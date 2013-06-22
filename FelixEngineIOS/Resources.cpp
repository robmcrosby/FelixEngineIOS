//
//  Resources.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/20/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Resources.h"

DEFINE_ENTITY_ID(Resources)

using namespace std;


Resources::Resources(XMLTag *tag, Entity *parrent): Entity(tag, parrent) {
   if (parrent)
      parrent->addListener(this);
   
   for (XMLTag::iterator itr = tag->begin(); itr != tag->end(); ++itr) {
      Resource *resource = Host::GetHost()->getResource(**itr);
      if (resource) {
         resource->setToTag(**itr);
         _resources.insert(resource);
      }
   }
}

Resources::~Resources() {
   unload();
}

void Resources::handleEvent(const Event &event) {
   if (event == EVENT_LOAD)
      load();
   else if (event == EVENT_UNLOAD)
      unload();
}

void Resources::load() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->load();
}

void Resources::unload() {
   std::set<Resource*>::iterator itr;
   for (itr = _resources.begin(); itr != _resources.end(); ++itr)
      (*itr)->unload();
}