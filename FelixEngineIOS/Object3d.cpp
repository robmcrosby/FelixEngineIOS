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
   applyTag(tag);
   _uniforms.addUniform(VAR_TEX_MTX, VAL_MAT3X3, &_textureMtx);
   cout << "Created Object3d" << endl;
}

Object3d::~Object3d() {
   
}

void Object3d::applyTag(const XMLTag &tag) {
  Host *host = Host::GetHost();
  HostDisplay *display = host ? host->getDisplay() : NULL;
  
  if (display) {
    const XMLTag *subtag;
    
    // add the shader
    subtag = tag.getSubTag(SHADER_TAG);
    _shader = subtag ? display->getShader(subtag->getAttribute("name")) : NULL;
    
    // add the mesh
    subtag = tag.getSubTag(MESH_TAG);
    _mesh = subtag ? display->getMesh(subtag->getAttribute("name")) : NULL;
    
    // add a texture
    subtag = tag.getSubTag(TEXTURE_TAG);
    if (subtag)
       _textures.push_back(display->getTexture(subtag->getAttribute("name")));
    
    // add textures
    subtag = tag.getSubTag(TEXTURES_TAG);
    if (subtag) {
      XMLTag::const_iterator itr;
      for (itr = subtag->begin(); itr != subtag->end(); ++itr) {
          if (**itr == TEXTURE_TAG)
            _textures.push_back(display->getTexture((*itr)->getAttribute("name")));
      }
     }
  }
}

void Object3d::draw() const {
  if (isVisible()) {
    _shader->use();
    _shader->setUniforms(&_uniforms);
    _shader->setUniforms(_transform.getUniforms());
    
    for (int i = 0; i < _textures.size(); ++i)
      _textures.at(i)->use(i);
    
    _mesh->use();
    _mesh->draw();
  }
}
