//
//  ColorBlock.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 10/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__ColorBlock__
#define __FelixEngineIOS__ColorBlock__

#include "UIObject.h"

class ColorBlock: public UIObject {
public:
  ColorBlock(const XMLTag &tag);
  virtual ~ColorBlock();

  virtual void draw() const;

protected:
  virtual void updateUI();

private:
  inline void applyTag(const XMLTag &tag);

  const Shader *_shader;
  const Mesh   *_mesh;
  std::vector<const Texture*> _textures;

  DECLARE_ENTITY_ID(ColorBlock)
};

#endif /* defined(__FelixEngineIOS__ColorBlock__) */
