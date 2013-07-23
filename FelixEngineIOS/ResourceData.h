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

enum TEX_FORMAT {
   TEX_GRAY,
   TEX_GRAY_ALPHA,
   TEX_RGB,
   TEX_RGBA,
   TEX_DEPTH,
};

enum FILT_TYPE {
   FILT_MIN_NEAR = 1,
   FILT_MIN_LINE = 2,
   FILT_MAG_NEAR = 4,
   FILT_MAG_LINE = 8,
   FILT_MIP_NEAR = 16,
   FILT_MIP_LINE = 32,
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
 Data base struct
 */
struct ResourceData {
   virtual ~ResourceData() {}
   
   std::string targetName;
};

/**
 Data for a shader
 */
struct ShaderData: public ResourceData {
   virtual ~ShaderData() {}
   
   std::string vFile, fFile;
   std::string vSrc, fSrc;
};

/**
 * Data for a texture
 */
struct TextureData: public ResourceData {
   TextureData(): data(0), filters(0) {}
   virtual ~TextureData() {free(data);}
   
   std::string file;
   unsigned int filters;
   TEX_FORMAT format;
   ivec2 size;
   int pixelSize;
   void *data;
};

/**
 * Data for a mesh
 */
struct MeshData: public ResourceData {
   virtual ~MeshData() {}
   
   std::string file;
   std::vector<float> vertices;
   std::vector<unsigned int> indices;
   Attributes attributes;
   PRIM_TYPE primType;
};

/**
 Data for a frame buffer object
 */
struct FrameBuffData: public ResourceData {
   virtual ~FrameBuffData() {}
   
   unsigned int flags;
   unsigned int filters;
   vec2 size;
   std::string colorTex;
   std::string depthTex;
};

#endif
