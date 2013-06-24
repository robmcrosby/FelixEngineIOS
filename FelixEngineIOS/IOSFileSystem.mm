//
//  IOSFileSystem.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "IOSFileSystem.h"

#include <fstream>
#include "ObjLoader.h"
#include "ResourceData.h"

using namespace std;

IOSFileSystem::IOSFileSystem(Host *host): _host(host) {
   _host->addListener(this);
   _basePath = [[[NSBundle mainBundle]bundlePath] UTF8String];
   _basePath += "/";
}

string IOSFileSystem::loadTxt(const string &path) const {
   fstream fileIn;
   string txtStr, fullPath = _basePath + path;
   
   fileIn.open(fullPath.c_str(), ios::in);
   if (fileIn.is_open()) {
      txtStr = string((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
      fileIn.close();
   }
   else
      cerr << "error reading text file: " << path << endl;
   
   return txtStr;
}

XMLTag* IOSFileSystem::loadXML(const string &path) const {
   fstream fileIn;
   string fullPath = _basePath + path;
   XMLTag *tag = NULL;
   
   fileIn.open(fullPath.c_str(), ios::in);
   if (fileIn.is_open()) {
      tag = new XMLTag();
      fileIn >> *tag;
      fileIn.close();
   }
   else
      cerr << "error reading XML file: " << path << endl;
   
   return tag;
}

TextureData* IOSFileSystem::loadTexture(const std::string &path) const {
   string fullPath = _basePath + path;
   NSString *nsPath = [[NSString alloc] initWithUTF8String:fullPath.c_str()];
   TextureData *data = new TextureData();
   
   data->type = TEX_RGBA;
   data->size = 0;
   
   UIImage* uiImage = [UIImage imageWithContentsOfFile:nsPath];
   [nsPath release];
   
   if (uiImage == nil)
      return data;
   
   data->size = ivec2(CGImageGetWidth(uiImage.CGImage), CGImageGetHeight(uiImage.CGImage));
   data->pixelSize = CGImageGetBitsPerPixel(uiImage.CGImage);
   data->type = data->pixelSize == 32 ? TEX_RGBA : TEX_RGB; //update for other types
   data->data = malloc(data->size.x * data->size.y * data->pixelSize);
   
   CGColorSpaceRef colorSpace = CGImageGetColorSpace(uiImage.CGImage);
   CGContextRef context = CGBitmapContextCreate(data->data, data->size.x, data->size.y, 8, data->pixelSize * data->size.x,
                                                colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
   
   CGColorSpaceRelease(colorSpace);
   CGContextClearRect(context, CGRectMake(0, 0, data->size.x, data->size.y));
   CGContextTranslateCTM(context, 0, 0);
   CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, data->size.y);
   CGContextConcatCTM(context, flipVertical);
   CGContextDrawImage(context, CGRectMake( 0, 0, data->size.x, data->size.y), uiImage.CGImage);
   
   CGContextRelease(context);
   
   return data;
}

bool IOSFileSystem::loadMeshes(const string &path, map<string, MeshData*> *data) const {
   fstream fileIn;
   string fullPath = _basePath + path;
   
   fileIn.open(fullPath.c_str(), ios::in);
   if (fileIn.is_open()) {
      ObjLoader::LoadData(fileIn, data);
      fileIn.close();
      return true;
   }
   
   cerr << "error reading mesh file: " << path << endl;
   return false;
}

