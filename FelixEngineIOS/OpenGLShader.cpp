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


OpenGLShader::OpenGLShader(const string &name): Shader(XMLTag("Shader")), _program(0) {
   _tag.setAttribute("name", name);
   Shaders[name] = this;
   cout << "create shader" << endl;
}

OpenGLShader::~OpenGLShader() {
   unload();
}

OpenGLShader* OpenGLShader::GetShader(const std::string &name) {
   if (Shaders.find(name) == Shaders.end())
      return new OpenGLShader(name);
   return Shaders[name];
}

void OpenGLShader::ClearShaders() {
   map<string, OpenGLShader*>::iterator itr;
   for (itr = Shaders.begin(); itr != Shaders.end(); ++itr)
      delete itr->second;
   Shaders.clear();
}

void OpenGLShader::load() {
   HostFileSystem *fs = Host::GetHost()->getFileSystem();
   
   if (!loaded()) {
      ShaderPart vertPart(GL_VERTEX_SHADER), fragPart(GL_FRAGMENT_SHADER);
      string vertSrc(fs->loadTxt("Shaders/" + _tag.getAttribute("vert")));
      string fragSrc(fs->loadTxt("Shaders/" + _tag.getAttribute("frag")));
      
      if (vertPart.compile(vertSrc) && fragPart.compile(fragSrc)) {
         GLint linkSuccess;
         
         _program = glCreateProgram();
         if (!_program) {
            cerr << "error creating shader program: ";
            cerr << _tag.getAttribute("name") << endl;
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
            cout << "loaded shader" << endl;
            
            vertPart.detatch(_program);
            fragPart.detatch(_program);
            Shader::load();
         }
      }
   }
   
   
}

void OpenGLShader::unload() {
   if (_program) {
      glDeleteProgram(_program);
      _program = 0;
   }
   Shader::unload();
}

void OpenGLShader::use() const {
   cout << "use shader: " << _tag.getAttribute("name") << endl;
   
   if (loaded() && _program) {
      glUseProgram(_program);
      Active = this;
   }
}

void OpenGLShader::setUniforms(const Uniforms &unis) const {
   if (_program) {
      Uniforms::const_iterator itr;
      
      for (itr = unis.begin(); itr != unis.end(); ++itr)
         setUniform(itr->first, itr->second);
   }
}

void OpenGLShader::setAttributes(const Attributes &atts) const {
   if (_program) {
      GLsizei stride = (GLsizei)atts.getStride();
      Attributes::const_iterator itr;
      
      for (itr = atts.begin(); itr != atts.end(); ++itr) {
         GLint loc = glGetAttribLocation(_program, itr->first.c_str());
         const Attribute &att = itr->second;
         
         if (loc != -1) {
            GLint size = (GLint)att.size;
            const GLvoid *offset = (const GLvoid*)(att.offset*sizeof(GLfloat));
            
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride, offset);
         }
      }
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