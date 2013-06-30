//
//  Scene.h
//  RayTracer
//
//  Created by Robert Crosby on 4/4/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__Scene__
#define __RayTracer__Scene__

#include <vector>
#include <map>

#include "Item.h"
#include "Camera.h"
#include "Light.h"
#include "Traceable.h"
#include "Shader.h"
#include "ImageBuffer.h"
#include "Distributions.h"

enum DEFAULTS {
   MAX_LEVELS = 8,
   GI_SAMPLES = 32,
   GI_LEVELS = 0,
   GI_RECURE = 1,
   AA_TYPE = DIST_NONE,
   DIST_COUNT = 32,
};

struct RayOptions {
   RayOptions(): maxLevels(MAX_LEVELS), giSamples(GI_SAMPLES),
   giLevels(GI_LEVELS), giRecure(GI_RECURE), aaType(AA_TYPE),
   varbose(false), parrallel(false), shader(SHADER_PHONG) {}
   
   int maxLevels;
   int giSamples;
   int giLevels;
   int giRecure;
   int aaType;
   int aoSamples;
   bool varbose;
   bool parrallel;
   SHADER_TYPE shader;
};

struct SpatialData {
   virtual ~SpatialData() {}
   virtual void addTraceable(Traceable *item) = 0;
   virtual void setup() = 0;
   virtual void clear() = 0;
   virtual bool getCollision(const ray3 &ray, Collision *collision) = 0;
};

class Scene: public Item {
public:
   Scene();
   virtual ~Scene();
   
   virtual vec3 getOrigin() const;
   virtual bbox getBounds() const;
   virtual void write(std::ostream &os, int indent) const;
   virtual void transform(const trans &t);
   
   virtual ImageBuffer *getImage(const std::string &file);
   virtual void addItem(Item *item);
   virtual void setup();
   virtual bool closestCollision(const ray3 &ray, Collision *collision) const;
   inline const Camera *getCamera() const {return _camera;}
   inline const std::vector<Light*> *getLights() const {return &_lights;}
   
   inline void setOptions(const RayOptions &opts) {_options = opts;}
   inline const RayOptions* getOptions() const {return &_options;}
   
   inline SHADER_TYPE getPrimShader() const {return _options.shader;}
protected:
   inline void clearItems();
   
   vec3 _origin;
   std::vector<Item*> _items; // all items in the scene
   std::vector<Traceable*> _traces;
   
   Camera *_camera;
   std::vector<Light*> _lights;
   std::map<std::string, ImageBuffer*> _images;
   trans _trans;
   bbox _bounds;
   
   RayOptions _options;
   
   SpatialData *_spatialData;
};

struct SceneBuilder {
   virtual Scene* buildScene(const std::string &filePat) = 0;
   
   static Scene* BuildScene(const std::string &filePath);
   static void SetBuilder(SceneBuilder *builder);
private:
   static SceneBuilder *Builder;
};

#endif /* defined(__RayTracer__Scene__) */
