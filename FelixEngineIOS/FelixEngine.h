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
#include "Pipeline.h"
#include "Color.h"

#include "View.h"

/**
 * Defines what device the engine is running on.
 */
enum DEV_TYPE {
  DEV_PHONE, /**< A phone like device. (iphone or ipod touch) */
  DEV_TABLET, /**< A tablet device. */
};


/**
 * Interface for the Host display context
 */
struct HostDisplay {
  virtual ~HostDisplay() {}
   
  virtual void clearContext(Color color = Color()) = 0;
  virtual void setDrawType(DRAW_TYPE type) = 0;
  
  virtual Resource* getResource(const XMLTag &tag) = 0;
  virtual void cleanUpResources() = 0;
  virtual const Shader* getShader(const std::string &name) = 0;
  virtual const Texture* getTexture(const std::string &name) = 0;
  virtual const Mesh* getMesh(const std::string &name) = 0;
  virtual const FrameBuff* getFrameBuff(const std::string &name) = 0;
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
class Host: public Entity {
public:
   
   /**
    * Gets the device type of the Host.
    * @return Enum for the device.
    */
   inline DEV_TYPE getDeviceType()  const {return _device;}
   
   
   
   /**
    * Gets the current screen size of the Host.
    * @return ivec2
    */
   inline ivec2 getScreenSize()  const {return _size;}
   
   /**
    * Gets the scale of the screen. (2.0f for Retina and 1.0f for Non-Retina Displays)
    * @return float scale.
    */
   inline float getScreenScale() const {return _scale;}
   
   
   
   /**
    * Gets the HostDisplay.
    * @return HostDisplay pointer.
    */
   inline HostDisplay *getDisplay() const {return _display;}
   
   /**
    * Gets the HostAudio.
    * @return HostAudio pointer.
    */
   inline HostAudio *getAudio() const {return _audio;}
   
   /**
    * Gets the HostFileSystem.
    * @return HostFileSystem pointer.
    */
   inline HostFileSystem *getFileSystem() const {return _fileSys;}
   
   
   
   /**
    * Gets the resource for the tag from either the Display or Audo Context.
    * @param tag XMLTag that specifies the Resource to get.
    * @return Resource pointer or NULL if it can not be found.
    */
   inline Resource* getResource(const XMLTag &tag) {
      if (tag == SHADER_TAG || tag == TEXTURE_TAG || tag == MESH_TAG || tag == FBO_TAG)
         return _display->getResource(tag);
      else if (tag == SOUND_TAG)
         return _audio->getResource(tag);
      return NULL;
   }
   
   /**
    * Clean up the unused Resources in HostDisplay and HostAudio.
    */
   inline void cleanUpResources() {
      _display->cleanUpResources();
      _audio->cleanUpResources();
   }
   
   /**
    * Gets the Host singlton instance.
    * @return Host pointer.
    */
   static Host* GetHost() {return Instance;}
   
protected:
   
   /**
    * The Host Constructor.
    * @param dev enum for the device.
    * @param size ivec2 of the screen size.
    * @param scale float for the screen scale.
    */
   Host(DEV_TYPE dev, ivec2 size, float scale):
   _device(dev), _size(size), _scale(scale), _display(0), _audio(0), _fileSys(0){
      Instance = this;
   }
   
   /**
    * Destructor deletes the HostDisplay, HostAudio, and HostFileSystem.
    */
   virtual ~Host() {
      delete _display;
      delete _audio;
      delete _fileSys;
   }
   
   DEV_TYPE _device; /**< device type of the host. */
   ivec2 _size; /**< screen size */
   float _scale; /**< screen scale */
   
   HostDisplay *_display; /**< HostDisplay pointer */
   HostAudio *_audio; /**< HostAudio pointer */
   HostFileSystem *_fileSys; /**< HostFileSystem pointer */
   
   static Host *Instance; /**< Host singleton instance */
};


#endif /* defined(__FelixEngineIOS__FelixEngine__) */
