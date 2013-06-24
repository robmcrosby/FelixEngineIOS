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

Host* Host::Instance = NULL;

IOSHost::IOSHost(DEV_TYPE dev, ivec2 size, float scale): Host(dev, size, scale) {
   // create the host components
   _display = new OpenGLDisplay(this);
   _audio   = new OpenALAudio(this);
   _fileSys = new IOSFileSystem(this);
   
   createAppEntity();
}

void IOSHost::lowMemory() {
   cout << "LOW MEMORY WARNING" << endl;
   handleEvent(EVENT_MEM_WARNING);
}

void IOSHost::resize(int sizeX, int sizeY, float scale) {
   _size.x = sizeX;
   _size.y = sizeY;
   _scale = scale;
   handleEvent(EVENT_RESIZE);
}

void IOSHost::update(float td) {
   handleEvent(Event(EVENT_UPDATE, &td));
}

void IOSHost::render() {
   handleEvent(EVENT_RENDER);
   _display->drawPass(0);
}

void IOSHost::touchDown(Moves moves) {
   handleEvent(Event(EVENT_TOUCH_DOWN, &moves));
}

void IOSHost::touchUp(Moves moves) {
   handleEvent(Event(EVENT_TOUCH_UP, &moves));
}

void IOSHost::touchMove(Moves moves) {
   handleEvent(Event(EVENT_TOUCH_MOVE, &moves));
}

void IOSHost::createAppEntity() {
   XMLTag *tag = _fileSys->loadXML(APP_CONFIG);
   if (!tag) {
      cerr << "Unable to create main app object" << endl;
      exit(1);
   }
   
   Entity *app = EntityId::CreateEntity(tag, this);
   if (!app) {
      cerr << "Unable to create main app object" << endl;
      delete tag;
      exit(1);
   }
   addChild(app);
}




IOSHost* IOSHost::CreateIOSHost(DEV_TYPE dev, int sizeX, int sizeY, float scale) {
   IOSHost *host;
   
   if (!Instance) {
      host = new IOSHost(dev, ivec2(sizeX, sizeY), scale);
      Instance = host;
   }
   else
      host = dynamic_cast<IOSHost*>(Instance);
   
   return host;
}