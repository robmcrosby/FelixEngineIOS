//
//  PovSceneBuilder.cpp
//  RayTracer
//
//  Created by Robert Crosby on 4/5/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include <fstream>
#include <vector>
#include <cmath>

#include "PovSceneBuilder.h"
#include "PerspCamera.h"
#include "PointLight.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
//#include "Box.h"
//#include "Cone.h"
#include "Mesh.h"

using namespace std;

Block::Block(istream &is) {
   static string buffer = "";
   size_t pos1, pos2;
   
   do {
      /* trim out the comments */
      pos1 = buffer.find("//");
      if (pos1 != string::npos)
         buffer = buffer.substr(0, pos1);
      
      pos1 = buffer.find_first_of("{");
      pos2 = buffer.find_first_of("}");
      
      /* check for closing bracket */
      if (pos2 != string::npos) {
         if (pos1 == string::npos || pos1 > pos2) {
            contents += buffer.substr(0, pos2);
            buffer = buffer.substr(pos2+1);
            break;
         }
      }
      
      /* check for opening bracket */
      if (pos1 != string::npos) {
         Block *blk;
         
         contents += buffer.substr(0, pos1);
         buffer = buffer.substr(pos1+1);
         
         /* create new block and find name in contents */
         blk = new Block(is);
         contents = contents.substr(0, contents.find_last_not_of(" \t")+1);
         pos1 = contents.find_last_of(" \t");
         blk->name = contents.substr(pos1+1);
         contents = contents.substr(0, pos1+1) + " ";
         
         blocks.push_back(blk);
      }
      
      contents += buffer;
   } while (getline(is, buffer));
}

Block::~Block() {
   vector<Block*>::iterator itr;
   for (itr = blocks.begin(); itr != blocks.end(); ++itr)
      delete *itr;
}

void Block::write(std::ostream &os, int idt) const {
   vector<Block*>::const_iterator itr;
   
   cout << string(idt*3, ' ') << "Name: " << name << endl;
   os << string(idt*3, ' ') << "Contents: " << contents << endl;
   for (itr = blocks.begin(); itr != blocks.end(); ++itr)
      (*itr)->write(os, idt+1);
   
   os << endl;
}

Block *Block::getBlock(const string &n) const {
   vector<Block*>::const_iterator itr;
   for (itr = blocks.begin(); itr != blocks.end(); ++itr) {
      if ((*itr)->name.find(n) != string::npos)
         return *itr;
   }
   return NULL;
}

Scene* PovSceneBuilder::buildScene(const string &filePath) {
   ifstream fin(filePath.c_str());
   string buffer;
   _scene = new Scene();
   
   if (fin.is_open()) {
      Block blk(fin);
      fin.close();
      
      processSubBlocks(blk);
   }
   else
      cerr << "Error reading file: " << filePath << endl;
   
   _scene->setup();
   return _scene;
}

void PovSceneBuilder::processSubBlocks(const Block &blk) {
   vector<Block*>::const_iterator itr;
   for (itr = blk.blocks.begin(); itr != blk.blocks.end(); ++itr)
      processBlock(**itr);
}

void PovSceneBuilder::processBlock(const Block &blk) {
   if (blk.name.find("camera") != string::npos)
      addCamera(blk);
   else if (blk.name.find("light_source") != string::npos)
      addLight(blk);
   else if (blk.name.find("sphere") != string::npos)
      addSphere(blk);
   else if (blk.name.find("plane") != string::npos)
      addPlane(blk);
   else if (blk.name.find("triangle") != string::npos)
      addTriangle(blk);
   else if (blk.name.find("box") != string::npos)
      addBox(blk);
   else if (blk.name.find("mesh2") != string::npos)
      addMesh(blk);
}

void PovSceneBuilder::addCamera(const Block &blk) {
   vec3 loc, up, right, look;
   PerspCamera *camera;
   
   loc = parseVec3("location", blk.contents);
   up = parseVec3("up", blk.contents);
   right = parseVec3("right", blk.contents);
   look = parseVec3("look_at", blk.contents);
   
   camera = new PerspCamera(loc, up, right, look);
   camera->transform(parseTransform(blk));
   
   if (blk.contents.find("focal_point") != string::npos) {
      double focus = (camera->getOrigin() - parseVec3("focal_point", blk.contents)).length();
      camera->setFocus(focus);
   }
   if (blk.contents.find("aperture") != string::npos)
      camera->setApature(parseDouble("aperture", blk.contents));
   if (blk.contents.find("blur_samples") != string::npos)
      camera->setSamples((int) parseDouble("blur_samples", blk.contents));
   
   _scene->addItem(camera);
}

