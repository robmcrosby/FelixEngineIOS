//
//  ImageBuffer.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ImageBuffer.h"
#include "ImageLoader.h"

using namespace std;


bool ImageBuffer::load(const string imagePath) {
   ImageData data;
   int i, j;
   
   if (!ImageLoader::loadImage(&data, imagePath))
      return false;
   
   resize(data.width, data.height);
   for (i = 0; i < _size.x; ++i)
      for (j = 0; j < _size.y; ++j)
         _data[i][j].readPixel(data.pixelAt(i, j));
   return true;
}

bool ImageBuffer::load() {
   return load(_imagePath);
}

bool ImageBuffer::save(const string imagePath) {
   ImageData data;
   int i, j;
   
   /* initalize data */
   data.width = _size.x;
   data.height = _size.y;
   data.allocate();
   
   /* set each pixel */
   for (i = 0; i < _size.x; ++i)
      for (j = 0; j < _size.y; ++j)
         _data[i][j].writePixel(data.pixelAt(i, j));
   
   /* save to file */
   return ImageLoader::saveImage(&data, imagePath);
}

bool ImageBuffer::save() {
   return save(_imagePath);
}

col4 ImageBuffer::getCol4(const vec2 &uv) {
   vec2 t(uv.mod()*(_size-2));
   col4 c1, c2;
   ivec2 i(t);
   ivec2 j(i+1);
   
   j.x = j.x == _size.x ? i.x : j.x;
   j.y = j.y == _size.y ? i.y : j.y;
   
   t -= vec2(i);
   c1 = _data[i.x][i.y].toCol()*(1.0-t.x) + _data[j.x][i.y].toCol()*t.x;
   c2 = _data[i.x][j.y].toCol()*(1.0-t.x) + _data[j.x][j.y].toCol()*t.x;
   return c1*(1.0-t.y) + c2*t.y;
}

col3 ImageBuffer::getCol3(const vec2 &uv) {
   return getCol4(uv).rgb();
}