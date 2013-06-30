//
//  ImageBuffer.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__ImageBuffer__
#define __ParallelRayTracer__ImageBuffer__

#include "Utils/Buffer.h"

class ImageBuffer: public Buffer2D<pix4> {
public:
   ImageBuffer(const std::string &imagePath): Buffer2D<pix4>(32), _imagePath(imagePath) {}
   virtual ~ImageBuffer() {}
   
   virtual bool load(const std::string imagePath);
   virtual bool load();
   virtual bool save(const std::string imagePath);
   virtual bool save();
   
   virtual col4 getCol4(const vec2 &uv);
   virtual col3 getCol3(const vec2 &uv);
   
protected:
   std::string _imagePath;
};

#endif /* defined(__ParallelRayTracer__ImageBuffer__) */
