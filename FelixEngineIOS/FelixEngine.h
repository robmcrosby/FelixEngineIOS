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
#include <set>
#include <list>
#include <vector>
#include "Vector.h"
#include "XMLTag.h"

#define APP_CONFIG "Config/main.xml"

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

enum DEV_TYPE {
   DEV_PHONE,
   DEV_TABLET,
};

enum VAL_TYPE {
   VAL_NONE,
   VAL_INT,
   VAL_FLOAT,
   VAL_VEC2,
   VAL_VEC3,
   VAL_VEC4,
   VAL_MAT2X2,
   VAL_MAT3X3,
   VAL_MAT4X4,
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
struct Event {
   Event(EVENT_TYPE e = EVENT_OTHER, void *d = NULL): type(e), data(d) {}
   bool operator==(const Event &e) const {return e.type == type;}
   
   EVENT_TYPE type;
   void *data;
};


/**
 * Defines a uniform value
 */
struct Uniform {
   Uniform(): type(VAL_NONE) {}

   VAL_TYPE type;
   int count;
   void *value;
};

/**
 * Collection of uniforms
 */
typedef std::map<std::string, Uniform> Uniforms;

/**
 * Defines an attribute for a shader
 */
struct Attribute {
   Attribute(): size(0), offset(0) {}
   Attribute(int s, int o): size(s), offset(o) {}

   int size, offset;
};

/**
 * Collection of attributes
 */
class Attributes {
public:
   Attributes(): _stride(0) {}
   typedef std::map<std::string, Attribute>::const_iterator const_iterator;
   inline const_iterator begin() const {return _atts.begin();}
   inline const_iterator end() const {return _atts.end();}
   inline int getStride() const {return _stride;}
   inline void push_back(const std::string &name, int s, int o) {
      _atts[name] = Attribute(s, o);
      _stride += s;
   }
   
private:
   int _stride;
   std::map<std::string, Attribute> _atts;
};

/**
 * Base class for all Felix resources
 */
class Resource {
public:
   Resource(const XMLTag &tag): _tag(tag), _count(0) {}
   virtual ~Resource() {}
   
   virtual void load() {++_count;}
   virtual void unload() {_count -= _count == 0 ? 0 : 1;}
   inline void setToTag(const XMLTag &tag) {_tag = tag;}
   
   inline bool loaded() const {return _count;}
   inline size_t getCount() const {return _count;}
   inline std::string getName() const {return _tag.getAttribute("name");}
   
protected:
   XMLTag _tag;
   size_t _count;
};

/**
 * Base class for shader resources
 */
class Shader: public Resource {
public:
   Shader(const XMLTag &tag): Resource(tag) {}
   
   virtual void use() const = 0;
   virtual void setUniforms(const Uniforms &unis) const = 0;
   virtual void setAttributes(const Attributes &atts) const = 0;
   
   static const Shader* GetActive() {return Active;}
   
protected:
   static const Shader *Active;
};

/**
 * Base class for textures
 */
struct Texture: public Resource {
   Texture(const XMLTag &tag): Resource(tag) {}
   
   virtual void use(int idx) const = 0;
};

/**
 * Base class for meshes
 */
struct Mesh: public Resource {
   Mesh(const XMLTag &tag): Resource(tag) {}
   
   virtual void use() const = 0;
   virtual void render() const = 0;
};


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

struct ShaderFiles {
   std::string name, vert, frag;
};

/**
 * Base class for all Drawable Felix Entities
 */
class Drawable: Entity {
public:
   Drawable(): _pass(0), _layer(0), _drawType(DRAW_BLEND) {}
   virtual ~Drawable() {}
   
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
class HostDisplay: public Entity {
public:
   virtual ~HostDisplay() {}
   virtual void drawPasses() = 0;
   virtual void addPassUniform(const Uniform &uniform, int pass) = 0;
   virtual void clearPassUniforms(int pass) = 0;
   
   virtual Resource* getResource(const XMLTag &tag) = 0;
   virtual const Shader* getShader(const std::string &name) = 0;
   virtual const Texture* getTexture(const std::string &name) = 0;
   virtual const Mesh* getMesh(const std::string &name) = 0;
};

/**
 * Base class for working with a host audio context
 */
class HostAudio: public Entity {
public:
   virtual ~HostAudio() {}
   
   virtual Resource* getResource(const XMLTag &tag) = 0;
};

/**
 * Base class for working with the host file system
 */
class HostFileSystem: public Entity {
public:
   virtual ~HostFileSystem() {}
   virtual std::string loadTxt(const std::string &path) const = 0;
   virtual XMLTag* loadXML(const std::string &path) const = 0;
};


/**
 * Represents the host system
 */
class Host: public Entity {
public:
   inline DEV_TYPE getDeviceType()  const {return _device;}
   
   inline ivec2 getScreenSize()  const {return _size;}
   inline float getScreenScale() const {return _scale;}
   
   inline HostDisplay *getDisplay() const {return _display;}
   inline HostAudio *getAudio() const {return _audio;}
   inline HostFileSystem *getFileSystem() const {return _fileSys;}
   
   inline Resource* getResource(const XMLTag &tag) {
      if (tag == "Shader" || tag == "Texture" || tag == "Mesh")
         return _display->getResource(tag);
      else if (tag == "Sound")
         return _audio->getResource(tag);
      return NULL;
   }
   
   static Host* GetHost() {return Instance;}
   
protected:
   Host(DEV_TYPE dev, ivec2 size, float scale);
   virtual ~Host();
   
   void createAppEntity();
   
   DEV_TYPE _device;
   ivec2 _size;
   float _scale;
   
   HostDisplay *_display;
   HostAudio *_audio;
   HostFileSystem *_fileSys;
   
   static Host *Instance;
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



#endif /* defined(__FelixEngineIOS__FelixEngine__) */
