//
//  FelixEngine.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__FelixEngine__
#define __FelixEngineIOS__FelixEngine__

#include "Resources.h"

#define APP_CONFIG "Config/main.xml"

class TextureData;
class MeshData;

enum DEV_TYPE {
   DEV_PHONE,
   DEV_TABLET,
};

/**
 * Base class for working with a host display context
 */
class HostDisplay: public Entity {
public:
   virtual ~HostDisplay() {}
   virtual void drawPasses() = 0;
   
   virtual Resource* getResource(const XMLTag &tag) = 0;
   virtual const Shader* getShader(const std::string &name) = 0;
   virtual const Texture* getTexture(const std::string &name) = 0;
   virtual const Mesh* getMesh(const std::string &name) = 0;
   
   virtual void setCurShader(const Shader *sh) = 0;
   virtual void setCurUniforms(const Uniforms &uniforms) = 0;
   virtual void setCurAttributes(const Attributes &attributes) = 0;
   
   virtual void addPassUniform(const std::string &name, const Uniform &uniform, int pass) = 0;
   virtual void clearPassUniforms(int pass) = 0;
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
   virtual TextureData* loadTexture(const std::string &path) const = 0;
   virtual bool loadMeshes(const std::string &path, std::map<std::string, MeshData*> *data) const = 0;
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
   Host(DEV_TYPE dev, ivec2 size, float scale):
   _device(dev), _size(size), _scale(scale), _display(0), _audio(0), _fileSys(0){
      Instance = this;
   }
   virtual ~Host() {
      delete _display;
      delete _audio;
      delete _fileSys;
   }
   
   DEV_TYPE _device;
   ivec2 _size;
   float _scale;
   
   HostDisplay *_display;
   HostAudio *_audio;
   HostFileSystem *_fileSys;
   
   static Host *Instance;
};


#endif /* defined(__FelixEngineIOS__FelixEngine__) */
