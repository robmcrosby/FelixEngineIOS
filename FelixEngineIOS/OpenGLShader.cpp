//
//  OpenGLSLShader.cpp
//  FelixEngineIOS
//
//  Created by Robert Crosby on 6/18/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#include "OpenGLShader.h"

using namespace std;

map<string, OpenGLShader*> OpenGLShader::Shaders;


OpenGLShader::OpenGLShader(const string &name): Shader(name), _program(0) {
   _filesys = Host::GetHost()->getFileSystem();
   Shaders[name] = this;
}

OpenGLShader::~OpenGLShader() {
   Shaders.erase(getName());
   deleteShader();
}

OpenGLShader* OpenGLShader::GetShader(const std::string &name) {
   if (Shaders.find(name) == Shaders.end())
      return new OpenGLShader(name);
   return Shaders[name];
}

void OpenGLShader::CleanUpShaders() {
   map<string, OpenGLShader*> shaders = Shaders;
   map<string, OpenGLShader*>::iterator itr;
   
   for (itr = shaders.begin(); itr != shaders.end(); ++itr) {
      if (!itr->second->getCount())
         delete itr->second;
   }
}

void OpenGLShader::ClearShaders() {
   map<string, OpenGLShader*>::iterator itr;
   for (itr = Shaders.begin(); itr != Shaders.end(); ++itr)
      delete itr->second;
   Shaders.clear();
}

void OpenGLShader::SetData(const ResourceData *data) {
   const ShaderData *sData = dynamic_cast<const ShaderData*>(data);
   if (sData)
      GetShader(sData->targetName)->setToData(*sData);
}

void OpenGLShader::load() {
   if (!loaded() && _tag.hasAttribute("vert") && _tag.hasAttribute("frag")) {
      ShaderData data;
      data.vSrc = _filesys->loadTxt("Shaders/" + _tag.getAttribute("vert"));
      data.fSrc = _filesys->loadTxt("Shaders/" + _tag.getAttribute("frag"));
      loadData(data);
   }
}

void OpenGLShader::unload() {
   deleteShader();
}

void OpenGLShader::setToData(const ShaderData &data) {
   if (!loaded()) {
      // set attributes for reloading
      if (data.vFile != "" && data.fFile != "") {
         _tag.setAttribute("vert", data.vFile);
         _tag.setAttribute("frag", data.fFile);
      }
      
      loadData(data);
   }
}

void OpenGLShader::use() const {
  if (loaded() && _program) {
    glUseProgram(_program);
    Shader::use();
  }
}

void OpenGLShader::setUniforms(const Uniforms *unis) const {
   if (_program) {
      Uniforms::const_iterator itr;
      
      for (itr = unis->begin(); itr != unis->end(); ++itr)
         setUniform(itr->first, itr->second);
   }
}

void OpenGLShader::setAttributes(const Attributes *atts) const {
   if (_program) {
      GLsizei stride = (GLsizei)(atts->getStride()*sizeof(GLfloat));
      Attributes::const_iterator itr;
      
      for (itr = atts->begin(); itr != atts->end(); ++itr) {
         GLint loc = glGetAttribLocation(_program, itr->first.c_str());
         
         if (loc != -1) {
            const Attribute &att = itr->second;
            GLint size = (GLint)att.size;
            const GLvoid *offset = (const GLvoid*)(att.offset*sizeof(GLfloat));
            
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride, offset);
         }
      }
   }
}