void PovSceneBuilder::addLight(const Block &blk) {
   vec3 loc = parseVec3("", blk.contents);
   col3 col = parseColor(blk.contents);
   PointLight *light = new PointLight(loc, col);
   
   light->transform(parseTransform(blk));
   
   _scene->addItem(light);
}

void PovSceneBuilder::addSphere(const Block &blk) {
   vec3 loc;
   double radius;
   Material material = parseMaterial(blk);
   Sphere *sphere;
   
   sscanf(blk.contents.c_str(), " < %lf , %lf , %lf > , %lf",
          &loc.x, &loc.y, &loc.z, &radius);
   
   sphere = new Sphere(loc, radius, material);
   sphere->transform(parseTransform(blk));
   
   _scene->addItem(sphere);
}

void PovSceneBuilder::addPlane(const Block &blk) {
   vec4 eq;
   Material material = parseMaterial(blk);
   Plane *plane;
   
   sscanf(blk.contents.c_str(), " < %lf , %lf , %lf > , %lf",
          &eq.x, &eq.y, &eq.z, &eq.w);
   
   plane = new Plane(eq, material);
   plane->transform(parseTransform(blk));
   
   _scene->addItem(plane);
}

void PovSceneBuilder::addTriangle(const Block &blk) {
   vec3 pts[3], nor[3];
   Material material = parseMaterial(blk);
   Triangle *triangle;
   size_t loc;
   bool normals = blk.name.find("smooth") != string::npos;
   
   if (normals) {
      sscanf(blk.contents.c_str(),
             " < %lf , %lf , %lf > , < %lf , %lf , %lf > , < %lf , %lf , %lf > , < %lf , %lf , %lf > , < %lf , %lf , %lf > , < %lf , %lf , %lf",
             &pts[0].x, &pts[0].y, &pts[0].z,
             &nor[0].x, &nor[0].y, &nor[0].z,
             &pts[1].x, &pts[1].y, &pts[1].z,
             &nor[1].x, &nor[1].y, &nor[1].z,
             &pts[2].x, &pts[2].y, &pts[2].z,
             &nor[2].x, &nor[2].y, &nor[2].z);
   }
   else {
      sscanf(blk.contents.c_str(), " < %lf , %lf , %lf > , < %lf , %lf , %lf > , < %lf , %lf , %lf",
             &pts[0].x, &pts[0].y, &pts[0].z,
             &pts[1].x, &pts[1].y, &pts[1].z,
             &pts[2].x, &pts[2].y, &pts[2].z);
   }
   
   
   loc = blk.contents.find("uv_vectors");
   if (loc != string::npos) {
      vec2 uvs[3];
      sscanf(blk.contents.substr(loc).c_str(), "uv_vectors < %lf , %lf > , < %lf , %lf > , < %lf , %lf",
             &uvs[0].x, &uvs[0].y,
             &uvs[1].x, &uvs[1].y,
             &uvs[2].x, &uvs[2].y);
      triangle = normals ? new Triangle(pts, nor, uvs, material) : new Triangle(pts, uvs, material);
   }
   else
      triangle = normals ? new Triangle(pts, nor, material) : new Triangle(pts, material);
   triangle->transform(parseTransform(blk));
   
   _scene->addItem(triangle);
}

void PovSceneBuilder::addBox(const Block &blk) {
   vec3 low, high;
   Material material = parseMaterial(blk);
   Mesh *box;
   
   sscanf(blk.contents.c_str(), " < %lf , %lf , %lf > , < %lf , %lf , %lf",
          &low.x, &low.y, &low.z, &high.x, &high.y, &high.z);
   
   box = new Mesh(low, high, material);
   box->transform(parseTransform(blk));
   _scene->addItem(box);
}

