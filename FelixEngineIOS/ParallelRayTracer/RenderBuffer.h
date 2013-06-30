//
//  RenderBuffer.h
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__RenderBuffer__
#define __RayTracer__RenderBuffer__

#include "Utils/Buffer.h"

class RenderBuffer: public Buffer2D<col4> {
public:
   RenderBuffer(ivec2 size, col4 color = col4(0, 0, 0, 1.0f)): Buffer2D<col4>(size, color) {}
   virtual ~RenderBuffer() {}
   
   bool save(const std::string imagePath);
};

#endif /* defined(__RayTracer__RenderBuffer__) */
