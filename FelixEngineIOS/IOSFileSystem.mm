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
   ivec2 size(uiImage.size.width, uiImage.size.height);
   
   data->size = size;
   data->data = malloc(size.x * size.y * 4);
   
   CGContextRef imageContext = CGBitmapContextCreate(data->data, size.x, size.y, 8, size.x * 4, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
   CGContextDrawImage(imageContext, CGRectMake(0.0, 0.0, size.x, size.y), uiImage.CGImage);
   CGContextRelease(imageContext);
   
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

