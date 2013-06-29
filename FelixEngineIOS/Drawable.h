//
//  Drawable.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_Drawable_h
#define FelixEngineIOS_Drawable_h

#include "FelixEngine.h"

/**
 * Base class for all Drawable Felix Entities
 */
class Drawable: public Entity {
public:
   Drawable(XMLTag *tag): Entity(tag), _visible(true) {
      setResources();
      
      _pass = MAIN_PASS;
      _layer = 0;
      _uniforms.addUniform(VAR_TEX_MTX, VAL_MAT3X3, &_textureMtx);
   }
   virtual ~Drawable() {}
   
   virtual void handleEvent(const Event &event) {
      if (event == EVENT_RENDER && isVisible())
         _display->addToPass(this, _pass);
      Entity::handleEvent(event);
   }
   
   virtual void addChild(Entity *child) {
      Entity::addChild(child);
      child->setTransformParrent(&_transform);
   }
   
   virtual void removeChild(Entity *child) {
      Entity::removeChild(child);
      child->setTransformParrent(NULL);
   }
   
   inline void draw() const {
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
   
   inline void setPass(int pass)  {_pass = pass;}
   inline void setLayer(int layer) {_layer = layer;}
   inline void setVisiblity(bool v) {_visible = v;}
   
   inline std::string getPass() const {return _pass;}
   inline int getLayer() const {return _layer;}
   inline bool isVisible() const {return _visible && _shader && _mesh;}
   
   inline void setDrawType(DRAW_TYPE type) {_drawType = type;}
   inline DRAW_TYPE getDrawType() const {return _drawType;}
   
protected:
   inline void setResources() {
      XMLTag *subtag;
      
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
         for (XMLTag::iterator itr = subtag->begin(); itr != subtag->end(); ++itr) {
            if (**itr == "Texture")
               _textures.push_back(_display->getTexture((*itr)->getAttribute("name")));
         }
      }
   }
   
   /* fields */
   std::string _pass;
   int _layer;
   bool _visible;
   DRAW_TYPE _drawType;
   mat3 _textureMtx;
   Uniforms _uniforms;
   
   const Shader *_shader;
   const Mesh *_mesh;
   std::vector<const Texture*> _textures;
   
   HostDisplay *_display;
};

#endif
