//
//  TgaImageLoader.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include <fstream>
#include "TgaImageLoader.h"

using namespace std;


bool TgaImageLoader::loadFile(ImageData *data, const std::string &file) {
   TgaHeader header;
   ifstream is(file.c_str(), ios::in | ios::binary);
   
   if (!is.is_open()) {
      cerr << "Error opening file: " << file << endl;
      return false;
   }
   
   if (!header.read(is)) {
      cerr << "Error reading header for file: " << file << endl;
      is.close();
      return false;
   }
   
   if (header.dataTypeCode != 2 && header.dataTypeCode !=10) {
      cerr << "Can only read image types 2 and 10" << endl;
      cerr << "File: " << file << " has type: " << header.dataTypeCode << endl;
      is.close();
      return false;
   }
   
   if (header.bitsPerPixel != 16 && header.bitsPerPixel != 24 && header.bitsPerPixel != 32) {
      cerr << "Can only read pixel depths of 16, 24, and 32" << endl;
      cerr << "File: " << file << " has a pixel depth of: " << header.bitsPerPixel << endl;
      is.close();
      return false;
   }
   
   if (header.colorMapType != 0 && header.colorMapType != 1) {
      cerr << "Can only read color map types 0 and 1" << endl;
      cerr << "File: " << file << " has type: " << header.colorMapType << endl;
      is.close();
      return false;
   }
   
   // set the data by the info in header.
   data->width = header.width;
   data->height = header.height;
   data->allocate();
   
   // skip over unnecessary stuff.
   //is.seekg(ios_base::cur + header.idLength + header.colorMapType * header.colorMapLength);
   
   if (header.dataTypeCode == 2) {
      if (!readUncompressed(data, is, header.bitsPerPixel / 8)) {
         cerr << "Error reading uncompressed contents of file: " << file << endl;
         is.close();
         return false;
      }
   }
   else if (header.dataTypeCode == 10) {
      if (!readCompressed(data, is, header.bitsPerPixel / 8)) {
         cerr << "Error reading compressed contents of file: " << file << endl;
         is.close();
         return false;
      }
   }
   
   is.close();
   return true;
}

bool TgaImageLoader::saveFile(ImageData *data, const std::string &file) {
   TgaHeader header;
   ofstream os(file.c_str(), ios::out | ios::binary);
   
   if (!os.is_open()) {
      cerr << "Error opening file: " << file << endl;
      return false;
   }
   
   //write the header
   header.dataTypeCode = 2;
   header.width = data->width;
   header.height = data->height;
   header.bitsPerPixel = 32;
   header.write(os);
   
   writeUncompressed(data, os);
   
   os.close();
   return true;
}


bool TgaImageLoader::readUncompressed(ImageData *data, std::ifstream &is, int bpp) {
   int i, size = data->width * data->height;
   unsigned char buffer[4];
   
   for (i = 0; i < size; ++i) {
      is.read((char *)buffer, bpp);
      data->setPixel(buffer, i, bpp);
   }
   
   return true;
}

bool TgaImageLoader::readCompressed(ImageData *data, std::ifstream &is, int bpp) {
   int i, j, size = data->width * data->height;
   unsigned char buffer[5];
   
   for (i = 0; i < size;) {
      is.read((char *)buffer, bpp + 1);
      data->setPixel(buffer+1, i, bpp);
      j = buffer[0];
      ++i;
      
      if (j < 128) {
         for (; j > 0 && i < size; --j, ++i) {
            is.read((char *)buffer, bpp);
            data->setPixel(buffer, i, bpp);
         }
      } else {
         for (j -= 128; j > 0 && i < size; --j, ++i)
            data->setPixel(buffer+1, i, bpp);
      }
   }
   
   return true;
}

bool TgaImageLoader::writeUncompressed(ImageData *data, std::ofstream &os) {
   int i, size = data->width * data->height;
   unsigned char buffer[4];
   
   for (i = 0; i < size; ++i) {
      data->getPixel(buffer, i);
      os.write((char*)buffer, 4);
   }
   
   return true;
}


TgaImageLoader::TgaHeader::TgaHeader():
idLength(0),
colorMapType(0),
dataTypeCode(2),
colorMapOrigin(0),
colorMapLength(0),
colorMapDepth(0),
xOrigin(0),
yOrigin(0),
bitsPerPixel(0),
width(0),
height(0),
imageDescriptor(0) {}

bool TgaImageLoader::TgaHeader::read(ifstream &is) {
   is.read(&idLength, 1);
   is.read(&colorMapType, 1);
   is.read(&dataTypeCode, 1);
   is.read((char *)&colorMapOrigin, 2);
   is.read((char *)&colorMapLength, 2);
   is.read(&colorMapDepth, 1);
   is.read((char *)&xOrigin, 2);
   is.read((char *)&yOrigin, 2);
   is.read((char *)&width, 2);
   is.read((char *)&height, 2);
   is.read((char *)&bitsPerPixel, 2);
   //is.read(&imageDescriptor, 1);
   
   return !is.eof();
}

void TgaImageLoader::TgaHeader::write(ofstream &os) {
   unsigned short val;
   
   os.write(&idLength, 1);
   os.write(&colorMapType, 1);
   os.write(&dataTypeCode, 1);
   
   val = colorMapOrigin;
   os.write((char *)&val, 2);
   val = colorMapLength;
   os.write((char *)&val, 2);
   
   os.write(&colorMapDepth, 1);
   
   val = xOrigin;
   os.write((char *)&val, 2);
   val = yOrigin;
   os.write((char *)&val, 2);
   val = width;
   os.write((char *)&val, 2);
   val = height;
   os.write((char *)&val, 2);
   val = bitsPerPixel;
   os.write((char *)&val, 2);
   
   //os.write(&imageDescriptor, 1);
}