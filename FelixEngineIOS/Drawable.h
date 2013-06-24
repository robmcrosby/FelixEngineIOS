//
//  Drawable.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_Drawable_h
#define FelixEngineIOS_Drawable_h

#include "Entity.h"
#include "Resources.h"

/**
 * Base class for all Drawable Felix Entities
 */
class Drawable: Entity {
public:
   Drawable(): _pass(0), _layer(0), _visible(true), _drawType(DRAW_BLEND) {}
   virtual ~Drawable() {}
   
   virtual void handleEvent(const Event &event) {
      Entity::handleEvent(event);
      //if (event == EVENT_RENDER && _visible)
   }
   
   inline void draw() {
      if (_visible) {
         _shader->use();
         _shader->setUniforms(_uniforms);
         for (int i = 0; i < _textures.size(); ++i)
            _textures.at(i)->use(i);
         _mesh->use();
         _mesh->draw();
      }
   }
   inline void setPass(int pass)  {_pass = pass;}
   inline void setLayer(int layer) {_layer = layer;}
   inline void setVisiblity(bool v) {_visible = v;}
   
   inline int getPass()  const {return _pass;}
   inline int getLayer() const {return _layer;}
   
   inline void setDrawType(DRAW_TYPE type) {_drawType = type;}
   inline DRAW_TYPE getDrawType() const {return _drawType;}
   
protected:
   int _pass;
   int _layer;
   bool _visible;
   DRAW_TYPE _drawType;
   Uniforms _uniforms;
   
   const Shader *_shader;
   const Mesh *_mesh;
   std::vector<const Texture*> _textures;
   
};

#endif
