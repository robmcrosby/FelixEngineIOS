//
//  Camera.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Camera.h"

using namespace std;

DEFINE_ENTITY_ID(Camera)

Camera::Camera(XMLTag *tag, Entity *parrent): Entity(tag, parrent) {
   cout << "Created Camera" << endl;
}

Camera::~Camera() {
   
}