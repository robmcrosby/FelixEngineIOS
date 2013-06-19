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

ObjViewer::ObjViewer(XMLTag *tag, FelixEntity *parent): FelixEntity(tag, parent) {
   cout << "It works!" << endl;
}

ObjViewer::~ObjViewer() {
   
}