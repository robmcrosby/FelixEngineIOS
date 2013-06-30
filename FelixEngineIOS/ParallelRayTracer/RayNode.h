//
//  RayNode.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 4/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__RayNode__
#define __ParallelRayTracer__RayNode__

#include <list>

#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_queue.h"
#include "tbb/spin_mutex.h"
#include "Utils/Ray.h"
#include "Utils/Color.h"
#include "Scene.h"
#include "Shader.h"
#include "Traceable.h"
#include "Camera.h"
#include "RayScheduler.h"

#define MAX_ITERATIONS 10
#define EPSISLON 0.0001
#define OFFSET_DIST 0.0001

enum RAY_STATE {
   RAY_FREE,
   RAY_UNCASTED,
   RAY_CASTED,
   RAY_STORED,
   RAY_DRAWN,
};

struct RayNode: public ray3 {
   RayNode(): job(0), state(RAY_FREE) {
      destBuff = &colorBuff;
      FreeQue.push(this);
   }
   RayNode(RenderJob *j): job(j), parrent(0), next(0), stored(0), level(0), subcount(0), state(RAY_UNCASTED) {
      subrays = 0;
      scene = job->getScene();
      shader = Shader::GetShader(SHADER_ROOT);
   }
   
   inline void addSubRay(const ray3 &ray, SHADER_TYPE sh, const Collision &col) {
      RayNode *subray = GetFreeRay();
      
      // initalize the subray
      subray->start = ray.start;
      subray->dir = ray.dir;
      subray->level   = level+1;
      subray->scene  = scene;
      subray->shader = Shader::GetShader(sh);
      subray->state = RAY_UNCASTED;
      subray->collision = col;
      subray->subrays = 0;
      subray->subcount = 0;
      subray->stored = NULL;
      
      // add to list
      subray->parrent = this;
      subray->next = next;
      next = subray;
      ++subrays;
      ++subcount;
   }
   inline void addSubRay(const ray3 &ray, SHADER_TYPE sh) {addSubRay(ray, sh, Collision());}
   
   virtual void setFree() {
      state = RAY_FREE;
      if (stored)
         stored->setFree();
      if (!job)
         FreeQue.push(this);
   }
   
   inline void cast() {
      state = RAY_CASTED;
      if (shader) {
         shader->cast(this);
         if (!subcount)
            draw();
      }
      else
         state = RAY_DRAWN;
   }
   inline bool castSelf() {
      if (scene)
         return scene->closestCollision(*this, &collision);
      return false;
   }
   inline void abort() {
      if (parrent) {
         state = RAY_DRAWN;
         if (parrent->abort(this)) {
            parrent->next = next;
            next = parrent;
         }
      }
   }
   inline bool abort(RayNode *subray) {
      return subrays.fetch_and_decrement() == 1;
   }
   
   inline void store(RayNode *ray) {
      tbb::spin_mutex::scoped_lock lock(rayMutex);
      ray->state = RAY_STORED;
      ray->stored = stored;
      stored = ray;
   }
   inline RayNode* findStored(SHADER_TYPE sh) {
      RayNode *cur = stored;
      while (cur && !(*cur->shader == sh))
         cur = cur->stored;
      return cur;
   }
   
   inline void draw() {
      if (parrent) {
         state = RAY_DRAWN;
         if (parrent->draw(this)) {
            parrent->next = next;
            next = parrent;
         }
      }
   }
   inline bool draw(RayNode *subray) {
      shader->draw(this, subray);
      return subrays.fetch_and_decrement() == 1;
   }
   
   
   inline void safeAddColor(const col4 &color) {
      tbb::spin_mutex::scoped_lock lock(rayMutex);
      *destBuff += color;
   }
   
   inline vec3 getReflection(const vec3 &i)  const {return vec3(collision.normal*2*collision.normal.dot(i) - i);}
   inline vec3 getHalfVec(const vec3 &light) const {return (light - dir).normalized();}
   
   inline vec3 getView()       const {return -dir;}
   inline vec3 getReflection() const {return getReflection(getView());}
   inline vec3 getOffsetOut()  const {return collision.loc + collision.offset * OFFSET_DIST;}
   inline vec3 getOffsetIn()   const {return collision.loc - collision.offset * OFFSET_DIST;}
   inline vec3 getNormal()     const {return collision.normal;}
   
   inline const Traceable *getTraced() const {return dynamic_cast<const Traceable*>(collision.item);}
   inline const Light *getLight()      const {return dynamic_cast<const Light*>(collision.item);}
   inline const Material *getMaterial() const {
      const Traceable *traced = getTraced();
      return traced == NULL ? NULL : traced->getMaterial();
   }
   
   inline const bool isInside() const {return getTraced() && dir.dot(getNormal()) > 0;}
   
   const Scene  *scene;
   const Shader *shader;
   Collision    collision;
   
   RenderJob *job;
   col4 *destBuff;
   col4 colorBuff;
   tbb::spin_mutex rayMutex;
   
   int  level;
   RAY_STATE state;
   
   tbb::atomic<size_t> subrays;
   size_t subcount;
   RayNode *parrent;
   RayNode *next;
   RayNode *stored;
   
   static RayNode* GetFreeRay();

private:
   static tbb::concurrent_queue<RayNode*> FreeQue;
   static tbb::concurrent_vector<RayNode> RayPool;
   static tbb::spin_mutex AddFreeMutex;
};

#endif /* defined(__ParallelRayTracer__RayNode__) */
