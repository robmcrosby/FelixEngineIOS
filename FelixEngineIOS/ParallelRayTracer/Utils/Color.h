//
//  Color.h
//  FirstRays
//
//  Created by Robert Crosby on 3/27/13.
//  Copyright (c) 2013 Robert Crosby. All rights reserved.
//

#ifndef FirstRays_Color_h
#define FirstRays_Color_h

#include "Vector.h"
//#include "MyLib.h"

enum COLOR {
   COLOR_BLACK = 0x000000,
   COLOR_WHITE = 0xffffff,
   COLOR_RED   = 0xff0000,
   COLOR_GREEN = 0x00ff00,
   COLOR_BLUE  = 0x0000ff,
};

template <typename T>
struct Color3: public Vector3<T> {
   Color3() : Vector3<T>(0) {}
   Color3(T r, T g, T b): Vector3<T>(r, g, b) {}
   Color3(unsigned int c) {
      //c = EndianXfer(c);
      unsigned char *cp = (unsigned char *)&c;
      Vector3<T>::r = *++cp;
      Vector3<T>::g = *++cp;
      Vector3<T>::b = *++cp;
   }
   
   template <typename I>
   Color3(const Vector3<I> &v): Vector3<T>(v) {}
   
   operator Color3<unsigned char>() const {return Color3<unsigned char>(*this);}
   operator Color3<double>() const {return Color3<double>(*this);}
      
   Color3<double> toCol() const {return Color3<double>(*this/255.0f);}
   Color3<unsigned char> toPix() const {
      Color3<unsigned char> color;
      color.r = Vector3<T>::r < 0.0f ? 0 : Vector3<T>::r > 1.0f ? 255 : Vector3<T>::r * 255;
      color.g = Vector3<T>::g < 0.0f ? 0 : Vector3<T>::g > 1.0f ? 255 : Vector3<T>::g * 255;
      color.b = Vector3<T>::b < 0.0f ? 0 : Vector3<T>::b > 1.0f ? 255 : Vector3<T>::b * 255;
      return color;
   }
   
   void setPixel(unsigned char *pixel) {
      Vector3<T>::r = pixel[0];
      Vector3<T>::g = pixel[1];
      Vector3<T>::b = pixel[2];
   }
      
   void getPixel(unsigned char *pixel) {
      pixel[0] = Vector3<T>::r;
      pixel[1] = Vector3<T>::g;
      pixel[2] = Vector3<T>::b;
      pixel[3] = 255;
   }
   
   friend std::ostream &operator<<(std::ostream &os, const Color3 &c) {
      return os << "(" << (double)c.r << ", " << (double)c.g << ", " << (double)c.b << ")";
   }
};


template <typename T>
struct Color4: public Vector4<T> {
   Color4(T a = 255): Vector4<T>(0, 0, 0, a) {}
   Color4(T r, T g, T b, T a): Vector4<T>(r, g, b, a) {}
   Color4(Color3<T> c, T a = 255): Vector4<T>(c.r, c.g, c.b, a) {}
   Color4(COLOR c) {
      //uint uc = EndianXfer((uint)c);
      unsigned int uc = (unsigned int)c;
      unsigned char *cp = (unsigned char *)&uc;
      Vector4<T>::r = *++cp;
      Vector4<T>::g = *++cp;
      Vector4<T>::b = *++cp;
      Vector4<T>::a = 255;
   }
   
   template <typename I>
   Color4(const Vector4<I> &v): Vector4<T>(v) {}
   
   operator Color4<unsigned char>() const {return Color4<unsigned char>(*this);}
   operator Color4<double>() const {return Color4<double>(*this);}
   
   Color4<double> toCol() const {
      Color4<double> c;
      c.r = this->r/255.0;
      c.g = this->g/255.0;
      c.b = this->b/255.0;
      c.a = this->a/255.0;
      return c;
   }
   Color4<unsigned char> toPix() const {
      Color4<unsigned char> color;
      color.r = Vector4<T>::r < 0.0f ? 0 : Vector4<T>::r > 1.0f ? 255 : Vector4<T>::r * 255;
      color.g = Vector4<T>::g < 0.0f ? 0 : Vector4<T>::g > 1.0f ? 255 : Vector4<T>::g * 255;
      color.b = Vector4<T>::b < 0.0f ? 0 : Vector4<T>::b > 1.0f ? 255 : Vector4<T>::b * 255;
      color.a = Vector4<T>::a < 0.0f ? 0 : Vector4<T>::a > 1.0f ? 255 : Vector4<T>::a * 255;
      return color;
   }
   
   void readPixel(unsigned char *pixel) {
      Vector4<T>::r = pixel[0];
      Vector4<T>::g = pixel[1];
      Vector4<T>::b = pixel[2];
      Vector4<T>::a = pixel[3];
   }
   
   void writePixel(unsigned char *pixel) {
      pixel[0] = Vector4<T>::r;
      pixel[1] = Vector4<T>::g;
      pixel[2] = Vector4<T>::b;
      pixel[3] = Vector4<T>::a;
   }
   
   Color3<T> rgb() const {return Vector4<T>::xyz();}
   Color4 abgr() const {return Vector4<T>::wzyx();}
   
   friend std::ostream &operator<<(std::ostream &os, const Color4 &c) {
      return os << "(" << (double)c.r << ", " << (double)c.g << ", " << (double)c.b << ", " << (double)c.a << ")";
   }
};

typedef Color3<unsigned char> pix3;
typedef Color4<unsigned char> pix4;

typedef Color3<double> col3;
typedef Color4<double> col4;


#endif
