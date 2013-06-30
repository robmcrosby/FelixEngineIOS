//
//  ImageLoader.h
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef RayTracer_ImageLoader_h
#define RayTracer_ImageLoader_h

#include <iostream>
#include <string>

struct ImageData {
   ImageData(): data(0) {}
   ~ImageData() {delete [] data;}
   
   bool allocate() {
      data = new unsigned char [width * height * 4];
      return true;
   }
   
   void setPixel(unsigned char *buff, int idx, int bpp) {
      unsigned char *loc = data + idx * 4;
      if (bpp == 4 || bpp == 3) {
         loc[0] = buff[2];
         loc[1] = buff[1];
         loc[2] = buff[0];
         loc[3] = bpp == 4 ? buff[3] : 255;
      }
      else if (bpp == 2) {
         loc[0] = (buff[1] & 0x7c) << 1;
         loc[1] = ((buff[1] & 0x03) << 6) | ((buff[0] & 0xe0) >> 2);
         loc[2] = (buff[0] & 0x1f) << 3;
         loc[3] = (buff[1] & 0x80);
      }
   }
   
   void getPixel(unsigned char *buff, int idx) {
      unsigned char *loc = data + idx * 4;
      buff[2] = loc[0];
      buff[1] = loc[1];
      buff[0] = loc[2];
      buff[3] = loc[3];
   }
   
   unsigned char *pixelAt(int w, int h) {
      return data + (w + h * width) * 4;
   }
   
   unsigned char *data;
   int width;
   int height;
};

struct ImageLoader {
   virtual bool loadFile(ImageData *data, const std::string &file) = 0;
   virtual bool saveFile(ImageData *data, const std::string &file) = 0;
   
   static bool loadImage(ImageData *data, const std::string &file);
   static bool saveImage(ImageData *data, const std::string &file);
   
   static ImageLoader *CurLoader;
};

#endif
