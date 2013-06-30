//
//  RayScheduler.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 4/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__RayScheduler__
#define __ParallelRayTracer__RayScheduler__

#include <iostream>
#include <queue>
#include <vector>

#include "tbb/tbb.h"
#include "tbb/concurrent_vector.h"
#include "tbb/atomic.h"
#include "Distributions.h"
#include "RenderBuffer.h"
#include "Scene.h"

class Scene;
class RayNode;

class RenderJob {
public:
   RenderJob(Scene *scene, RenderBuffer *buff): _scene(scene), _buffer(buff) {
      DIST_TYPE dist = (DIST_TYPE)scene->getOptions()->aaType;
      
      _counter = 0;
      _total = _buffer->size().x * _buffer->size().y;
      _dists = Distributions::CreateDistsEven2D(dist, DIST_COUNT);
   }
   
   inline col4* nextPixel(ivec2 *loc) {
      size_t pixel = _counter.fetch_and_increment();
      ivec2 size = _buffer->size();
      
      if (pixel >= size.x*size.y)
         return NULL;
      
      // get the buffer location
      loc->x = (int)(pixel % (size_t)size.x);
      loc->y = (int)(pixel / (size_t)size.x);
      
      return _buffer->ptrAt(*loc);
   }
   inline const size_t getTotal() const {return _total;}
   inline const size_t getCount() const {return _counter;}
   inline const bool isDone() const {
      size_t cur = _counter;
      return cur >= _total;
   }
   inline const size_t getPixel() {return _counter.fetch_and_increment();}
   inline const ivec2 getLoc(size_t pixel) const {
      return ivec2((int)pixel % _buffer->size().x, (int)pixel / _buffer->size().x);
   }
   inline col4* getDest(size_t pixel) {
      return pixel < _total ? _buffer->ptrAt(getLoc(pixel)) : NULL;
   }
   inline const ivec2 getBuffSize() const {return _buffer->size();}
   inline const Scene* getScene() const {return _scene;}
   inline const size_t getDistSize() const {return _dists->size() * _dists->at(0).size();}
   inline const dist2d* getDist(size_t loc) const {return &_dists->at(loc%_dists->size());}
   
private:
   const Scene  *_scene;
   RenderBuffer *_buffer;
   dists2d      *_dists;
   size_t _total;
   tbb::atomic<size_t> _counter;
};

class RayScheduler {
public:
   RayScheduler(): _numThreads(1) {}
   virtual ~RayScheduler() {}
   
   void addRenderJob(RenderJob *job) {_renderJobs.push(job);}
   virtual void runRenderJobs() = 0;
   
protected:
   std::queue<RenderJob*> _renderJobs;
   int _numThreads;
};


class SerialScheduler: public RayScheduler {
public:
   SerialScheduler() {}
   virtual void runRenderJobs();
};


class ParallelScheduler: public RayScheduler {
public:
   ParallelScheduler();
   virtual void runRenderJobs();
};

class RayTask: public tbb::task {
public:
   RayTask(RayNode *ray): _ray(ray) {}
   task *execute();
private:
   inline void processRay();
   inline task *createNew();
   inline RayNode *getNext();
   
   RayNode *_ray;
};


#endif /* defined(__ParallelRayTracer__RayScheduler__) */
