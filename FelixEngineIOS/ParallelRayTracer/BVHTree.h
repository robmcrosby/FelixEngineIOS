//
//  BVHTree.h
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/14/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __ParallelRayTracer__BVHTree__
#define __ParallelRayTracer__BVHTree__

#include "Scene.h"

struct BVHNode: public bbox {
   BVHNode(): subNodes(0), items(0), size(0) {}
   BVHNode(Traceable **itm, size_t s): subNodes(0) {
      addItems(itm, s);
   }
   ~BVHNode() {delete[] subNodes;}
   
   void operator=(const bbox &box) {low = box.low; high = box.high;}
   inline void addItems(Traceable **itm, size_t s) {
      items = itm; size = s;
      *this = (*itm++)->getBounds();
      while (itm != items+size)
         addBox((*itm++)->getBounds());
   }
   inline Axis longestAxis() const {
      vec3 a = high - low;
      return a.y < a.x ? a.z < a.x ? AxisX : AxisZ : a.z < a.y ? AxisY : AxisZ;
   }
   inline bool getCollision(const ray3 &ray, Collision *collision);
   void setup();
   
   BVHNode *subNodes;
   Traceable **items;
   size_t size;
};

struct SortVolumes {
   bool operator()(const Traceable *l, const Traceable *r) {
      return l->getBounds().volume() < r->getBounds().volume();
   }
};

class SortAxis {
public:
   SortAxis(Axis axis): _axis(axis) {}
   bool operator()(const Traceable *l, const Traceable *r) {
      bbox lbox = l->getBounds(), rbox = r->getBounds();
      if (_axis == AxisX)
         return (lbox.high.x-lbox.low.x)/2.0+lbox.low.x < (rbox.high.x-rbox.low.x)/2.0+rbox.low.x;
      if (_axis == AxisY)
         return (lbox.high.y-lbox.low.y)/2.0+lbox.low.y < (rbox.high.y-rbox.low.y)/2.0+rbox.low.y;
      return (lbox.high.z-lbox.low.z)/2.0+lbox.low.z < (rbox.high.z-rbox.low.z)/2.0+rbox.low.z;
   }
private:
   Axis _axis;
};

class BVHTree: public SpatialData {
public:
   BVHTree();
   virtual ~BVHTree();
   
   virtual void addTraceable(Traceable *item);
   virtual void setup();
   virtual void clear();
   virtual bool getCollision(const ray3 &ray, Collision *collision);
   
protected:
   size_t _globals;
   BVHNode *_root;
   std::vector<Traceable*> _items;
};

#endif /* defined(__ParallelRayTracer__BVHTree__) */
