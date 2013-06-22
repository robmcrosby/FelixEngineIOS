//
//  FelixEngine.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "FelixEngine.h"

using namespace std;

const Shader* Shader::Active = NULL;
Host* Host::Instance = NULL;
EntityId* EntityId::IDs;


Entity::Entity(XMLTag *tag, Entity *parrent): _tag(tag), _parrent(parrent) {
   if (_parrent)
      _parrent->addChild(this);
}

Entity::~Entity() {
   for (set<Entity*>::iterator itr = _children.begin(); itr != _children.end(); ++itr)
      delete *itr;
}

void Entity::createChildren(XMLTag *tag) {
   for (XMLTag::iterator itr = tag->begin(); itr != tag->end(); ++itr) {
      Entity *child = EntityId::CreateEntity(*itr, this);
      if (child)
         addChild(child);
   }
}


Host::Host(DEV_TYPE dev, ivec2 size, float scale):
_device(dev), _size(size), _scale(scale), _display(0), _audio(0), _fileSys(0) {
   Instance = this;
}

Host::~Host() {
   delete _display;
   delete _audio;
   delete _fileSys;
}

void Host::createAppEntity() {
   XMLTag *tag = _fileSys->loadXML(APP_CONFIG);
   if (!tag) {
      cerr << "Unable to create main app object" << endl;
      exit(1);
   }
   
   Entity *app = EntityId::CreateEntity(tag, this);
   if (!app) {
      cerr << "Unable to create main app object" << endl;
      delete tag;
      exit(1);
   }
   addChild(app);
}


Entity* EntityId::CreateEntity(XMLTag *tag, Entity *parrent) {
   EntityId *curId = IDs;
   
   while (curId) {
      if (curId->_idStr == tag->getElement())
         return curId->create(tag, parrent);
      curId = curId->_next;
   }
    
   cerr << "could not find entity: " << tag->getElement() << endl;
   return NULL;
}

EntityId::EntityId(const string &idStr): _idStr(idStr) {
   _next = IDs;
   IDs = this;
}
