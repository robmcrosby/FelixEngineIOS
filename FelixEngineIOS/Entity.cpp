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


Entity::Entity(XMLTag *tag): _tag(tag), _parrent(0), _loaded(0) {
  applyTag();
}

Entity::~Entity() {
  clearParrent();
  clearChildren();
}

void Entity::applyTag() {
  if (_tag) {
    // set the transform
    if (_tag->hasSubTag(TRANSFORM_TAG))
      _transform.applyTag(*_tag->getSubTag(TRANSFORM_TAG));
    else
      _transform.applyTag(*_tag);

    // set the name and update the entity map
    if (_tag->hasAttribute(ATT_NAME))
      setName(_tag->getAttribute(ATT_NAME));
  }
}

void Entity::createChildren(XMLTag *tag) {
  for (XMLTag::iterator itr = tag->begin(); itr != tag->end(); ++itr) {
    Entity *child = EntityId::CreateEntity(*itr);
    if (child)
      addChild(child);
  }
}

void Entity::setParrent(Entity *parrent) {
  if (_parrent != parrent) {
    clearParrent();

    _parrent = parrent;
    if (_parrent) {
      _parrent->_children.insert(this);
    }
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