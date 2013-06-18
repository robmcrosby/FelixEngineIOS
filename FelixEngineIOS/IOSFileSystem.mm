//
//  IOSFileSystem.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "IOSFileSystem.h"

using namespace std;

IOSFileSystem::IOSFileSystem(FelixHost *host): _host(host) {
   _host->addListener(this);
   _basePath = [[[NSBundle mainBundle]bundlePath] UTF8String];
}

