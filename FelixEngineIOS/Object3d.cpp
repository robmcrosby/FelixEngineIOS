//
//  Object3d.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "FelixEngine.h"
#include "Object3d.h"

using namespace std;

DEFINE_ENTITY_ID(Object3d)


Object3d::Object3d(const XMLTag &tag): Drawable(tag) {
  _viewMtx = mat4::Scale(vec4(10.0f, 20.0f, 1.0f, 1.0f));
}

Object3d::~Object3d() {

}
