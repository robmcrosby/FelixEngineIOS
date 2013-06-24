//
//  ObjLoader.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/22/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__ObjLoader__
#define __FelixEngineIOS__ObjLoader__

#include <iostream>
#include <Vector>
#include <list>
#include <map>
#include "Vector.h"

class ObjData;
class MeshData;

class ObjLoader {
public:
   static void LoadData(std::istream &is, std::map<std::string, MeshData*> *data);
   
private:
   static void AddVec2(std::vector<float> &a, const vec2 &v);
   static void AddVec3(std::vector<float> &a, const vec3 &v);
   
   struct Face {
      ivec3 verts[3];
      bool smooth;
   };
   
   struct ObjData {
      ObjData(): name(""), smooth(0) {}
      void reset() {
         name = "";
         smooth = false;
         faces.clear();
      }
      
      std::string name;
      bool smooth;
      std::vector<vec3> locations;
      std::vector<vec3> normals;
      std::vector<vec2> textures;
      std::vector<Face> faces;
   };
   
   static void AddData(const ObjData &objData, std::map<std::string, MeshData*> *data);
};

#endif /* defined(__FelixEngineIOS__ObjLoader__) */
