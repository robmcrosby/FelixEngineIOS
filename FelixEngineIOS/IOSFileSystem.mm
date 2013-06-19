//
//  IOSFileSystem.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "IOSFileSystem.h"
#include <fstream>

using namespace std;

IOSFileSystem::IOSFileSystem(FelixHost *host): _host(host) {
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