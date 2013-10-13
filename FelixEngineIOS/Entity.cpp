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


Entity::Entity(const XMLTag &tag): _tag(tag), _parrent(0), _loaded(0) {
  applyTag(tag);
}

Entity::~Entity() {
  clearParrent();
  clearChildren();
}

void Entity::applyTag(const XMLTag &tag) {
  // set the name
  if (tag.hasAttribute(ATT_NAME))
    setName(tag.getAttribute(ATT_NAME));
}

void Entity::createChildren(const XMLTag &tag) {
  for (XMLTag::const_iterator itr = tag.begin(); itr != tag.end(); ++itr) {
    Entity *child = EntityId::CreateEntity(**itr);
    if (child)
      addChild(child);
  }
}

void Entity::setParrent(Entity *parrent) {
  if (_parrent != parrent) {
    clearParrent();

    _parrent = parrent;
    if (_parrent)
      _parrent->_children.insert(this);
  }
}

void Entity::clearParrent() {
  if (_parrent)
    _parrent->_children.erase(this);
  _parrent = NULL;
}

void Entity::update(float td) {
  // update the children
  set<Entity*>::iterator itr;
  for (itr = _children.begin(); itr != _children.end(); ++itr)
    (*itr)->update(td);
}

void Entity::load() {
  _loaded = true;

  // load the children
  set<Entity*>::iterator itr;
  for (itr = _children.begin(); itr != _children.end(); ++itr)
    (*itr)->load();
}

void Entity::unload() {
  _loaded = false;

  // unload the children
  set<Entity*>::iterator itr;
  for (itr = _children.begin(); itr != _children.end(); ++itr)
    (*itr)->unload();
}

View* Entity::getView() {
  return getParrentView();
}

void Entity::setName(const std::string &name) {
  if (name == "") {
    EntityMap.erase(_name);
    _tag.removeAttribute(ATT_NAME);
  }
  else if (_name != name) {
    EntityMap.erase(_name);
    EntityMap[name] = this;
    _tag.setAttribute(ATT_NAME, _name);
  }
  _name = name;
}

View* Entity::getParrentView() {
  return _parrent ? _parrent->getView() : NULL;
}

const View* Entity::getParrentView() const {
  return _parrent ? _parrent->getView() : NULL;
}


Entity* Entity::GetEntity(const string &name) {
  map<string, Entity*>::iterator itr;
  itr = EntityMap.find(name);
  if (itr != EntityMap.end())
    return itr->second;

  cerr << "could not find entity with name: " << name << endl;
  return NULL;
}

Entity* EntityId::CreateEntity(const XMLTag &tag) {
  EntityId *curId = IDs;

  while (curId) {
    if (curId->_idStr == tag.getElement())
      return curId->create(tag);
    curId = curId->_next;
  }

  cerr << "could not find entity type: " << tag.getElement() << endl;
  return NULL;
}

EntityId::EntityId(const string &idStr): _idStr(idStr) {
  _next = IDs;
  IDs = this;
}