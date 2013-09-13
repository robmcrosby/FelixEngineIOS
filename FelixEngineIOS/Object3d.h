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


class Object3d: public Drawable {
public:
   Object3d(XMLTag *tag);
   virtual ~Object3d();
   
   virtual void addChild(Entity *child);
   virtual void removeChild(Entity *child) ;
   
   virtual void draw() const;
   
protected:
   mat3 _textureMtx;
   Uniforms _uniforms;
   
   const Shader *_shader;
   const Mesh *_mesh;
   std::vector<const Texture*> _textures;
   
private:
   inline void applyTag();
   
   DECLARE_ENTITY_ID(Object3d)
};

#endif /* defined(__FelixEngineIOS__Object3d__) */
