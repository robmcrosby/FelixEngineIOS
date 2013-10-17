//
//  ColorBlock.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 10/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ColorBlock.h"
#include "FelixEngine.h"

using namespace std;

DEFINE_ENTITY_ID(ColorBlock)

/**
 *
 */
ColorBlock::ColorBlock(const XMLTag &tag): UIObject(tag), _shader(0), _mesh(0) {
  applyTag(tag);
  createChildren(tag);
}

/**
 *
 */
ColorBlock::~ColorBlock() {

}

/**
 *
 */
void ColorBlock::draw() const {
  if (isVisible() && _shader && _mesh) {
    // Setup the shader.
    _shader->use();
    _shader->setUniforms(&_uniforms);
    _shader->setUniforms(_transform.getUniforms());

    // Setup the textures.
    for (int i = 0; i < _textures.size(); ++i)
      _textures.at(i)->use(i);

    // Setup and draw the mesh.
    _mesh->use();
    _mesh->draw();
  }
}

/**
 *  Updates the transform of the UIObject.
 */
void ColorBlock::updateUI() {
  _transform.setScale(vec3(getUISize(), 1));
  _transform.setPos(vec3(getUICenter(), 0));

  UIObject::updateUI();
}

/**
 * Gets the pass and layer from the tag.
 */
void ColorBlock::applyTag(const XMLTag &tag) {
  const XMLTag *subtag;

  // add the shader
  subtag = tag.getSubTag(SHADER_TAG);
  if (subtag && subtag->hasAttribute(ATT_NAME))
    _shader = _hostDisplay->getShader(subtag->getAttribute(ATT_NAME));

  // add the mesh
  subtag = tag.getSubTag(MESH_TAG);
  if (subtag && subtag->hasAttribute(ATT_NAME))
    _mesh = _hostDisplay->getMesh(subtag->getAttribute(ATT_NAME));

  // add a texture
  subtag = tag.getSubTag(TEXTURE_TAG);
  if (subtag && subtag->hasAttribute(ATT_NAME))
    _textures.push_back(_hostDisplay->getTexture(subtag->getAttribute(ATT_NAME)));

  // add textures
  subtag = tag.getSubTag(TEXTURES_TAG);
  if (subtag) {
    XMLTag::const_iterator itr;
    for (itr = subtag->begin(); itr != subtag->end(); ++itr) {
      if (**itr == TEXTURE_TAG && (*itr)->hasAttribute(ATT_NAME))
        _textures.push_back(_hostDisplay->getTexture((*itr)->getAttribute(ATT_NAME)));
    }
  }
}