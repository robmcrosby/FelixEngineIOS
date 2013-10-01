//
//  FelixEngine.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__FelixEngine__
#define __FelixEngineIOS__FelixEngine__

#include "UIElement.h"
#include "Resources.h"

/**
 * Defines what device the engine is running on.
 */
enum DEV_TYPE {
  DEV_PHONE, /**< Phone like device. (iphone or ipod touch) */
  DEV_TABLET, /**< Tablet Device. (ipad) */
};


/**
 * Interface for the Host display context
 */
struct HostDisplay {
  virtual ~HostDisplay() {}

  virtual void resize(int sizeX, int sizeY, float scale) = 0;
  virtual ivec2 getScreenSize() const = 0;
  virtual float getScreenScale() const = 0;

  virtual Resource* getResource(const XMLTag &tag) = 0;
  virtual void updateResources() = 0;
  virtual const Shader* getShader(const std::string &name) = 0;
  virtual const Texture* getTexture(const std::string &name) = 0;
  virtual const Mesh* getMesh(const std::string &name) = 0;
  virtual const FrameBuff* getFrameBuff(const std::string &name) = 0;
  virtual void updateFrameBuffs() = 0;
  virtual void setResourceData(const ResourceData *data) = 0;
};



/**
 * An interface for working with a host audio context.
 */
struct HostAudio {
  virtual ~HostAudio() {}
  
  virtual Resource* getResource(const XMLTag &tag) = 0;
  virtual void cleanUpResources() = 0;
};



/**
 * An interface for working with the host file system
 */
struct HostFileSystem {
  virtual ~HostFileSystem() {}
  
  virtual std::string loadTxt(const std::string &path) const = 0;
  virtual XMLTag* loadXML(const std::string &path) const = 0;
  virtual TextureData* loadTexture(const std::string &path) const = 0;
  virtual bool loadMeshes(const std::string &path, std::map<std::string, MeshData*> *data) const = 0;
};


/**
 * Base class that represents the host system.
 */
struct Host: UIElement {
  virtual ~Host() {}

  virtual void lowMemory() = 0;
  virtual void update(float td) = 0;
  virtual void render() = 0;
  
  virtual DEV_TYPE getDeviceType() const = 0;
  virtual HostDisplay *getDisplay() const = 0;
  virtual HostAudio *getAudio() const = 0;
  virtual HostFileSystem *getFileSystem() const = 0;
  virtual Resource* getResource(const XMLTag &tag) = 0;
  virtual void cleanUpResources() = 0;
  
  static Host* GetHost();
};


#endif /* defined(__FelixEngineIOS__FelixEngine__) */
