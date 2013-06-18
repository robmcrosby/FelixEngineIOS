//
//  FelixEngine.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__FelixEngine__
#define __FelixEngineIOS__FelixEngine__

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include "Vector.h"
#include "XMLTag.h"

enum EVENT_TYPE {
   EVENT_MEM_WARNING,
   EVENT_RESIZE,
   EVENT_UPDATE,
   EVENT_RENDER,
   EVENT_TOUCH_DOWN,
   EVENT_TOUCH_UP,
   EVENT_TOUCH_MOVE,
   EVENT_OTHER,
};

enum DEV_TYPE {
   DEV_PHONE,
   DEV_TABLET,
};

enum DRAW_TYPE {
   DRAW_DEPTH,
   DRAW_BLEND,
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
 * Base struct for all Felix Events
 */
struct FelixEvent {
   FelixEvent(EVENT_TYPE e = EVENT_OTHER, void *d = NULL): type(e), data(d) {}
   bool operator==(const FelixEvent &e) const {return e.type == type;}
   
   EVENT_TYPE type;
   void *data;
};


/**
 * Base class for all Felix resources
 */
class FelixResource {
public:
   FelixResource(const std::string &str): _id(str), _count(1), _loaded(0) {RMap[_id] = this;}
   virtual ~FelixResource() {RMap.erase(_id);}
   
   inline size_t retain() {return ++_count;}
   inline size_t release() {
      size_t count = --_count;
      if (!count)
         delete this;
      return count;
   }
   
   virtual bool load() = 0;
   virtual bool unload() = 0;
   inline bool loaded() const {return _loaded;}
   
   inline size_t getCount() const {return _count;}
   inline std::string getId() const {return _id;}
   
   static FelixResource* GetResource(const std::string &str) {
      return RMap.find(str) != RMap.end() ? RMap[str] : NULL;
   }
   
private:
   std::string _id;
   size_t _count;
   bool _loaded;
   static std::map<std::string, FelixResource*> RMap;
};


/**
 * Base class for all Felix Entities
 */
class FelixEntity {
public:
   FelixEntity(XMLTag *tag = NULL, FelixEntity *parrent = NULL): _tag(tag), _parrent(parrent) {
      if (_parrent)
         _parrent->addListener(this);
   }
   virtual ~FelixEntity() {}
   
   virtual void handleEvent(const FelixEvent &event) {notifyList(event);}
   
   inline void addListener(FelixEntity *listener) {_list.push_back(listener);}
   inline void removeListener(FelixEntity *listener) {_list.remove(listener);}
   inline void notifyList(const FelixEvent &event) {
      for (std::list<FelixEntity*>::iterator itr = _list.begin(); itr != _list.end(); ++itr)
         (*itr)->handleEvent(event);
   }
   
protected:
   XMLTag *_tag;
   FelixEntity *_parrent;
   
private:
   std::list<FelixEntity*> _list;
};


/**
 * Base class for all Drawable Felix Entities
 */
class DrawableEntity: FelixEntity {
public:
   DrawableEntity(): _pass(0), _layer(0), _drawType(DRAW_BLEND) {}
   virtual ~DrawableEntity() {}
   
   virtual void draw() = 0;
   
   inline void setPass(int pass)  {_pass = pass;}
   inline void setLayer(int layer) {_layer = layer;}
   inline int getPass()  const {return _pass;}
   inline int getLayer() const {return _layer;}
   
   inline void setDrawType(DRAW_TYPE type) {_drawType = type;}
   inline DRAW_TYPE getDrawType() const {return _drawType;}
   
protected:
   int _pass;
   int _layer;
   DRAW_TYPE _drawType;
};


/**
 * Base class for working with a host display context
 */
class FelixHostDisplay: public FelixEntity {
public:
   virtual ~FelixHostDisplay() {}
   virtual void drawPasses() = 0;
};

/**
 * Base class for working with a host audio context
 */
class FelixHostAudio: public FelixEntity {
public:
   virtual ~FelixHostAudio() {}
};

/**
 * Base class for working with the host file system
 */
class FelixHostFileSys: public FelixEntity {
public:
   virtual ~FelixHostFileSys() {}
};


/**
 * Represents the host system
 */
class FelixHost: public FelixEntity {
public:
   FelixHost(DEV_TYPE dev, ivec2 size, float scale): _device(dev), _size(size), _scale(scale),
   _display(0), _audio(0), _fileSys(0) {}
   virtual ~FelixHost() {}
   
   inline DEV_TYPE getDeviceType()  const {return _device;}
   
   inline ivec2 getScreenSize()  const {return _size;}
   inline float getScreenScale() const {return _scale;}
   
   inline FelixHostDisplay *getDisplay()    const {return _display;}
   inline FelixHostAudio   *getAudio()      const {return _audio;}
   inline FelixHostFileSys *getFileSystem() const {return _fileSys;}
   
   static FelixHost* GetHost() {return Instance;}
   
protected:
   DEV_TYPE _device;
   ivec2 _size;
   float _scale;
   
   FelixHostDisplay *_display;
   FelixHostAudio *_audio;
   FelixHostFileSys *_fileSys;
   
   static FelixHost *Instance;
};


/**
 * Used for reflection with xml
 */
class EntityId {
public:
   static FelixEntity* CreateEntity(XMLTag *tag, FelixEntity *parrent);
   
protected:
   EntityId(const std::string &idStr);
   virtual ~EntityId() {}
   virtual FelixEntity* create(XMLTag *tag, FelixEntity *parrent) = 0;
   
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
virtual Item *create(XMLTag *tag, FelixEntity *parrent);\
};\
static T##Id ID;

/**
 * Macro used to define the EntityId sub class. Should be used in the
 * implementation file of the targeted Entity.
 */
#define DEFINE_ENTITY_ID(T) \
T::T##Id T::ID;\
Item *T::T##Id::create(XMLTag *tag, FelixEntity *parrent) {\
return new T(tag, parrent);\
}



#endif /* defined(__FelixEngineIOS__FelixEngine__) */
