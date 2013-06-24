//
//  ResourceData.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/23/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_ResourceData_h
#define FelixEngineIOS_ResourceData_h

#include "ShaderVaribles.h"

enum TEX_TYPE {
   TEX_GRAY,
   TEX_GRAY_ALPHA,
   TEX_RGB,
   TEX_RGBA,
};

enum PRIM_TYPE {
   PRIM_TRI,
   PRIM_TRI_STRIP,
};

/**
 * Data for a texture
 */
struct TextureData {
   TextureData(): data(0) {}
   ~TextureData() {free(data);}
   
   TEX_TYPE type;
   ivec2 size;
   int pixelSize;
   void *data;
};

/**
 * Data for a mesh
 */
struct MeshData {
   std::vector<float> vertices;
   std::vector<unsigned int> indices;
   Attributes attributes;
   PRIM_TYPE primType;
};

#endif
