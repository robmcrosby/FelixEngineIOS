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
   notify(EVENT_LOAD);
}

void IOSHost::lowMemory() {
   cout << "LOW MEMORY WARNING" << endl;
   notify(EVENT_MEM_WARNING);
}

void IOSHost::resize(int sizeX, int sizeY, float scale) {
   _size.x = sizeX;
   _size.y = sizeY;
   _scale = scale;
   notify(EVENT_RESIZE);
}

void IOSHost::update(float td) {
   notify(Event(EVENT_UPDATE, &td));
}

void IOSHost::render() {
   //_display->emptyPasses();
   notify(EVENT_RENDER);
   //_display->render();
}

void IOSHost::touchDown(const Moves &moves) {
   notify(Event(EVENT_TOUCH_DOWN, &moves));
}

void IOSHost::touchUp(const Moves &moves) {
   notify(Event(EVENT_TOUCH_UP, &moves));
}

void IOSHost::touchMove(const Moves &moves) {
   notify(Event(EVENT_TOUCH_MOVE, &moves));
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
   addListener(app);
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