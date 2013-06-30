//
//  Resources.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/20/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Resources__
#define __FelixEngineIOS__Resources__

#include "Entity.h"
#include "ResourceData.h"
#include "ShaderVaribles.h"

#define ATT_NAME "name"

enum DRAW_TYPE {
   DRAW_DEPTH,
   DRAW_BLEND,
};

/**
 * Base class for all resources
 */
class Resource {
public:
   Resource(const XMLTag &tag): _tag(tag), _count(0), _loaded(0) {}
   virtual ~Resource() {}
   
   inline void retain() {++_count;}
   inline void release() {_count -= _count == 0 ? 0 : 1;}
   inline void setToTag(const XMLTag &tag) {_tag = tag;}
   
   virtual void load() = 0;
   virtual void unload() = 0;
   
   inline bool loaded() const {return _loaded;}
   inline size_t getCount() const {return _count;}
   inline std::string getName() const {return _tag.getAttribute(ATT_NAME);}
   
protected:
   XMLTag _tag;
   size_t _count;
   bool _loaded;
   
};

/**
 * Base class for shader resources
 */
class Shader: public Resource {
public:
   Shader(const XMLTag &tag): Resource(tag) {}
   
   virtual void setToData(const ShaderData &data) = 0;
   
   virtual void use() const = 0;
   virtual void setUniforms(const Uniforms *unis) const = 0;
   virtual void setAttributes(const Attributes *atts) const = 0;
};

/**
 * Base class for textures
 */
struct Texture: public Resource {
   Texture(const XMLTag &tag): Resource(tag) {}
   
   virtual void setToData(const TextureData &data) = 0;
   virtual void use(int idx) const = 0;
};

/**
 * Base class for meshes
 */
struct Mesh: public Resource {
   Mesh(const XMLTag &tag): Resource(tag) {}
   
   virtual void setToData(const MeshData &data) = 0;
   
   virtual void use() const = 0;
   virtual void draw() const = 0;
   
   static MeshData* GetPlaneData();
};

/**
 * Initally only contains file paths to resources. After loading, this
 * contains resources associated with the file paths.
 */
class Resources: Entity {
public:
   Resources(XMLTag *tag);
   virtual ~Resources();
   
   virtual void handleEvent(const Event &event);
   
private:
   inline void loadResources();
   inline void unloadResources();
   inline void releaseResources();
   
   bool _loaded;
   std::set<Resource*> _resources;
   
   DECLARE_ENTITY_ID(Resources)
};

#endif /* defined(__FelixEngineIOS__Resources__) */
