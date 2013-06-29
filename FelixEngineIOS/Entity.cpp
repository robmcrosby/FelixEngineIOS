//
//  Entity.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Entity.h"

using namespace std;


map<string, Entity*> Entity::EntityMap;
EntityId* EntityId::IDs;


Entity::Entity(XMLTag *tag): _tag(tag), _parrent(0), _subject(0) {
   if (_tag && _tag->hasAttribute("name"))
      EntityMap[_tag->getAttribute("name")] = this;
}

Entity::~Entity() {
   clearListeners();
   clearChildren();
   if (_subject)
      _subject->removeListener(this);
}

void Entity::createChildren(XMLTag *tag) {
   for (XMLTag::iterator itr = tag->begin(); itr != tag->end(); ++itr) {
      Entity *child = EntityId::CreateEntity(*itr);
      if (child) {
         addChild(child);
         if (!child->_subject)
            addListener(child);
      }
   }
}

Entity* Entity::GetEntity(const string &name) {
   map<string, Entity*>::iterator itr;
   itr = EntityMap.find(name);
   if (itr != EntityMap.end())
      return itr->second;
   
   cerr << "could not find entity with name: " << name << endl;
   return NULL;
}

Entity* EntityId::CreateEntity(XMLTag *tag) {
   EntityId *curId = IDs;
   
   while (curId) {
      if (curId->_idStr == tag->getElement())
         return curId->create(tag);
      curId = curId->_next;
   }
   
   cerr << "could not find entity type: " << tag->getElement() << endl;
   return NULL;
}

EntityId::EntityId(const string &idStr): _idStr(idStr) {
   _next = IDs;
   IDs = this;
}