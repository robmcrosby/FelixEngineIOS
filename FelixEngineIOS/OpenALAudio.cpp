//
//  OpenALAudio.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenALAudio.h"

using namespace std;


OpenALAudio::OpenALAudio(FelixHost *host): _host(host) {
   host->addListener(this);
}