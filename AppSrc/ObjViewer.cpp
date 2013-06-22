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

ObjViewer::ObjViewer(XMLTag *tag, Entity *parent): Entity(tag, parent) {
   createChildren(_tag);
   cout << "It works!" << endl;
   
   handleEvent(EVENT_LOAD);
}

ObjViewer::~ObjViewer() {
   
}