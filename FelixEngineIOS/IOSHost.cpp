//
//  IOSHost.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "IOSHost.h"
#include "OpenGLESDisplay.h"
#include "OpenALAudio.h"
#include "IOSFileSystem.h"

using namespace std;


IOSHost::IOSHost(DEV_TYPE dev, ivec2 size, float scale): FelixHost(dev, size, scale) {
   _display = new OpenGLESDisplay(this);
   _audio   = new OpenALAudio(this);
   _fileSys = new IOSFileSystem(this);
}

IOSHost::~IOSHost() {
   delete _display;
   delete _audio;
   delete _fileSys;
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
   handleEvent(FelixEvent(EVENT_UPDATE, &td));
}

void IOSHost::render() {
   handleEvent(EVENT_RENDER);
   _display->drawPasses();
}

void IOSHost::touchDown(Moves moves) {
   handleEvent(FelixEvent(EVENT_TOUCH_DOWN, &moves));
}

void IOSHost::touchUp(Moves moves) {
   handleEvent(FelixEvent(EVENT_TOUCH_UP, &moves));
}

void IOSHost::touchMove(Moves moves) {
   handleEvent(FelixEvent(EVENT_TOUCH_MOVE, &moves));
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