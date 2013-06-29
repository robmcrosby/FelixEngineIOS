//
//  Transform.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/28/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FelixEngineIOS_Transform_h
#define FelixEngineIOS_Transform_h

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "ShaderVaribles.h"
#include "XMLTag.h"

class Transform {
public:
   Transform(): _scale(1.0f), _parrent(0), _updated(0) {
      _uniforms.addUniform(VAR_LOCAL_MTX, VAL_MAT4X4, &_localMtx);
      _uniforms.addUniform(VAR_MODEL_MTX, VAL_MAT4X4, &_modelMtx);
      _uniforms.addUniform(VAR_NORM_MTX, VAL_MAT3X3, &_normalMtx);
   };
   
   inline void setPos(vec3 p) {_pos = p; _updated = false;}
   inline void setRot(quat r) {_rot = r; _updated = false;}
   inline void setScale(vec3 s) {_scale = s; _updated = false;}
   
   inline void move(vec3 v) {_pos += v; _updated = false;}
   inline void rotate(quat r) {_rot *= r; _updated = false;}
   inline void scale(vec3 s) {_scale *= s; _updated = false;}
   
   inline void setParrent(const Transform *p) {_parrent = p; _updated = false;}
   
   inline vec3 getPos() const {return _parrent ? _parrent->getModelMtx()*_pos : _pos;}
   inline quat getRot() const {return _parrent ? _parrent->getRot() * _rot : _rot;}
   
   inline bool update() const {
      if (_parrent ? _parrent->update() || !_updated : !_updated) {
         _localMtx = mat4::Trans3d(_pos) * _rot.toMat4() * mat4::Scale(_scale);
         _modelMtx = _parrent ? _parrent->getModelMtx()*_localMtx : _localMtx;
         _normalMtx = getRot().toMat3();
         _updated = true;
         return true;
      }
      return false;
   }
   
   inline mat4 getLocalMtx() const {
      update();
      return _localMtx;
   }
   inline mat4 getModelMtx() const {
      update();
      return _modelMtx;
   }
   inline mat3 getNormalMtx() const {
      update();
      return _normalMtx;
   }
   
   inline const Uniforms* getUniforms() const {
      update();
      return &_uniforms;
   }
   
private:
   mutable bool _updated;
   vec3 _pos;
   quat _rot;
   vec3 _scale;
   
   mutable mat4 _localMtx;
   mutable mat4 _modelMtx;
   mutable mat3 _normalMtx;
   
   const Transform *_parrent;
   Uniforms _uniforms;
};

#endif
