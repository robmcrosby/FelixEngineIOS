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
_near(CAM_NEAR), _far(CAM_FAR), _size(CAM_SIZE), _abs(0) {
   _host = Host::GetHost();
   _display = _host->getDisplay();
   
   applyTag();
   
   updateProjMtx();
   updateViewMtx();
}

Camera::~Camera() {
   
}

/*
void Camera::notify(const Event &event) {
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
   
   Entity::notify(event);
}*/

void Camera::updateProjMtx() {
  /*
   vec2 screen = _abs ? vec2(1.0f) : vec2(_host->getScreenSize());
   
   if (screen.x < screen.y) {
      float w = 4.0f * screen.x / screen.y;
      _projMtx = mat4::Frustum(-w/_size.x, w/_size.x, -_size.y, _size.y, _near, _far);
   }
   else {
      float h = 4.0f * screen.y / screen.x;
      _projMtx = mat4::Frustum(-_size.x, _size.x, -h/_size.y, h/_size.y, _near, _far);
   }*/
}

void Camera::updateViewMtx() {
   _viewMtx = mat4::LookAt(_pos, _target, _up);
}

void Camera::setToPass(const string &pass) {
   //_display->addPassUniform(VAR_PROJ_MTX, Uniform(VAL_MAT4X4, &_projMtx), pass);
   //_display->addPassUniform(VAR_VIEW_MTX, Uniform(VAL_MAT4X4, &_viewMtx), pass);
}

void Camera::removeFromPass(const string &pass) {
   //_display->removePassUniform(VAR_PROJ_MTX, pass);
   //_display->removePassUniform(VAR_VIEW_MTX, pass);
}

void Camera::applyTag() {
   if (_tag) {
      XMLTag *subTag;
      
      // get position
      subTag = _tag->getSubTag("pos");
      _pos = subTag ? vec3::ParseFloat(subTag->getContents()) : vec3(0, 0, 10);
      
      // get target
      subTag = _tag->getSubTag("target");
      _target = subTag ? vec3::ParseFloat(subTag->getContents()) : vec3(0);
      
      // get up vector
      subTag = _tag->getSubTag("up");
      _up = subTag ? vec3::ParseFloat(subTag->getContents()) : vec3(0);
      
      // get pass or passes
      subTag = _tag->getSubTag("passes");
      if (subTag) {
         XMLTag::iterator itr;
         for (itr = subTag->begin(); itr != subTag->end(); ++itr)
            _passes.insert((*itr)->getContents());
      }
      else {
         subTag = _tag->getSubTag("pass");
         if (subTag)
            _passes.insert(subTag->getContents());
      }
      
      // get flags
      _abs = _tag->hasAttribute("flags");
   }
}