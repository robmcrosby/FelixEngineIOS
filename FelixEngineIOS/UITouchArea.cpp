//
//  UITouchArea.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/6/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "UITouchArea.h"

using namespace std;

DEFINE_ENTITY_ID(UITouchArea)


UITouchArea::UITouchArea(XMLTag *tag): Entity(tag) {
  cout << "Created UITouchArea: " << getName() << endl;
}

UITouchArea::~UITouchArea() {
  
}