void PovSceneBuilder::addMesh(const Block &blk) {
   cout << "add mesh" << endl;
   
   /*
   Block *bp = blk.getBlock("face_indices");
   
   if (bp) {
      size_t size;
      vec3 *verts = parseVec3s(blk.getBlock("vertex_vectors"), &size);
      vec3 *norms = parseVec3s(blk.getBlock("normal_vectors"), &size);
      vec2 *uvs   = parseVec2s(blk.getBlock("uv_vectors"), &size);
      Material *mats = parseMaterials(blk.getBlock("texture_list"), &size);
      sscanf(bp->contents.c_str(), " %li", &size);
      
      if (size && verts) {
         ivec3 *curIdx, *idx;
         size_t triSize = size;
         MeshTri *curTri, *tris = new MeshTri[triSize];
         fill_n(tris, triSize, MeshTri(verts, norms, uvs, mats));
         
         curTri = tris;
         curIdx = idx = parseIvec3s(bp, &size);
         for (curTri = tris; size-- > 0; ++curTri) {
            curTri->verts = *curIdx++;
            curTri->mats = ivec3(0);
         }
         delete[] idx;
         
         bp = blk.getBlock("normal_indices");
         if (bp) {
            curTri = tris;
            curIdx = idx = parseIvec3s(bp, &size);
            for (curTri = tris; size-- > 0; ++curTri)
               curTri->norms = *curIdx++;
            delete[] idx;
         }
         
         bp = blk.getBlock("uv_indices");
         if (bp) {
            curTri = tris;
            curIdx = idx = parseIvec3s(bp, &size);
            for (curTri = tris; size-- > 0; ++curTri)
               curTri->uvs = *curIdx++;
            delete[] idx;
         }
         
         _scene->addItem(new Mesh(tris, triSize));
      }
      else {
         delete[] verts;
         delete[] norms;
         delete[] uvs;
         delete[] mats;
      }
   }*/
}



double PovSceneBuilder::parseDouble(const string &name, string str) const {
   double value = 0;
   size_t pos = str.find(name);
   
   if (pos == string::npos)
      return value;
   str = str.substr(pos+name.size()+1);
   sscanf(str.c_str(), " %lf", &value);
   return value;
}

vec3 PovSceneBuilder::parseVec3(const string &name, string str) const {
   size_t pos = str.find(name);
   
   if (pos == string::npos)
      return 0.0f;
   str = str.substr(pos);
   
   pos = str.find("<");
   if (pos == string::npos)
      return 0.0f;
   
   return vec3::parseDouble(str.substr(pos+1));
}

col3 PovSceneBuilder::parseColor(const string &str) const {
   return parseVec3("rgb", str);
}

void PovSceneBuilder::parsePigment(const Block &blk, Material *mat) const {
   string str = blk.contents;
   size_t pos;
   Block *bp;
   
   if ((pos = str.find("rgbft")) != string::npos) {
      sscanf(str.substr(pos).c_str(), "rgbft < %lf , %lf , %lf , %lf , %lf",
             &mat->color.r, &mat->color.g, &mat->color.b, &mat->flt, &mat->tra);
   }
   else if ((pos = str.find("rgbt")) != string::npos) {
      sscanf(str.substr(pos).c_str(), "rgbt < %lf , %lf , %lf , %lf",
             &mat->color.r, &mat->color.g, &mat->color.b, &mat->tra);
   }
   else if ((pos = str.find("rgbf")) != string::npos) {
      sscanf(str.substr(pos).c_str(), "rgbf < %lf , %lf , %lf , %lf",
             &mat->color.r, &mat->color.g, &mat->color.b, &mat->flt);
   }
   else if ((pos = str.find("rgb")) != string::npos) {
      sscanf(str.substr(pos).c_str(), "rgb < %lf , %lf , %lf",
             &mat->color.r, &mat->color.g, &mat->color.b);
   }
   
   bp = blk.getBlock("image_map");
   if (bp) {
      str = bp->contents;
      str = str.substr(str.find('\"')+1);
      str = str.substr(0, str.find('\"'));
      mat->img = _scene->getImage(str);
   }
}

