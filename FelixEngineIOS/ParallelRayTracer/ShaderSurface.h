//
//  ShaderSurface.h
//  RayTracer
//
//  Created by Robert Crosby on 4/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __RayTracer__ShaderSurface__
#define __RayTracer__ShaderSurface__

#include "Shader.h"

class ShaderSurface: public Shader {
public:
   virtual void cast(RayNode *ray) const;
   virtual void draw(RayNode *ray, RayNode *sub) const;
   virtual col4 draw(RayNode *ray) const;
   
   virtual double diffuse(const RayNode &vRay, const RayNode &lRay) const;
   virtual double specular(const RayNode &vRay, const RayNode &lRay) const {return 0;}
   
protected:
   inline void addLightRays(RayNode *ray) const;
   
   inline void addLight(RayNode *ray, RayNode *sub) const;
   inline void addBounce(RayNode *ray) const;
   
   inline double shlick(RayNode *ray, RayNode *sub) const;
   inline col4 beers(RayNode *ray, RayNode *sub) const;
   
   ShaderSurface(SHADER_TYPE type): Shader(type) {}
   virtual ~ShaderSurface() {}
};


#include "Shader.h"

class ShaderColor: public ShaderSurface {
public:
   virtual void cast(RayNode *ray) const;
   virtual col4 draw(RayNode *ray) const;
   
protected:
   ShaderColor(): ShaderSurface(SHADER_COLOR) {}
   virtual ~ShaderColor() {}
   
   static ShaderColor *Instance;
};


class ShaderPhong: public ShaderSurface {
protected:
   ShaderPhong(): ShaderSurface(SHADER_PHONG) {}
   virtual ~ShaderPhong() {}
   
   virtual double specular(const RayNode &vRay, const RayNode &lRay) const;
   
   static ShaderPhong *Instance;
};


class ShaderBlinn: public ShaderSurface {
protected:
   ShaderBlinn(): ShaderSurface(SHADER_BLINN) {}
   virtual ~ShaderBlinn() {}
   
   virtual double specular(const RayNode &vRay, const RayNode &lRay) const;
   
   static ShaderBlinn *Instance;
};


class ShaderGaussian: public ShaderSurface {
   ShaderGaussian(): ShaderSurface(SHADER_GAUSSIAN) {}
   virtual ~ShaderGaussian() {}
   
   virtual double specular(const RayNode &vRay, const RayNode &lRay) const;
   
   static ShaderGaussian *Instance;
};

#endif /* defined(__RayTracer__ShaderPhong__) */
