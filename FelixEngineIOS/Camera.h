//
//  Camera.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Camera__
#define __FelixEngineIOS__Camera__

#include "FelixEngine.h"
#include "Matrix.h"

enum CAM_DEFAULTS {
   CAM_NEAR = 4,
   CAM_SIZE = 2,
   CAM_FAR = 1000,
};

class Camera: public Entity {
public:
   Camera(XMLTag *tag);
   virtual ~Camera();
   
   virtual void notify(const Event &event);
   
protected:
   inline void updateProjMtx();
   inline void updateViewMtx();
   inline void setToPass(const std::string &pass);
   inline void removeFromPass(const std::string &pass);
   
   mat4 _projMtx;
   mat4 _viewMtx;
   
   float _near, _far;
   vec2 _size;
   
   vec3 _pos, _center, _up;
   
   Host *_host;
   HostDisplay *_display;
   
   std::set<std::string> _passes;
   
private:
   DECLARE_ENTITY_ID(Camera)
};

#endif /* defined(__FelixEngineIOS__Camera__) */
