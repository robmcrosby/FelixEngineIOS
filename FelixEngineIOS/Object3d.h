//
//  Object3d.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__Object3d__
#define __FelixEngineIOS__Object3d__

#include "FelixEngine.h"


class Object3d: public Entity, Drawable {
public:
   Object3d(XMLTag *tag);
   virtual ~Object3d();
   
   virtual void notify(const Event &event);
   virtual void addChild(Entity *child);
   virtual void removeChild(Entity *child) ;
   
   virtual bool isVisible() const {return _visible;}
   virtual DRAW_TYPE getDrawType() const {return _drawType;}
   virtual std::string getPass() const {return _pass;}
   virtual int getLayer() const {return _layer;}
   
   virtual void setDrawType(DRAW_TYPE type) {_drawType = type;}
   virtual void setVisiblity(bool vis) {_visible = vis;}
   virtual void setPass(const std::string &pass) {_pass = pass;}
   virtual void setLayer(int layer) {_layer = layer;}
   
   virtual void draw() const;
   
protected:
   int _layer;
   bool _visible;
   std::string _pass;
   DRAW_TYPE _drawType;
   mat3 _textureMtx;
   Uniforms _uniforms;
   
   const Shader *_shader;
   const Mesh *_mesh;
   std::vector<const Texture*> _textures;
   
   HostDisplay *_display;
   
private:
   inline void applyTag();
   
   DECLARE_ENTITY_ID(Object3d)
};

#endif /* defined(__FelixEngineIOS__Object3d__) */
