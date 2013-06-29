//
//  OpenGLSLMesh.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLMesh.h"

#include <climits>
#include "ResourceData.h"

using namespace std;

map<string, OpenGLMesh*> OpenGLMesh::Meshes;

OpenGLMesh::OpenGLMesh(const std::string &name): Mesh(XMLTag("Mesh")), _vertex(0), _index(0) {
   _tag.setAttribute("name", name);
   Meshes[name] = this;
   _display = Host::GetHost()->getDisplay();
   _filesys = Host::GetHost()->getFileSystem();
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
   if (_tag.hasAttribute("src") && !loaded()) {
      string src = _tag.getAttribute("src");
      string path = "Meshes/" + src;
      map<string, MeshData*> data;
      map<string, MeshData*>::iterator itr;
      
      if (_tag.hasAttribute("name"))
         data[_tag.getAttribute("name")] = NULL;
      else
         Mesh::load();
      
      if (_filesys->loadMeshes(path, &data)) {
         for (itr = data.begin(); itr != data.end(); ++itr) {
            GetMesh(itr->first)->loadData(*itr->second, src);
            delete itr->second;
         }
      }
      else {
         for (itr = data.begin(); itr != data.end(); ++itr)
            delete itr->second;
      }
   }
   else
      Mesh::load();
}

void OpenGLMesh::unload() {
   if (loaded()) {
      Mesh::unload();
      if (!loaded()) {
         glDeleteBuffers(1, &_vertex);
         if (_index)
            glDeleteBuffers(1, &_index);
         
         cout << "unloaded mesh: " << _tag.getAttribute("name") << endl;
      }
   }
}

void OpenGLMesh::use() const {
   if (loaded()) {
      glBindBuffer(GL_ARRAY_BUFFER, _vertex);
      _display->setCurAttributes(&_attributes);
   }
}

void OpenGLMesh::draw() const {
   if (loaded()) {
      if (_index) {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index);
         glDrawElements(_primType, _drawCount, _indexType, 0);
      }
      else
         glDrawArrays(_primType, 0, _drawCount);
   }
}

void OpenGLMesh::loadData(const MeshData &data, const std::string &src) {
   if (loaded()) {
      Mesh::load();
      return;
   }
   
   _tag.setAttribute("src", src);
   
   _primType = data.primType == PRIM_TRI ? GL_TRIANGLES : GL_TRIANGLE_STRIP;
   _attributes = data.attributes;
   _drawCount = data.vertices.size()/_attributes.getStride();
   
   glGenBuffers(1, &_vertex);
   glBindBuffer(GL_ARRAY_BUFFER, _vertex);
   glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(float), &data.vertices[0], GL_STATIC_DRAW);
   
   if (data.indices.size()) {
      glGenBuffers(1, &_index);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index);
      
      // try to save some space
      if (_drawCount < UCHAR_MAX) {
         _indexType = GL_UNSIGNED_BYTE;
         vector<unsigned char> indices(data.indices.begin(), data.indices.end());
         glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indices.size() * sizeof(char), &indices[0], GL_STATIC_DRAW);
      }
      else if (_drawCount < USHRT_MAX) {
         _indexType = GL_UNSIGNED_SHORT;
         vector<unsigned short> indices(data.indices.begin(), data.indices.end());
         glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indices.size() * sizeof(short), &indices[0], GL_STATIC_DRAW);
      }
      else {
         _indexType = GL_UNSIGNED_INT;
         glBufferData(GL_ELEMENT_ARRAY_BUFFER,  data.indices.size() * sizeof(int), &data.indices[0], GL_STATIC_DRAW);
      }
      
      // set the count to the index count
      _drawCount = data.indices.size();
   }
   
   Mesh::load();
   cout << "loaded mesh: " << _tag.getAttribute("name") << endl;
}
