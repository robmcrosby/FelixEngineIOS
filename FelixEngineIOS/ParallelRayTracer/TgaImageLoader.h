//
//  TgaImageLoader.h
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__TgaImageLoader__
#define __RayTracer__TgaImageLoader__

#include <iostream>
#include "ImageLoader.h"

class TgaImageLoader: public ImageLoader {
public:
   virtual bool loadFile(ImageData *data, const std::string &file);
   virtual bool saveFile(ImageData *data, const std::string &file);
   
private:
   struct TgaHeader {
      TgaHeader();
      bool read(std::ifstream &is);
      void write(std::ofstream &os);
      
      char idLength;
      char colorMapType;
      char dataTypeCode;
      short colorMapOrigin;
      short colorMapLength;
      char colorMapDepth;
      short xOrigin;
      short yOrigin;
      short width;
      short height;
      char bitsPerPixel;
      char imageDescriptor;
   };
   
   inline bool readUncompressed(ImageData *data, std::ifstream &is, int bpp);
   inline bool readCompressed(ImageData *data, std::ifstream &is, int bpp);
   inline bool writeUncompressed(ImageData *data, std::ofstream &os);
};

#endif /* defined(__RayTracer__TgaImageLoader__) */
