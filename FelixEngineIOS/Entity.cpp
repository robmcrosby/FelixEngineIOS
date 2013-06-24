//
//  Entity.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Entity.h"

using namespace std;


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