//
//  Scene.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "Scene.h"
#include "PovSceneBuilder.h"
#include "BVHTree.h"

using namespace std;

SceneBuilder *SceneBuilder::Builder = new PovSceneBuilder();

Scene *SceneBuilder::BuildScene(const string &filePath) {
   if (Builder)
      return Builder->buildScene(filePath);
   return new Scene();
}

Scene::Scene() {
   _spatialData = new BVHTree();
}

Scene::~Scene() {
   clearItems();
   delete _spatialData;
}

vec3 Scene::getOrigin() const {
   return _trans.mtx * _origin;
}

bbox Scene::getBounds() const {
   return _bounds;
}

void Scene::write(ostream &os, int indent) const {
   vector<Item*>::const_iterator itr;
   
   os << "Scene" << endl;
   for (itr = _items.begin(); itr != _items.end(); ++itr)
      os << **itr;
}

void Scene::transform(const trans &t) {
   vector<Item*>::const_iterator itr;
   
   _trans = t;
   for (itr = _items.begin(); itr != _items.end(); ++itr)
      (*itr)->transform(t);
}

ImageBuffer *Scene::getImage(const std::string &file) {
   map<string, ImageBuffer*>::iterator itr = _images.find(file);
   if (itr != _images.end())
      return itr->second;
   else {
      ImageBuffer *image = new ImageBuffer(file);
      if (image->load())
         _images[file] = image;
      else {
         delete image;
         image = NULL;
      }
      return image;
   }
}

void Scene::addItem(Item *item) {
   if (item) {
      Camera *camera = dynamic_cast<Camera*>(item);
      Traceable *traceable = dynamic_cast<Traceable*>(item);
      Light *light = dynamic_cast<Light*>(item);
      
      if (camera)
         _camera = camera;
      if (traceable) {
         _bounds.addBox(traceable->getBounds());
         _spatialData->addTraceable(traceable);
         _traces.push_back(traceable);
      }
      if (light)
         _lights.push_back(light);
      
      _items.push_back(item);
   }
}

void Scene::setup() {
   _spatialData->setup();
}

bool Scene::closestCollision(const ray3 &ray, Collision *collision) const {
   vector<Traceable*>::const_iterator itr;
   /*bool hit = false;
   
   for (itr = _traces.begin(); itr != _traces.end(); ++itr)
      hit = (*itr)->getCollision(ray, collision) || hit;
   return hit;*/
   
   return _spatialData->getCollision(ray, collision);
}

void Scene::clearItems() {
   vector<Item*>::const_iterator item;
   map<string, ImageBuffer*>::iterator img;
   
   // clear the items
   for (item = _items.begin(); item != _items.end(); ++item)
      delete *item;
   _items.clear();
   
   // clear the images
   for (img = _images.begin(); img != _images.end(); ++img)
      delete img->second;
   _images.clear();
   
   _spatialData->clear();
   _lights.clear();
   _camera = NULL;
}
