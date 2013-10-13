//
//  Object3d.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Object3d__
#define __FelixEngineIOS__Object3d__

#include "FelixEngine.h"
#include "Drawable.h"
#include "Transform.h"


class Object3d: public Drawable {
public:
  Object3d(const XMLTag &tag);
  virtual ~Object3d();

private:
  DECLARE_ENTITY_ID(Object3d)
};

#endif /* defined(__FelixEngineIOS__Object3d__) */
