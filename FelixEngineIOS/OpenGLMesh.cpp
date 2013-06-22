//
//  OpenGLSLMesh.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLMesh.h"

using namespace std;

map<string, OpenGLMesh*> OpenGLMesh::Meshes;

OpenGLMesh::OpenGLMesh(const std::string &name): Mesh(XMLTag("Mesh")) {
   _tag.setAttribute("name", name);
   Meshes[name] = this;
   cout << "create mesh" << endl;
}

OpenGLMesh::~OpenGLMesh() {
   
}

OpenGLMesh* OpenGLMesh::GetMesh(const string &name) {
   if (Meshes.find(name) == Meshes.end())
      return new OpenGLMesh(name);
   return Meshes[name];
}

void OpenGLMesh::ClearMeshes() {
   map<string, OpenGLMesh*>::iterator itr;
   for (itr = Meshes.begin(); itr != Meshes.end(); ++itr)
      delete itr->second;
   Meshes.clear();
}

void OpenGLMesh::load() {
   cout << "load mesh" << endl;
}

void OpenGLMesh::unload() {
   cout << "unload mesh" << endl;
}

void OpenGLMesh::use() const {
   
}

void OpenGLMesh::render() const {
   
}