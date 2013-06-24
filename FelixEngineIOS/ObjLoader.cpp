//
//  ObjLoader.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/22/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "ObjLoader.h"
#include "ResourceData.h"

using namespace std;

void ObjLoader::LoadData(std::istream &is, std::map<std::string, MeshData*> *data) {
   ObjData objData;
   string line;
   
   if (data->size())
      objData.name = data->begin()->first;
   
   while (!is.eof()) {
      getline(is, line);
      
      if (line.find("o ") == 0) {
         if (objData.name != "")
            AddData(objData, data);
         objData.reset();
         objData.name = line.substr(2);
      }
      else if (line.find("v ") == 0) {
         vec3 loc;
         sscanf(line.c_str(), "v %f %f %f", &loc.x, &loc.y, &loc.z);
         objData.locations.push_back(loc);
      }
      else if (line.find("f ") == 0) {
         Face f;
         if (objData.normals.size()) {
            sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &f.verts[0].x, &f.verts[0].y, &f.verts[0].z,
                   &f.verts[1].x, &f.verts[1].y, &f.verts[1].z,
                   &f.verts[2].x, &f.verts[2].y, &f.verts[2].z);
         }
         else {
            sscanf(line.c_str(), "f %d/%d %d/%d %d/%d",
                   &f.verts[0].x, &f.verts[0].y,
                   &f.verts[1].x, &f.verts[1].y,
                   &f.verts[2].x, &f.verts[2].y);
         }
         f.verts[0] -= ivec3(1, 1, 1);
         f.verts[1] -= ivec3(1, 1, 1);
         f.verts[2] -= ivec3(1, 1, 1);
         f.smooth = objData.smooth;
         objData.faces.push_back(f);
      }
      else if (line.find("vn ") == 0) {
         vec3 norm;
         sscanf(line.c_str(), "vn %f %f %f", &norm.x, &norm.y, &norm.z);
         objData.normals.push_back(norm);
      }
      else if (line.find("vt ") == 0) {
         vec2 uv;
         sscanf(line.c_str(), "vt %f %f", &uv.x, &uv.y);
         //uv.y = 1 - uv.y;
         objData.textures.push_back(uv);
      }
      else if (line.find("s ") == 0)
         objData.smooth = line.find("off") == string::npos;
   }
   
   AddData(objData, data);
}

void ObjLoader::AddData(const ObjData &objData, std::map<std::string, MeshData*> *data) {
   int vCount = 0;
   map<ivec3, int> vMap;
   map<ivec3, int>::iterator mapIter;
   vector<Face>::const_iterator face;
   MeshData *mData;
   
   if (objData.name == "")
      return;
   
   mData = new MeshData();
   
   for (face = objData.faces.begin(); face != objData.faces.end(); ++face) {
      for (int i = 0; i < 3; ++i) {
         mapIter = vMap.find(face->verts[i]);
         if (mapIter == vMap.end()) {
            AddVec3(mData->vertices, objData.locations.at(face->verts[i].x));
            AddVec2(mData->vertices, objData.textures.at(face->verts[i].y));
            if (objData.normals.size())
               AddVec3(mData->vertices, objData.normals.at(face->verts[i].z));
            
            mData->indices.push_back(vCount);
            vMap.insert(pair<ivec3, int>(face->verts[i], vCount));
            ++vCount;
         }
         else
            mData->indices.push_back(mapIter->second);
      }
   }
   
   // set the primative type
   mData->primType = PRIM_TRI;
   
   // set the attributes
   mData->attributes.addAttribute("Pos", 3, 0);
   mData->attributes.addAttribute("Tex", 2, 3);
   if (objData.normals.size())
      mData->attributes.addAttribute("Norm", 3, 5);
   
   (*data)[objData.name] = mData;
}

void ObjLoader::AddVec2(vector<float> &a, const vec2 &v) {
   a.push_back(v.x);
   a.push_back(v.y);
}

void ObjLoader::AddVec3(vector<float> &a, const vec3 &v) {
   a.push_back(v.x);
   a.push_back(v.y);
   a.push_back(v.z);
}