//
//  OpenGLSLShader.h
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef __FelixEngineIOS__OpenGLShader__
#define __FelixEngineIOS__OpenGLShader__

#include "FelixEngine.h"
#include "OpenGLIncludes.h"

#define MSG_SIZE 256
#define MTX_TRANSPOSE GL_FALSE

class OpenGLShader: public Shader {
private:
  OpenGLShader(const std::string &name);
  virtual ~OpenGLShader();
  
public:
  static OpenGLShader* GetShader(const std::string &name);
  static void CleanUpShaders();
  static void ClearShaders();
  static void SetData(const ResourceData *data);
  
  virtual void load();
  virtual void unload();
  
  virtual void setToData(const ShaderData &data);
  
  virtual void use() const;
  virtual void setUniforms(const Uniforms *unis) const;
  virtual void setAttributes(const Attributes *atts) const;
  
private:
  inline void loadData(const ShaderData &data);
  inline void deleteShader();
  inline void setUniform(const std::string &name, const Uniform &uni) const;
  inline void setTextures();
  
  HostFileSystem *_filesys;
  GLuint _program;
  
  class ShaderPart {
  public:
    ShaderPart(GLenum type): _type(type), _shader(0) {}
    ~ShaderPart();
    
    inline bool compile(std::string src);
    inline void attach(GLint program);
    inline void detatch(GLint program);
  private:
    GLenum _type;
    GLuint _shader;
  };
  
  static std::map<std::string, OpenGLShader*> Shaders;
};

#endif /* defined(__FelixEngineIOS__OpenGLShader__) */
