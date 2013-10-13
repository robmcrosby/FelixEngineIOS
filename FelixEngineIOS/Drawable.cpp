//
//  Drawable.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 9/12/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Drawable.h"
#include "View.h"
#include "FelixEngine.h"

using namespace std;

Drawable::Drawable(const XMLTag &tag): Entity(tag), _passName(MAIN_PASS),
_layer(0), _visible(true), _drawType(DRAW_DEPTH), _drawId(getNewId()),
_curView(0), _shader(0), _mesh(0), _hostDisplay(Host::GetHost()->getDisplay()) {
  _uniforms.addUniform(VAR_VIEW_MTX, VAL_MAT4X4, &_viewMtx);
  _uniforms.addUniform(VAR_TEX_MTX, VAL_MAT3X3, &_textureMtx);
  applyTag(tag);
}

Drawable::~Drawable() {
  
}

/**
 * Sets the visibilty.
 * @param vis A boolean value, true for visible and false for not visible.
 */
void Drawable::setVisiblity(bool vis) {
  if (_curView && _visible != vis) {
    _curView->setChanged();
    _visible = vis;
  }
}

/**
 * Sets how this is drawn.
 * @param type Either a blend or depth DRAW_TYPE.
 */
void Drawable::setDrawType(DRAW_TYPE type) {
  if (_curView && _drawType != type) {
    _curView->setChanged();
    _drawType = type;
  }
}

/**
 * Sets the pass that this is associated with.
 * @param pass A string value for the associated pass.
 */
void Drawable::setPassName(const std::string &pass) {
  if (_curView && _passName != pass) {
    _curView->removeDrawable(this);
    _passName = pass;
    _curView->addDrawable(this);
  }
}

/**
 * Sets the layer value.
 * @param layer An int value for the numbered layer.
 */
void Drawable::setLayer(int layer) {
  if (_curView && _layer != layer) {
    _curView->removeDrawable(this);
    _layer = layer;
    _curView->addDrawable(this);
  }
}

/**
 *
 */
void Drawable::load() {
  View *parrentView = getParrentView();

  // update the current view.
  if (_curView != parrentView) {
    // remove from a previous view.
    if (_curView)
      _curView->removeDrawable(this);

    // add to the parrent view.
    if (parrentView)
      parrentView->addDrawable(this);

    _curView = parrentView;
  }

  Entity::load();
}

/**
 *
 */
void Drawable::unload() {
  if (_curView)
    _curView->removeDrawable(this);
  _curView = NULL;

  Entity::unload();
}

/**
 * Draws this to the screen if it is visible.
 */
void Drawable::draw() const {
  if (isVisible()) {
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
 * Gets the pass and layer from the tag.
 */
void Drawable::applyTag(const XMLTag &tag) {
  const XMLTag *subtag;

  // set pass
  if (tag.hasAttribute("pass"))
    _passName = tag.getAttribute("pass");

  // set the layer
  if (tag.hasAttribute("layer"))
    sscanf(tag.getAttribute("layer").c_str(), " %i", &_layer);

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

int Drawable::getNewId() {
  static int Count = 0;
  
  return (Count++);
}
