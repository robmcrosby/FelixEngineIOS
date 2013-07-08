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

enum FBO_FLAGS {
   FBO_FIXED_SIZE = 1,
   FBO_COLOR_COMP = 2,
   FBO_DEPTH_COMP = 4,
   FBO_COLOR_TEX = 8,
   FBO_DEPTH_TEX = 16,
   FBO_FINAL = 32,
};

/**
 Data for a shader
 */
struct ShaderData {
   std::string vFile, fFile;
   std::string vSrc, fSrc;
};

/**
 * Data for a texture
 */
struct TextureData {
   TextureData(): data(0) {}
   ~TextureData() {free(data);}
   
   std::string file;
   TEX_TYPE type;
   ivec2 size;
   int pixelSize;
   void *data;
};

/**
 * Data for a mesh
 */
struct MeshData {
   std::string file;
   std::vector<float> vertices;
   std::vector<unsigned int> indices;
   Attributes attributes;
   PRIM_TYPE primType;
};

/**
 Data for a frame buffer object
 */
struct FrameBuffData {
   unsigned int flags;
   vec2 size;
   std::string colorTex;
   std::string depthTex;
};

#endif
