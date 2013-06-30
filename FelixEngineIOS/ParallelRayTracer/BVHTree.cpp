//
//  BVHTree.cpp
//  ParallelRayTracer
//
//  Created by Robert Crosby on 5/14/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "BVHTree.h"
//#include <algorithm>

using namespace std;

BVHTree::BVHTree(): _root(0), _globals(0) {
   
}

BVHTree::~BVHTree() {
   clear();
}

void BVHTree::addTraceable(Traceable *item) {
   _items.push_back(item);
}

void BVHTree::setup() {
   vector<Traceable*>::iterator itm;
   
   // remove any previous tree
   delete _root;
   
   // sort out planes
   sort(_items.begin(), _items.end(), SortVolumes());
   itm = _items.begin();
   _globals = 0;
   while (itm != _items.end() && (*itm++)->getBounds().volume() < 0)
      ++_globals;
   
   // create tree
   _root = new BVHNode(&_items[0]+_globals, _items.size()-_globals);
   _root->setup();
}

void BVHTree::clear() {
   delete _root;
   _root = NULL;
   _items.clear();
}

bool BVHTree::getCollision(const ray3 &ray, Collision *collision) {
   Traceable **itm = &_items[0];
   bool hit = false;
   
   while (itm != &_items[_globals])
      hit |= (*itm++)->getCollision(ray, collision);
   if (_root)
      hit |= _root->getCollision(ray, collision);
   
   return hit;
}

void BVHNode::setup() {
   if (size > 3) {
      sort(items, items+size, SortAxis(longestAxis()));
      subNodes = new BVHNode[2];
      
      subNodes[0].addItems(items, size/2);
      subNodes[1].addItems(items+(size/2), size-(size/2));
      
      //cout << "Size: " << size << " Sub1: " << subNodes[0].size << " Sub2:" << subNodes[1].size << endl;
      
      subNodes[0].setup();
      subNodes[1].setup();
      
      size = 0;
      items = 0;
   }
}

bool BVHNode::getCollision(const ray3 &ray, Collision *collision) {
   double dist = collision->distance;
   bool hit = false;
   
   if (intersect(ray, &dist)) {
      if (subNodes) {
         hit |= subNodes[0].getCollision(ray, collision);
         hit |= subNodes[1].getCollision(ray, collision);
      }
      else {
         Traceable **itm = items;
         
         while (itm != items+size)
            hit |= (*itm++)->getCollision(ray, collision);
      }
   }
   return hit;
}