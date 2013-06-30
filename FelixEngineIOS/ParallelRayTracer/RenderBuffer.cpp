//
//  RenderBuffer.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "RenderBuffer.h"
#include "ImageLoader.h"

using namespace std;


bool RenderBuffer::save(const std::string imagePath) {
   ImageData data;
   int i, j;
   
   /* initalize data */
   data.width = _size.x;
   data.height = _size.y;
   data.allocate();
   
   /* set each pixel */
   for (i = 0; i < _size.x; ++i)
      for (j = 0; j < _size.y; ++j)
         _data[i][j].toPix().writePixel(data.pixelAt(i, j));
   
   /* save to file */
   return ImageLoader::saveImage(&data, imagePath);
}
