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


#define SHADER_TAG "Shader"
#define TEXTURE_TAG "Texture"
#define TEXTURES_TAG "Textures"
#define MESH_TAG "Mesh"
#define FBO_TAG "FrameBuff"

#define SOUND_TAG "Sound"

#define ATT_NAME "name"

enum DRAW_TYPE {
   DRAW_DEPTH,
   DRAW_BLEND,
};

/**
 Base class for all resources
 */
class Resource {
public:
   Resource(const std::string &name, const std::string &tag): _tag(tag), _count(0), _loaded(0) {
      _tag.setAttribute(ATT_NAME, name);
   }
   virtual ~Resource() {}
   
   inline void retain() const {++_count;}
   inline void release() const {_count -= _count == 0 ? 0 : 1;}
   inline size_t getCount() const {return _count;}
   
   inline void setToTag(const XMLTag &tag) {_tag = tag;}
   
   virtual void load() = 0;
   virtual void unload() = 0;
   inline bool loaded() const {return _loaded;}
   
   inline std::string getName() const {return _tag.getAttribute(ATT_NAME);}
   
protected:
   XMLTag _tag;
   mutable size_t _count;
   bool _loaded;
};

/**
 Base class for shader resources
 */
class Shader: public Resource {
public:
   Shader(const std::string &name): Resource(name, SHADER_TAG) {}
   
   virtual void setToData(const ShaderData &data) = 0;
   
   virtual void use() const = 0;
   virtual void setUniforms(const Uniforms *unis) const = 0;
   virtual void setAttributes(const Attributes *atts) const = 0;
};

/**
 Base class for textures
 */
struct Texture: public Resource {
   Texture(const std::string &name): Resource(name, TEXTURE_TAG) {}
   
   virtual void setToData(const TextureData &data) = 0;
   virtual void use(int idx) const = 0;
   
   static unsigned int ParseFilters(const std::string &filtStr);
};

/**
 * Base class for meshes
 */
struct Mesh: public Resource {
   Mesh(const std::string &name): Resource(name, MESH_TAG) {}
   
   virtual void setToData(const MeshData &data) = 0;
   
   virtual void use() const = 0;
   virtual void draw() const = 0;
   
   static MeshData* GetPlaneData();
};

/**
 * Base class for frame buffer objects
 */
struct FrameBuff: public Resource {
   FrameBuff(const std::string &name): Resource(name, FBO_TAG) {}
   
   virtual void setToData(const FrameBuffData &data) = 0;
   virtual ivec2 getSize() const = 0;
   virtual void use() const = 0;
};

/**
 * Initally only contains file paths to resources. After loading, this
 * contains resources associated with the file paths.
 */
class Resources: Entity {
public:
   Resources(XMLTag *tag);
   virtual ~Resources();
   
   virtual void notify(const Event &event);
   
private:
   inline void loadResources();
   inline void unloadResources();
   inline void releaseResources();
   
   bool _loaded;
   std::set<Resource*> _resources;
   
   DECLARE_ENTITY_ID(Resources)
};

#endif /* defined(__FelixEngineIOS__Resources__) */
