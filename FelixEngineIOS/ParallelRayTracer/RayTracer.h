//
//  RayTracer.h
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__RayTracer__
#define __RayTracer__RayTracer__

#include "Scene.h"
#include "Shader.h"
#include "RenderBuffer.h"

#define DEF_SIZE 1024
#define DEF_MAX_DEPTH 6

class RayTracer {
public:
   RayTracer();
   ~RayTracer();
   
   void setScene(const std::string &scenePath);
   void renderScene();
   void setBufferSize(ivec2 size);
   void clearBuffer(col4 color = col4(0, 0, 0.0f, 1.0f));
   void saveBuffer(const std::string &imagePath);
   void setOptions(const RayOptions &opts);

private:
   Scene *_scene;
   RenderBuffer *_renderBuffer;
};

#endif /* defined(__RayTracer__RayTracer__) */
