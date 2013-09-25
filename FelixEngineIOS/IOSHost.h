//
//  IOSHost.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__IOSHost__
#define __FelixEngineIOS__IOSHost__

#include "FelixEngine.h"
#include "View.h"

class OpenGLFrameBuff;

class IOSHost: public View, public Host {
public:
  IOSHost(DEV_TYPE dev, ivec2 size, float scale);
  virtual ~IOSHost();
   
  void lowMemory();
  void resize(int sizeX, int sizeY, float scale);
  void update(float td);
  void render();

  void touchDown(const Moves &moves);
  void touchUp(const Moves &moves);
  void touchMove(const Moves &moves);
  
  virtual DEV_TYPE getDeviceType() const;
  virtual ivec2 getScreenSize() const;
  virtual float getScreenScale() const;
  virtual HostDisplay *getDisplay() const;
  virtual HostAudio *getAudio() const;
  virtual HostFileSystem *getFileSystem() const;
  virtual Resource* getResource(const XMLTag &tag);
  virtual void cleanUpResources();
  
  static IOSHost* GetIOSHost();
  static IOSHost* CreateIOSHost(DEV_TYPE dev, int sizeX, int sizeY, float scale);
   
private:
  inline void createAppEntity();
  
  OpenGLFrameBuff *_finalFbo;
  DEV_TYPE _device; /**< device type of the host. */
  ivec2 _size; /**< screen size */
  float _scale; /**< screen scale */
  
  HostDisplay *_display; /**< HostDisplay pointer */
  HostAudio *_audio; /**< HostAudio pointer */
  HostFileSystem *_fileSys; /**< HostFileSystem pointer */
  
  static IOSHost *Instance; /**< IOSHost singleton instance */
};

#endif /* defined(__FelixEngineIOS__IOSHost__) */
