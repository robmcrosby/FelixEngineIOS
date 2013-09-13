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


Object3d::Object3d(XMLTag *tag): Drawable(tag) {
   applyTag();
   _uniforms.addUniform(VAR_TEX_MTX, VAL_MAT3X3, &_textureMtx);
   cout << "Created Object3d" << endl;
}

Object3d::~Object3d() {
   
}

void Object3d::applyTag() {
   if (_tag) {
      const XMLTag *subtag;
      
      //_display = Host::GetHost()->getDisplay();
      
      // add the shader
      subtag = _tag->getSubTag(SHADER_TAG);
      //_shader = subtag ? _display->getShader(subtag->getAttribute("name")) : NULL;
      
      // add the mesh
      subtag = _tag->getSubTag(MESH_TAG);
      //_mesh = subtag ? _display->getMesh(subtag->getAttribute("name")) : NULL;
      
      // add a texture
      subtag = _tag->getSubTag(TEXTURE_TAG);
      //if (subtag)
      //   _textures.push_back(_display->getTexture(subtag->getAttribute("name")));
      
      // add textures
      /*subtag = _tag->getSubTag(TEXTURES_TAG);
      if (subtag) {
         XMLTag::const_iterator itr;
         for (itr = subtag->begin(); itr != subtag->end(); ++itr) {
            if (**itr == TEXTURE_TAG)
               _textures.push_back(_display->getTexture((*itr)->getAttribute("name")));
         }
      }*/
   }
}

void Object3d::addChild(Entity *child) {
   Entity::addChild(child);
   child->setTransformParrent(&_transform);
}

void Object3d::removeChild(Entity *child) {
   Entity::removeChild(child);
   child->setTransformParrent(NULL);
}

void Object3d::draw() const {
  /*
  if (isVisible()) {
      _display->setDrawType(getDrawType());
      _shader->use();
      _display->setCurUniforms(&_uniforms);
      _display->setCurUniforms(_transform.getUniforms());
      for (int i = 0; i < _textures.size(); ++i)
         _textures.at(i)->use(i);
      _mesh->use();
      _mesh->draw();
   }*/
}
