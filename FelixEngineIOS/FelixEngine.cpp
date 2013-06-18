//
//  FelixEngine.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/16/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "FelixEngine.h"

using namespace std;

map<std::string, FelixResource*> FelixResource::RMap;
FelixHost* FelixHost::Instance = NULL;
EntityId* EntityId::IDs;



FelixEntity* EntityId::CreateEntity(XMLTag *tag, FelixEntity *parrent) {
   EntityId *curId = IDs;
   
   /*
   while (curId) {
      if (curId->_idStr == tag->Name())
         return curId->create(tag, scene);
      curId = curId->next_;
   }
    
   cerr << "could not find tag: " << tag->Name() << endl;*/
   return NULL;
}

EntityId::EntityId(const string &idStr): _idStr(idStr) {
   _next = IDs;
   IDs = this;
}
