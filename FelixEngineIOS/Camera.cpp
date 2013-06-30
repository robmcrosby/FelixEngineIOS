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

Camera::Camera(XMLTag *tag): Entity(tag),
_near(CAM_NEAR), _far(CAM_FAR), _size(CAM_SIZE) {
   _host = Host::GetHost();
   _display = _host->getDisplay();
   
   updateProjMtx();
   updateViewMtx();
   
   _passes.insert(MAIN_PASS);
}

Camera::~Camera() {
   
}

void Camera::handleEvent(const Event &event) {
   if (event == EVENT_RESIZE)
      updateProjMtx();
   else if (event == EVENT_UPDATE)
      updateViewMtx();
   else if (event == EVENT_LOAD) {
      for (set<string>::iterator pass = _passes.begin(); pass != _passes.end(); ++pass)
         setToPass(*pass);
   }
   else if (event == EVENT_UNLOAD) {
      for (set<string>::iterator pass = _passes.begin(); pass != _passes.end(); ++pass)
         removeFromPass(*pass);
   }
   
   Entity::handleEvent(event);
}

void Camera::updateProjMtx() {
   vec2 screen = _host->getScreenSize();
   
   if (screen.x < screen.y) {
      float w = 4.0f * screen.x / screen.y;
      _projMtx = mat4::Frustum(-w/_size.x, w/_size.x, -_size.y, _size.y, _near, _far);
   }
   else {
      float h = 4.0f * screen.y / screen.x;
      _projMtx = mat4::Frustum(-_size.x, _size.x, -h/_size.y, h/_size.y, _near, _far);
   }
}

void Camera::updateViewMtx() {
   _viewMtx = mat4::LookAt(vec3(0, 0, 20), vec3(0, 0, 0), vec3(0, 1, 0));
}

void Camera::setToPass(const string &pass) {
   _display->addPassUniform(VAR_PROJ_MTX, Uniform(VAL_MAT4X4, &_projMtx), pass);
   _display->addPassUniform(VAR_VIEW_MTX, Uniform(VAL_MAT4X4, &_viewMtx), pass);
}

void Camera::removeFromPass(const string &pass) {
   _display->removePassUniform(VAR_PROJ_MTX, pass);
   _display->removePassUniform(VAR_VIEW_MTX, pass);
}