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
#include "Entity.h"

class IOSHost: public Entity, public Host {
public:
  IOSHost(DEV_TYPE dev, ivec2 size, float scale);
  virtual ~IOSHost();
   
  virtual void lowMemory();
  virtual void update(float td);

  virtual bool touchesBegin(const Touches &touches);
  virtual bool touchesEnd(const Touches &touches);
  virtual bool touchesMove(const Touches &touches);
  virtual void setTouchDeligate(TouchDeligate *deligate);

  virtual DEV_TYPE getDeviceType() const;
  virtual HostDisplay *getDisplay() const;
  virtual HostAudio *getAudio() const;
  virtual HostFileSystem *getFileSystem() const;
  virtual Resource* getResource(const XMLTag &tag);
  virtual void cleanUpResources();
  
  static IOSHost* GetIOSHost();
  static IOSHost* CreateIOSHost(DEV_TYPE dev, int sizeX, int sizeY, float scale);
   
private:
  inline void createAppEntity();

  DEV_TYPE _device; /**< device type of the host. */
  
  HostDisplay *_display; /**< HostDisplay pointer */
  HostAudio *_audio; /**< HostAudio pointer */
  HostFileSystem *_fileSys; /**< HostFileSystem pointer */

  TouchDeligate *_touchDeligate;
  
  static IOSHost *Instance; /**< IOSHost singleton instance */
};

#endif /* defined(__FelixEngineIOS__IOSHost__) */
