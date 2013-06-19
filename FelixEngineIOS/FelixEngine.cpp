//
//  FelixEngine.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "FelixEngine.h"

using namespace std;

FelixHost* FelixHost::Instance = NULL;
EntityId* EntityId::IDs;


FelixHost::FelixHost(DEV_TYPE dev, ivec2 size, float scale):
_device(dev), _size(size), _scale(scale), _display(0), _audio(0), _fileSys(0), _app(0) {
   Instance = this;
}

FelixHost::~FelixHost() {
   delete _display;
   delete _audio;
   delete _fileSys;
   delete _app;
}

void FelixHost::createAppEntity() {
   XMLTag *tag = _fileSys->loadXML(APP_CONFIG);
   if (!tag) {
      cerr << "Unable to create main app object" << endl;
      exit(1);
   }
   
   _app = EntityId::CreateEntity(tag, this);
   if (!_app) {
      cerr << "Unable to create main app object" << endl;
      delete tag;
      exit(1);
   }
}


FelixEntity* EntityId::CreateEntity(XMLTag *tag, FelixEntity *parrent) {
   EntityId *curId = IDs;
   
   while (curId) {
      if (curId->_idStr == tag->getElement())
         return curId->create(tag, parrent);
      curId = curId->_next;
   }
    
   cerr << "could not find entity: " << tag->getElement() << endl;
   return NULL;
}

EntityId::EntityId(const string &idStr): _idStr(idStr) {
   _next = IDs;
   IDs = this;
}
