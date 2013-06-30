//
//  RayTracer.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/9/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "RayTracer.h"
#include "RayScheduler.h"

using namespace std;

RayTracer::RayTracer(): _scene(0) {
   _renderBuffer = new RenderBuffer(DEF_SIZE);
}

RayTracer::~RayTracer() {
   delete _scene;
   delete _renderBuffer;
}

void RayTracer::setScene(const string &scenePath) {
   _scene = SceneBuilder::BuildScene(scenePath);
}

void RayTracer::renderScene() {
   RayScheduler *scheduler;
   
   if (_scene->getOptions()->parrallel)
      scheduler = new ParallelScheduler();
   else
      scheduler = new SerialScheduler();
      
   RenderJob job(_scene, _renderBuffer);
   
   scheduler->addRenderJob(&job);
   scheduler->runRenderJobs();
}

void RayTracer::setBufferSize(ivec2 size) {
   _renderBuffer->resize(size);
}

void RayTracer::clearBuffer(col4 color) {
   _renderBuffer->clear(color);
}

void RayTracer::saveBuffer(const string &imagePath) {
   _renderBuffer->save(imagePath);
}

void RayTracer::setOptions(const RayOptions &opts) {
   if (_scene)
      _scene->setOptions(opts);
}