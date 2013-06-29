//
//  ObjViewer.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ObjViewer.h"

using namespace std;

DEFINE_ENTITY_ID(ObjViewer)

ObjViewer::ObjViewer(XMLTag *tag): Entity(tag) {
   createChildren(_tag);
   
   handleEvent(EVENT_LOAD);
   
   bunny = Entity::GetEntity("bunny");
}

ObjViewer::~ObjViewer() {
   
}


void ObjViewer::handleEvent(const Event &event) {
   if (event == EVENT_UPDATE) {
      float td = *(float*)event.data;
      
      if (bunny)
         bunny->getTransform()->rotate(quat(vec3(0, 1, 0), td));
      
      //ModelMtx *= mat4::RotY(td);
   }
   
   Entity::handleEvent(event);
}



