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


Entity::Entity(XMLTag *tag): _tag(tag), _parrent(0) {
   applyTag();
}

Entity::~Entity() {
   clearListeners();
   clearChildren();
}

void Entity::applyTag() {
   if (_tag) {
      // set the transform
      if (_tag->hasSubTag("Transform"))
         _transform.applyTag(*_tag->getSubTag("Transform"));
      else
         _transform.applyTag(*_tag);
      
      // set the name and update the entity map
      if (_tag->hasAttribute("name")) {
         string name = _tag->getAttribute("name");
         
         // reset or add entity to entity map
         if (_name != "" && _name != name)
            EntityMap.erase(_name);
         _name = name;
         if (_name != "")
            EntityMap[_name] = this;
      }
      else {
         if (_name != "")
            EntityMap.erase(_name);
         _name = "";
      }
   }
}

void Entity::createChildren(XMLTag *tag) {
   for (XMLTag::iterator itr = tag->begin(); itr != tag->end(); ++itr) {
      Entity *child = EntityId::CreateEntity(*itr);
      if (child) {
         addChild(child);
         if (!child->getSubject())
            addListener(child);
      }
   }
}

void Entity::addChild(Entity *child) {
   if (child->_parrent)
      child->_parrent->removeChild(child);
   child->_parrent = this;
   _children.insert(child);
}

void Entity::removeChild(Entity *child) {
   if (child->_parrent == this)
      child->_parrent = NULL;
   _children.erase(child);
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