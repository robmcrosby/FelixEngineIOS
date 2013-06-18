//
//  OpenALAudio.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/17/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenALAudio__
#define __FelixEngineIOS__OpenALAudio__

#include "FelixEngine.h"

class OpenALAudio: public FelixHostAudio {
public:
   OpenALAudio(FelixHost *host);
   virtual ~OpenALAudio() {}
   
protected:
   FelixHost *_host;
};

#endif /* defined(__FelixEngineIOS__OpenALAudio__) */
