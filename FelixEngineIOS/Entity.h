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
 * Base class for all Felix Entities
 */
class Entity {
public:
   Entity(XMLTag *tag = NULL, Entity *parrent = NULL);
   virtual ~Entity();
   
   virtual void handleEvent(const Event &event) {notifyList(event);}
   virtual void createChildren(XMLTag *tag);
   
   inline void addChild(Entity *child) {_children.insert(child);}
   inline void removeChild(Entity *child) {_children.erase(child);}
   
   inline void addListener(Entity *listener) {_listeners.insert(listener);}
   inline void removeListener(Entity *listener) {_listeners.erase(listener);}
   inline void notifyList(const Event &event) {
      for (std::set<Entity*>::iterator itr = _listeners.begin(); itr != _listeners.end(); ++itr)
         (*itr)->handleEvent(event);
   }
   
protected:
   std::set<Entity*> _children;
   XMLTag *_tag;
   Entity *_parrent;
   
private:
   std::set<Entity*> _listeners;
};


/**
 * Used for reflection with xml
 */
class EntityId {
public:
   static Entity* CreateEntity(XMLTag *tag, Entity *parrent);
   
protected:
   EntityId(const std::string &idStr);
   virtual ~EntityId() {}
   virtual Entity* create(XMLTag *tag, Entity *parrent) = 0;
   
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
virtual Entity *create(XMLTag *tag, Entity *parrent);\
};\
static T##Id ID;


/**
 * Macro used to define the EntityId sub class. Should be used in the
 * implementation file of the targeted Entity.
 */
#define DEFINE_ENTITY_ID(T) \
T::T##Id T::ID;\
Entity *T::T##Id::create(XMLTag *tag, Entity *parrent) {\
return new T(tag, parrent);\
}

#endif
