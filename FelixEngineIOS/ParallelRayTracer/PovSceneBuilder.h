//
//  PovSceneBuilder.h
//  RayTracer
//
//  Created by Robert Crosby on 4/5/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__PovSceneBuilder__
#define __RayTracer__PovSceneBuilder__

#include <vector>

#include "Scene.h"

class MeshTri;

struct Block {
   Block(std::istream &is);
   ~Block();
   void write(std::ostream &os, int idt = 0) const;
   Block *getBlock(const std::string &n) const;
   
   std::string name;
   std::string contents;
   std::vector<Block*> blocks;
};

class PovSceneBuilder: public SceneBuilder {
public:
   virtual Scene* buildScene(const std::string &filePat);
   
private:
   inline void processSubBlocks(const Block &blk);
   inline void processBlock(const Block &blk);
   
   inline void addCamera(const Block &blk);
   inline void addLight(const Block &blk);
   inline void addSphere(const Block &blk);
   inline void addPlane(const Block &blk);
   inline void addTriangle(const Block &blk);
   inline void addBox(const Block &blk);
   inline void addMesh(const Block &blk);
   
   inline double parseDouble(const std::string &name, std::string str) const;
   inline vec3 parseVec3(const std::string &name, std::string str) const;
   inline col3 parseColor(const std::string &str) const;
   inline void parsePigment(const Block &blk, Material *mat) const;
   inline Material parseMaterial(const Block &blk) const;
   inline trans parseTransform(const Block &blk) const;
   
   inline vec3 *parseVec3s(const Block *blk, size_t *size) const;
   inline vec2 *parseVec2s(const Block *blk, size_t *size) const;
   inline ivec3 *parseIvec3s(const Block *blk, size_t *size) const;
   inline Material *parseMaterials(const Block *blk, size_t *size) const;
   
   Scene *_scene;
};

#endif /* defined(__RayTracer__PovSceneBuilder__) */
