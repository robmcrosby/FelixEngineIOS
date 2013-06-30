//
//  RayScheduler.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 4/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "RayScheduler.h"
#include "RayNode.h"
#include "Camera.h"
#include "tbb/tbb.h"

using namespace std;
using namespace tbb;

void SerialScheduler::runRenderJobs() {
   while (_renderJobs.size()) {
      RenderJob *job = _renderJobs.front();
      RayNode *last, *cur, treeRoot(job);
      
      cur = &treeRoot;
      
      do {
         if (cur->state == RAY_UNCASTED)
            cur->cast();
         else if (cur->state == RAY_CASTED)
            cur->draw();
         
         // advance to next ray
         last = cur;
         cur = cur->next;
         
         // clean up last
         if (last->state == RAY_DRAWN)
            last->setFree();
      } while (cur);
      
      printf("Done\n");
      
      _renderJobs.pop();
   }
}


ParallelScheduler::ParallelScheduler() {
   tbb::task_scheduler_init init(tbb::task_scheduler_init::automatic);
   _numThreads = task_scheduler_init::default_num_threads();
   
   cout << "Number of threads: " << _numThreads << endl;
}

void ParallelScheduler::runRenderJobs() {
   while (_renderJobs.size()) {
      RenderJob *job = _renderJobs.front();
      vector<RayNode> trees(_numThreads, RayNode(job));
      vector<RayNode>::iterator tree;
      task_list list;
      
      for (tree = trees.begin(); tree != trees.end(); ++tree)
         list.push_back(*new (task::allocate_root()) RayTask(&*tree));
      
      task::spawn_root_and_wait(list);
      cout << "done" << endl;
      
      _renderJobs.pop();
   }
}

/*
void ParallelScheduler::runRenderJobs() {
   while (_renderJobs.size()) {
      RenderJob *job = _renderJobs.front();
      vector<RayNode> trees(_numThreads, RayNode(job));
      vector<RayNode>::iterator tree;
      size_t cur = 0, total = job->getTotal();
      float done;
      
      task* empty = new (task::allocate_root()) empty_task();
      empty->set_ref_count(_numThreads);
      for (tree = trees.begin(); tree != trees.end(); ++tree)
         empty->spawn(*new (empty->allocate_child()) RayTask(&*tree));
      
      do {
         this_tbb_thread::sleep(tick_count::interval_t(1.0));
         cur = job->getCount();
         cur = cur > total ? total : cur;
         done = (float)cur / (float)total * 100.0f;
         printf("%d %d %.2f\n", (int)total, (int)cur, done);
      } while (cur < total);
      
      empty->wait_for_all();
      empty->destroy(*empty);
      
      _renderJobs.pop();
   }
   
   cout << "done" << endl;
}*/

task* RayTask::execute() {
   processRay();
   if (_ray)
      return createNew();
   return NULL;
}

void RayTask::processRay() {
   RayNode *cur = _ray;
   
   if (cur) {
      if (cur->state == RAY_UNCASTED)
         cur->cast();
      else if (cur->state == RAY_CASTED)
         _ray->draw();
      
      _ray = cur->next;
      if (cur->state == RAY_DRAWN)
         cur->setFree();
   }
}

task* RayTask::createNew() {
   task *c = new (allocate_continuation()) empty_task();
   task_list list;
   int count = 0;
   RayNode *cur;
   
   while ((cur = getNext())) {
      list.push_back(*new (c->allocate_child()) RayTask(cur));
      ++count;
   }
   spawn(list);
   c->set_ref_count(count);
   
   return NULL;
}

RayNode* RayTask::getNext() {
   RayNode *next = _ray;
   
   if (next) {
      _ray = next->next;
      next->next = NULL;
   }
   
   return next;
}