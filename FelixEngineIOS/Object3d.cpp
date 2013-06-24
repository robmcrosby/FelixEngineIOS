//
//  Object3d.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Object3d.h"

using namespace std;

DEFINE_ENTITY_ID(Object3d)


Object3d::Object3d(XMLTag *tag, Entity *parrent): Drawable(tag, parrent) {
   cout << "Created Object3d" << endl;
}

Object3d::~Object3d() {
   
}