void OpenGLShader::loadData(const ShaderData &data) {
   ShaderPart vertPart(GL_VERTEX_SHADER), fragPart(GL_FRAGMENT_SHADER);
   
   if (vertPart.compile(data.vSrc) && fragPart.compile(data.fSrc)) {
      GLint linkSuccess;
      
      _program = glCreateProgram();
      if (!_program) {
         cerr << "error creating shader program: ";
         cerr << getName() << endl;
         return;
      }
      
      vertPart.attach(_program);
      fragPart.attach(_program);
      glLinkProgram(_program);
      glGetProgramiv(_program, GL_LINK_STATUS, &linkSuccess);
      if (linkSuccess == GL_FALSE) {
         GLchar messages[MSG_SIZE];
         glGetProgramInfoLog(_program, sizeof(messages), 0, &messages[0]);
         
         vertPart.detatch(_program);
         fragPart.detatch(_program);
         _program = 0;
         cerr << messages << endl;
      }
      else {
         setTextures();
         
         vertPart.detatch(_program);
         fragPart.detatch(_program);

         _loaded = true;
         //cout << "loaded shader: " << getName() << endl;
      }
   }
}

void OpenGLShader::deleteShader() {
   if (loaded()) {
      glDeleteProgram(_program);
      _program = 0;
      _loaded = false;
      //cout << "deleted shader: " << getName() << endl;
   }
}

void OpenGLShader::setUniform(const string &name, const Uniform &uni) const {
   GLint loc = glGetUniformLocation(_program, name.c_str());
   if (loc != -1) {
      if (uni.type == VAL_INT)
         glUniform1iv(loc, uni.count, (const GLint*)uni.value);
      else if (uni.type == VAL_FLOAT)
         glUniform1fv(loc, uni.count, (const GLfloat*)uni.value);
      else if (uni.type == VAL_VEC2)
         glUniform2fv(loc, uni.count, (const GLfloat*)uni.value);
      else if (uni.type == VAL_VEC3)
         glUniform3fv(loc, uni.count, (const GLfloat*)uni.value);
      else if (uni.type == VAL_VEC4)
         glUniform4fv(loc, uni.count, (const GLfloat*)uni.value);
      else if (uni.type == VAL_MAT2X2)
         glUniformMatrix2fv(loc, uni.count, MTX_TRANSPOSE, (const GLfloat*)uni.value);
      else if (uni.type == VAL_MAT3X3)
         glUniformMatrix3fv(loc, uni.count, MTX_TRANSPOSE, (const GLfloat*)uni.value);
      else if (uni.type == VAL_MAT4X4)
         glUniformMatrix4fv(loc, uni.count, MTX_TRANSPOSE, (const GLfloat*)uni.value);
   }
}

void OpenGLShader::setTextures() {
   GLint loc, curProgram;
   int idx = 0;
   char name[10];
   
   if (_program) {
      glGetIntegerv(GL_CURRENT_PROGRAM, &curProgram);
      glUseProgram(_program);
      
      sprintf(name, "Sampler%d", idx);
      loc = glGetUniformLocation(_program, name);
      while (loc != -1) {
         glActiveTexture(GL_TEXTURE0 + idx);
         glUniform1i(loc, idx);
         sprintf(name, "Sampler%d", ++idx);
         loc = glGetUniformLocation(_program, name);
      }
      
      glUseProgram(curProgram);
   }
}



OpenGLShader::ShaderPart::~ShaderPart() {
   if (_shader)
      glDeleteShader(_shader);
}

bool OpenGLShader::ShaderPart::compile(std::string src) {
   const char *c_src = src.c_str();
   GLint compileSuccess;
   
   // create the shader
   _shader = glCreateShader(_type);
   if (!_shader) {
      cerr << "error creating shader" << endl;
      return false;
   }
   
   // compile the shader
   glShaderSource(_shader, 1, &c_src, 0);
   glCompileShader(_shader);
   glGetShaderiv(_shader, GL_COMPILE_STATUS, &compileSuccess);
   if (compileSuccess == GL_FALSE) {
      GLchar messages[MSG_SIZE];
      glGetShaderInfoLog(_shader, sizeof(messages), 0, &messages[0]);
      cerr << messages << endl;
      
      glDeleteShader(_shader);
      _shader = 0;
      return false;
   }
   return true;
}

void OpenGLShader::ShaderPart::attach(GLint program) {
   if (_shader)
      glAttachShader(program, _shader);
}

void OpenGLShader::ShaderPart::detatch(GLint program) {
   if (_shader)
      glDetachShader(program, _shader);
}