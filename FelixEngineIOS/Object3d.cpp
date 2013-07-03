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


Object3d::Object3d(XMLTag *tag): Entity(tag),
_pass(MAIN_PASS), _layer(0), _visible(true), _display(0) {
   applyTag();
   _uniforms.addUniform(VAR_TEX_MTX, VAL_MAT3X3, &_textureMtx);
   cout << "Created Object3d" << endl;
}

Object3d::~Object3d() {
   
}

void Object3d::applyTag() {
   if (_tag) {
      const XMLTag *subtag;
      
      _display = Host::GetHost()->getDisplay();
      
      // add the shader
      subtag = _tag->getSubTag("Shader");
      _shader = subtag ? _display->getShader(subtag->getAttribute("name")) : NULL;
      
      // add the mesh
      subtag = _tag->getSubTag("Mesh");
      _mesh = subtag ? _display->getMesh(subtag->getAttribute("name")) : NULL;
      
      // add a texture
      subtag = _tag->getSubTag("Texture");
      if (subtag)
         _textures.push_back(_display->getTexture(subtag->getAttribute("name")));
      
      // add textures
      subtag = _tag->getSubTag("Textures");
      if (subtag) {
         XMLTag::const_iterator itr;
         for (itr = subtag->begin(); itr != subtag->end(); ++itr) {
            if (**itr == "Texture")
               _textures.push_back(_display->getTexture((*itr)->getAttribute("name")));
         }
      }
      
      // set pass
      if (_tag->hasAttribute("pass"))
         _pass = _tag->getAttribute("pass");
      
      // set the layer
      if (_tag->hasAttribute("layer"))
         sscanf(_tag->getAttribute("layer").c_str(), " %i", &_layer);
   }
}

void Object3d::notify(const Event &event) {
   if (_display && event == EVENT_RENDER && isVisible())
      _display->addToPass(this, _pass);
   Entity::notify(event);
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
   if (_visible) {
      _shader->use();
      _display->setCurUniforms(&_uniforms);
      _display->setCurUniforms(_transform.getUniforms());
      for (int i = 0; i < _textures.size(); ++i)
         _textures.at(i)->use(i);
      _mesh->use();
      _mesh->draw();
   }
}
