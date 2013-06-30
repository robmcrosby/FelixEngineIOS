//
//  Entity.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_Entity_h
#define FelixEngineIOS_Entity_h

#include <iostream>
#include <set>
#include <vector>
#include "XMLTag.h"
#include "Vector.h"
#include "Color.h"
#include "Transform.h"

/**
 * Defines the type of event
 */
enum EVENT_TYPE {
   EVENT_MEM_WARNING,
   EVENT_RESIZE,
   EVENT_UPDATE,
   EVENT_RENDER,
   EVENT_TOUCH_DOWN,
   EVENT_TOUCH_UP,
   EVENT_TOUCH_MOVE,
   EVENT_LOAD,
   EVENT_UNLOAD,
   EVENT_OTHER,
};


/**
 * Base struct for all events used Entities
 */
struct Event {
   Event(EVENT_TYPE e = EVENT_OTHER, void *d = NULL): type(e), data(d) {}
   bool operator==(const Event &e) const {return e.type == type;}
   
   EVENT_TYPE type;
   void *data;
};


/**
 * Holds move information for touches and mouse
 */
struct Move {
   Move(float cx, float cy, float px, float py): cur(cx, cy), pre(px, py) {}
   vec2 cur;
   vec2 pre;
};
typedef std::vector<Move> Moves;


/**
 * Listener class
 */
class EventHandler {
public:
   EventHandler(): _subject(0) {}
   ~EventHandler() {
      if (_subject)
         _subject->removeListener(this);
   }
   
   virtual void notify(const Event &event) {notifyListeners(event);}
   
   inline EventHandler* getSubject() {return _subject;}
   inline const EventHandler* getSubject() const {return _subject;}
   inline void addListener(EventHandler *listener) {
      if (listener->_subject)
         listener->_subject->removeListener(listener);
      _listeners.insert(listener);
      listener->_subject = this;
   }
   inline void removeListener(EventHandler *listener) {
      _listeners.erase(listener);
      listener->_subject = NULL;
   }
   inline void notifyListeners(const Event &event) {
      if (_listeners.size()) {
         std::set<EventHandler*>::iterator itr;
         std::set<EventHandler*> listeners = _listeners;
         for (itr = _listeners.begin(); itr != _listeners.end(); ++itr)
            (*itr)->notify(event);
      }
   }
   inline void clearListeners() {
      std::set<EventHandler*>::iterator itr;
      std::set<EventHandler*> listeners = _listeners;
      for (itr = listeners.begin(); itr != listeners.end(); ++itr)
         removeListener(*itr);
   }
   
private:
   std::set<EventHandler*> _listeners;
   EventHandler *_subject;
};

/**
 * Base class for all Felix Entities
 */
class Entity: public EventHandler {
public:
   Entity(XMLTag *tag = NULL);
   virtual ~Entity();
   
   virtual void createChildren(XMLTag *tag);
   virtual void addChild(Entity *child) {
      if (child->_parrent)
         child->_parrent->removeChild(child);
      child->_parrent = this;
      _children.insert(child);
   }
   virtual void removeChild(Entity *child) {
      if (child->_parrent == this)
         child->_parrent = NULL;
      _children.erase(child);
   }
   inline void clearChildren() {
      std::set<Entity*>::iterator itr;
      for (itr = _children.begin(); itr != _children.end(); ++itr)
         delete *itr;
   }
   
   inline void setTransformParrent(const Transform *t) {_transform.setParrent(t);}
   
   inline std::string getName() const {
      if (_tag && _tag->hasAttribute("name"))
         return _tag->getAttribute("name");
      return "";
   }
   
   inline Transform* getTransform() {return &_transform;}
   inline const Transform* getTransform() const {return &_transform;}
   
   static Entity* GetEntity(const std::string &name);
   
protected:
   Transform _transform;
   XMLTag *_tag;
   
private:
   std::set<Entity*> _children;
   Entity *_parrent;
   
   static std::map<std::string, Entity*> EntityMap;
};


/**
 * Used for reflection with xml
 */
class EntityId {
public:
   static Entity* CreateEntity(XMLTag *tag);
   
protected:
   EntityId(const std::string &idStr);
   virtual ~EntityId() {}
   virtual Entity* create(XMLTag *tag) = 0;
   
   std::string _idStr;
   EntityId *_next;
   
   static EntityId *IDs;
};


/**
 * Macro used to declare a sub class of EntityId for creating a particular
 * Entity. Should be used in header of Entity.
 */
#define DECLARE_ENTITY_ID(T) \
class T##Id: public EntityId {\
public:\
T##Id(): EntityId(#T) {}\
virtual ~T##Id() {}\
virtual Entity *create(XMLTag *tag);\
};\
static T##Id ID;


/**
 * Macro used to define the EntityId sub class. Should be used in the
 * implementation file of the targeted Entity.
 */
#define DEFINE_ENTITY_ID(T) \
T::T##Id T::ID;\
Entity *T::T##Id::create(XMLTag *tag) {\
return new T(tag);\
}

#endif
