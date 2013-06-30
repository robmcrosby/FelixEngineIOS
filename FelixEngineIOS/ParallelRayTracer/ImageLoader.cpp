//
//  ImageLoader.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ImageLoader.h"
#include "TgaImageLoader.h"

ImageLoader* ImageLoader::CurLoader = new TgaImageLoader();


bool ImageLoader::loadImage(ImageData *data, const std::string &file) {
   if (CurLoader) {
      return CurLoader->loadFile(data, file);
   }
   return false;
}

bool ImageLoader::saveImage(ImageData *data, const std::string &file) {
   if (CurLoader) {
      return CurLoader->saveFile(data, file);
   }
   return false;
}