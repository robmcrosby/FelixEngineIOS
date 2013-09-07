//
//  UITouchArea.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/6/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__UITouchArea__
#define __FelixEngineIOS__UITouchArea__

#include "Entity.h"

class UITouchArea: public Entity {
public:
  UITouchArea(XMLTag *tag = NULL);
  virtual ~UITouchArea();
  
private:
  DECLARE_ENTITY_ID(UITouchArea)
};

#endif /* defined(__FelixEngineIOS__UITouchArea__) */