Material PovSceneBuilder::parseMaterial(const Block &blk) const {
   Material mat;
   Block *bp;
   
   bp = blk.getBlock("pigment");
   if (bp)
      parsePigment(*bp, &mat);
   
   bp = blk.getBlock("finish");
   if (bp) {
      mat.amb = parseDouble("ambient", bp->contents);
      mat.dif = parseDouble("diffuse", bp->contents);
      mat.spc = parseDouble("specular", bp->contents);
      mat.rou = parseDouble("roughness", bp->contents);
      mat.ref = parseDouble("reflection", bp->contents);
      mat.fra = parseDouble("refraction", bp->contents);
      mat.ior = parseDouble("ior", bp->contents);
   }
   
   // pre compute attenuation for a dielectric surface
   if (mat.fra) {
      mat.color.r = log(mat.color.r);
      mat.color.g = log(mat.color.g);
      mat.color.b = log(mat.color.b);
   }
   
   return mat;
}

trans PovSceneBuilder::parseTransform(const Block &blk) const {
   string str = blk.contents;
   size_t posS, posR, posT;
   trans tra;
   
   do {
      posS = str.find("scale");
      posR = str.find("rotate");
      posT = str.find("translate");
      
      if (posS != string::npos && (posR == string::npos || posS < posR) && (posT == string::npos || posS < posT)) {
         tra.mtx = mat4::Scale(parseVec3("scale", str)) * tra.mtx;
         str = str.substr(posS+1);
      }
      else if (posR != string::npos && (posT == string::npos || posR < posT)) {
         vec3 rot = parseVec3("rotate", str);
         rot *= Pi / 180.0; // convert to radian
         tra.mtx = mat4::Rotate(rot) * tra.mtx;
         tra.rot = mat4::Rotate(rot) * tra.rot;
         str = str.substr(posR+1);
      }
      else if (posT != string::npos) {
         tra.mtx = mat4::Trans3d(parseVec3("translate", str)) * tra.mtx;
         str = str.substr(posT+1);
      }
   } while (posS != string::npos || posR != string::npos || posT != string::npos);
   
   tra.invMtx = tra.mtx.inverse();
   tra.invRot = tra.rot.inverse();
   return tra;
}

vec3 *PovSceneBuilder::parseVec3s(const Block *blk, size_t *size) const {
   size_t loc;
   vec3 *vecs, *cur;
   
   if (!blk)
      return NULL;
   
   string str = blk->contents;
   sscanf(str.c_str(), " %li", size);
   if (!size)
      return NULL;
   
   cur = vecs = new vec3[*size];
   
   while ((loc = str.find("<")) != string::npos) {
      str = str.substr(++loc);
      *cur++ = vec3::parseDouble(str);
   }
   return vecs;
}

vec2 *PovSceneBuilder::parseVec2s(const Block *blk, size_t *size) const {
   size_t loc;
   vec2 *vecs, *cur;
   
   if (!blk)
      return NULL;
   
   string str = blk->contents;
   sscanf(str.c_str(), " %li", size);
   if (!size)
      return NULL;
   
   cur = vecs = new vec2[*size];
   
   while ((loc = str.find("<")) != string::npos) {
      str = str.substr(++loc);
      *cur++ = vec2::parseDouble(str);
   }
   return vecs;
}

ivec3 *PovSceneBuilder::parseIvec3s(const Block *blk, size_t *size) const {
   string str = blk->contents;
   size_t loc;
   ivec3 *vecs, *cur;
   
   sscanf(str.c_str(), " %li", size);
   if (!size)
      return NULL;
   
   cur = vecs = new ivec3[*size];
   
   while ((loc = str.find("<")) != string::npos) {
      str = str.substr(++loc);
      *cur++ = ivec3::parseInt(str);
   }
   return vecs;
}

Material *PovSceneBuilder::parseMaterials(const Block *blk, size_t *size) const {
   Material *mats;
   size_t i;
   
   if (!blk)
      return NULL;
   
   sscanf(blk->contents.c_str(), " %li", size);
   if (*size <= 0 || blk->blocks.size() < *size)
      return NULL;
   
   mats = new Material[*size];
   for (i = 0; i < *size; ++i)
      mats[i] = parseMaterial(*blk->blocks.at(i));
   return mats;
}
