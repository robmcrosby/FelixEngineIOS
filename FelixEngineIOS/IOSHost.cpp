//
//  IOSHost.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "IOSHost.h"
#include "OpenGLDisplay.h"
#include "OpenALAudio.h"
#include "IOSFileSystem.h"

using namespace std;


IOSHost* IOSHost::Instance = NULL;



/**
 * Gets the Host singlton instance.
 * @return Host pointer.
 */
Host* Host::GetHost() {
  return IOSHost::GetIOSHost();
}

/**
 * Gets the IOSHost singlton instance.
 * @return IOSHost pointer.
 */
IOSHost* IOSHost::GetIOSHost() {
  return Instance;
}

IOSHost* IOSHost::CreateIOSHost(DEV_TYPE dev, int sizeX, int sizeY, float scale) {
  if (!Instance)
    Instance = new IOSHost(dev, ivec2(sizeX, sizeY), scale);
  
  return Instance;
}


/**
 * The IOSHost Constructor.
 * @param dev enum for the device.
 * @param size ivec2 of the screen size.
 * @param scale float for the screen scale.
 */
IOSHost::IOSHost(DEV_TYPE dev, ivec2 size, float scale): _device(dev) {
  Instance = this;

  // The Host Display is also the Root View and the Touch Deligate.
  OpenGLDisplay *display = new OpenGLDisplay(size, scale);
  display->addChild(this);

  // create the host components
  _display = display;
  _audio   = new OpenALAudio(this);
  _fileSys = new IOSFileSystem(this);
  
  createAppEntity();
  _display->updateResources();

  load();
}

/**
 * Destructor deletes the HostDisplay, HostAudio, and HostFileSystem.
 */
IOSHost::~IOSHost() {
  delete _display;
  delete _audio;
  delete _fileSys;
}


void IOSHost::lowMemory() {
  cout << "LOW MEMORY WARNING" << endl;
}

void IOSHost::update(float td) {
  Entity::update(td);
}

bool IOSHost::touchesBegin(const Touches &touches) {
  return _display->touchesBegin(touches);
}

bool IOSHost::touchesEnd(const Touches &touches) {
  return _display->touchesEnd(touches);
}

bool IOSHost::touchesMove(const Touches &touches) {
  return _display->touchesMove(touches);
}

void IOSHost::createAppEntity() {
   XMLTag *tag = _fileSys->loadXML(APP_CONFIG);
   if (!tag) {
      cerr << "Unable to create main app object" << endl;
      exit(1);
   }
   
   Entity *app = EntityId::CreateEntity(tag);
   if (!app) {
      cerr << "Unable to create main app object" << endl;
      delete tag;
      exit(1);
   }
   addChild(app);
}



/**
 * Gets the device type of the Host.
 * @return Enum for the device.
 */
DEV_TYPE IOSHost::getDeviceType()  const {
  return _device;
}

/**
 * Gets the HostDisplay.
 * @return HostDisplay pointer.
 */
HostDisplay* IOSHost::getDisplay() const {
  return _display;
}

/**
 * Gets the HostAudio.
 * @return HostAudio pointer.
 */
HostAudio* IOSHost::getAudio() const {
  return _audio;
}

/**
 * Gets the HostFileSystem.
 * @return HostFileSystem pointer.
 */
HostFileSystem* IOSHost::getFileSystem() const {
  return _fileSys;
}



/**
 * Gets the resource for the tag from either the Display or Audo Context.
 * @param tag XMLTag that specifies the Resource to get.
 * @return Resource pointer or NULL if it can not be found.
 */
Resource* IOSHost::getResource(const XMLTag &tag) {
  if (tag == SHADER_TAG || tag == TEXTURE_TAG || tag == MESH_TAG || tag == FBO_TAG)
    return _display->getResource(tag);
  else if (tag == SOUND_TAG)
    return _audio->getResource(tag);
  return NULL;
}

/**
 * Clean up the unused Resources in HostDisplay and HostAudio.
 */
void IOSHost::cleanUpResources() {
  //_display->cleanUpResources();
  //_audio->cleanUpResources();
}