//
//  RayNode.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "RayNode.h"

using namespace std;
using namespace tbb;

concurrent_queue<RayNode*> RayNode::FreeQue;
concurrent_vector<RayNode> RayNode::RayPool(1024);
spin_mutex RayNode::AddFreeMutex;


RayNode* RayNode::GetFreeRay() {
   RayNode *next;
   
   while (!FreeQue.try_pop(next)) {
      spin_mutex::scoped_lock lock(AddFreeMutex);
      if (!FreeQue.try_pop(next)) {
         size_t size = RayPool.size() + 1024;
         RayPool.grow_to_at_least(size);
         cout << "Ray Pool grown to: " << size << endl;
      }
   }
   
   return next;
}