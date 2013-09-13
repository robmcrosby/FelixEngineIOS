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

OpenGLMesh::OpenGLMesh(const std::string &name): Mesh(name), _vertex(0), _index(0) {
   _display = Host::GetHost()->getDisplay();
   _filesys = Host::GetHost()->getFileSystem();
   Meshes[name] = this;
}

OpenGLMesh::~OpenGLMesh() {
   Meshes.erase(getName());
   deleteMesh();
}

OpenGLMesh* OpenGLMesh::GetMesh(const string &name) {
   if (Meshes.find(name) == Meshes.end())
      return new OpenGLMesh(name);
   return Meshes[name];
}

void OpenGLMesh::CleanUpMeshes() {
   map<string, OpenGLMesh*> meshes = Meshes;
   map<string, OpenGLMesh*>::iterator itr;
   
   for (itr = meshes.begin(); itr != meshes.end(); ++itr) {
      if (!itr->second->getCount())
         delete itr->second;
   }
}

void OpenGLMesh::ClearMeshes() {
   map<string, OpenGLMesh*> meshes = Meshes;
   map<string, OpenGLMesh*>::iterator itr;
   
   for (itr = Meshes.begin(); itr != Meshes.end(); ++itr)
      delete itr->second;
}

void OpenGLMesh::SetData(const ResourceData *data) {
   const MeshData *mData = dynamic_cast<const MeshData*>(data);
   if (mData)
      GetMesh(mData->targetName)->setToData(*mData);
}

void OpenGLMesh::load() {
   if (!loaded() && _tag.hasAttribute("src")) {
      string src = _tag.getAttribute("src");
      string path = "Meshes/" + src;
      map<string, MeshData*> data;
      map<string, MeshData*>::iterator itr;
      
      // create a null data item for this mesh
      if (_tag.hasAttribute(ATT_NAME))
         data[getName()] = NULL;
      
      // load the data to the meshes
      if (_filesys->loadMeshes(path, &data)) {
         for (itr = data.begin(); itr != data.end(); ++itr) {
            if (itr->second) {
               itr->second->file = src;
               GetMesh(itr->first)->setToData(*itr->second);
            }
         }
      }
      
      // delete the data
      for (itr = data.begin(); itr != data.end(); ++itr)
         delete itr->second;
   }
}

void OpenGLMesh::unload() {
   if (!getCount())
      deleteMesh();
}

void OpenGLMesh::setToData(const MeshData &data) {
   if (!loaded()) {
      // set the src file for reloading
      if (data.file != "")
         _tag.setAttribute("src", data.file);
      
      // load the data
      loadData(data);
   }
}

void OpenGLMesh::use() const {
  const Shader *activeShader = Shader::GetActiveShader();
  
  if (loaded() && activeShader) {
    glBindBuffer(GL_ARRAY_BUFFER, _vertex);
    activeShader->setAttributes(&_attributes);
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

void OpenGLMesh::loadData(const MeshData &data) {
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
   
   _loaded = true;
   //cout << "loaded mesh: " << getName() << endl;
}

void OpenGLMesh::deleteMesh() {
   if (!loaded()) {
      glDeleteBuffers(1, &_vertex);
      if (_index)
         glDeleteBuffers(1, &_index);
      
      _vertex = 0;
      _index = 0;
      _loaded = false;
      //cout << "unloaded mesh: " << getName() << endl;
   }
}

