//
//  OpenALAudio.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenALAudio.h"

using namespace std;


OpenALAudio::OpenALAudio(Host *host): _host(host) {

}


Resource* OpenALAudio::getResource(const XMLTag &tag) {
   return NULL;
}

void OpenALAudio::cleanUpResources() {

}