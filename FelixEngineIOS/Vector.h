//
//  Vector.h
//  TackyEngine
//
//  Created by Robert Crosby on 12/9/12.
//  Copyright (c) 2012 Robert Crosby. All rights reserved.
//

#ifndef TackyEngine_Vector_h
#define TackyEngine_Vector_h

#include <cmath>
#include <iostream>
#include <string>

const float Pi = 4 * std::atan(1.0f);
const float TwoPi = 2 * Pi;

template <typename T>
struct Vector2 {
   Vector2(): x(0), y(0) {}
   Vector2(T v): x(v), y(v) {}
   Vector2(T x, T y): x(x), y(y) {}
   
   template <typename I>
   Vector2(const Vector2<I> &v): x(v.x), y(v.y) {}
   
   template <typename I>
   void operator=(const Vector2<I> v) {x = v.x; y = v.y;}
   
   Vector2 yx() const {return Vector2(y, x);}
   T dot(const Vector2& v) const {return x * v.x + y * v.y;}
   Vector2 max(const Vector2 &v) const {
      Vector2 vec;
      vec.x = x < v.x ? v.x : x;
      vec.y = y < v.y ? v.y : y;
      return vec;
   }
   Vector2 min(const Vector2 &v) const {
      Vector2 vec;
      vec.x = x < v.x ? x : v.x;
      vec.y = y < v.y ? y : v.y;
      return vec;
   }
   Vector2 mod() const {
      Vector2 vec;
      vec.x = x<0 ? 1+(x-(long)x) : 1<x ? x-(long)x : x;
      vec.y = y<0 ? 1+(y-(long)y) : 1<y ? y-(long)y : y;
      return vec;
   }
   T *ptr() {return &x;}
   const T *ptr() const {return &x;}
   
   Vector2 operator+(const Vector2& v) const {return Vector2(x + v.x, y + v.y);}
   Vector2 operator-(const Vector2& v) const {return Vector2(x - v.x, y - v.y);}
   Vector2 operator*(const Vector2& v) const {return Vector2(x * v.x, y * v.y);}
   Vector2 operator/(const Vector2& v) const {
      if (v.x && v.y)
         return Vector2(x / v.x, y / v.y);
      std::cerr << "divide by zero" << std::endl;
      return *this;
   }
   
   Vector2 operator*(T s) const {return Vector2(x * s, y * s);}
   Vector2 operator/(T s) const {
      if (s)
         return Vector2(x / s, y / s);
      std::cerr << "divide by zero" << std::endl;
      return *this;
   }
   
   void operator+=(const Vector2& v) {*this = *this + v;}
   void operator-=(const Vector2& v) {*this = *this - v;}
   void operator*=(const Vector2& v) {*this = *this * v;}
   void operator/=(const Vector2& v) {*this = *this / v;}
   
   void operator*=(T s) {*this = *this * s;}
   void operator/=(T s) {*this = *this / s;}
   
   T lengthSquared()    const {return x * x + y * y;}
   T length()           const {return sqrt(lengthSquared());}
   Vector2 normalized() const {return *this / length();}
   
   void normalize() {this = *this / length();}
   
   bool operator==(const Vector2& v)   const {return x == v.x && y == v.y;}
   bool operator<(const Vector2& v)    const {return x == v.x ? y < v.y : x < v.x;}
   Vector2 lerp(T t, const Vector2& v) const {return *this * (1 - t) + v * t;}
   
   T distanceSquared(const Vector2& v) const {return (*this - v).LengthSquared();}
   T distance(const Vector2& v) const {return (*this - v).Length();}
   
   friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
      return os << "<" << v.x << ", " << v.y << ">";
   }
   
   static Vector2<float> ParseFloat(const std::string &str) {
      Vector2<float> v;
      int res = sscanf(str.c_str(), " %f , %f", &v.x, &v.y);
      return res == 2 ? v : v.x;
   }
   
   static bool IsPowerOfTwo(T val) {
      unsigned int iVal = val;
      return !(val - (T)iVal) && (iVal != 0) && !(iVal & (iVal - 1));
   }
   bool isPowerOfTwo() const {return IsPowerOfTwo(x) && IsPowerOfTwo(y);}
   
   union {T x; T r;};
   union {T y; T g;};
};

template <typename T>
struct Vector3 {
   Vector3(): x(0), y(0), z(0) {}
   Vector3(T v): x(v), y(v), z(v) {}
   Vector3(T x, T y, T z): x(x), y(y), z(z) {}
   Vector3(const Vector2<T> &v, T z): x(v.x), y(v.y), z(z) {}
   Vector3(T x, const Vector2<T> &v): x(x), y(v.y), z(v.z) {}
   
   template <typename I>
   Vector3(const Vector3<I> &v): x(v.x), y(v.y), z(v.z) {}
   
   template <typename I>
   void operator=(const Vector3<I> v) {x = v.x; y = v.y; z = v.z;}
   
   Vector2<T> xy() const {return Vector2<T>(x, y);}
   Vector2<T> yx() const {return Vector2<T>(y, x);}
   Vector3 zyx() const {return Vector3(z, y, x);}
   
   T dot(const Vector3& v) const {return x * v.x + y * v.y + z * v.z;}
   Vector3 cross(const Vector3& v) const {
      return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   Vector3 max(const Vector3 &v) const {
      Vector3 vec;
      vec.x = x < v.x ? v.x : x;
      vec.y = y < v.y ? v.y : y;
      vec.z = z < v.z ? v.z : z;
      return vec;
   }
   Vector3 min(const Vector3 &v) const {
      Vector3 vec;
      vec.x = x < v.x ? x : v.x;
      vec.y = y < v.y ? y : v.y;
      vec.z = z < v.z ? z : v.z;
      return vec;
   }
   T *ptr() {return &x;}
   const T *ptr() const {return &x;}
   
   Vector3 operator+(const Vector3& v) const {return Vector3(x + v.x, y + v.y, z + v.z);}
   Vector3 operator-(const Vector3& v) const {return Vector3(x - v.x, y - v.y, z - v.z);}
   Vector3 operator*(const Vector3& v) const {return Vector3(x * v.x, y * v.y, z * v.z);}
   Vector3 operator/(const Vector3& v) const {
      if (v.x && v.y && v.z)
         return Vector3(x / v.x, y / v.y, z / v.z);
      std::cerr << "divide by zero" << std::endl;
      return *this;
   }
   
   Vector3 operator*(T s) const {return Vector3(x * s, y * s, z * s);}
   Vector3 operator/(T s) const {
      if (s)
         return Vector3(x / s, y / s, z / s);
      std::cerr << "divide by zero" << std::endl;
      return *this;
   }
   Vector3 operator-() const {return Vector3(-x, -y, -z);}
   
   void operator+=(const Vector3& v) {*this = *this + v;}
   void operator-=(const Vector3& v) {*this = *this - v;}
   void operator*=(const Vector3& v) {*this = *this * v;}
   void operator/=(const Vector3& v) {*this = *this / v;}
   
   void operator*=(T s) {*this = *this * s;}
   void operator/=(T s) {*this = *this / s;}
   
   T lengthSquared()    const {return x * x + y * y + z * z;}
   T length()           const {return sqrt(lengthSquared());}
   Vector3 normalized() const {return *this / length();}
   
   void normalize() {*this /= length();}
   
   bool operator==(const Vector3& v)   const {return x == v.x && y == v.y && z == v.z;}
   bool operator<(const Vector3& v)    const {return x == v.x ? y == v.y ? z < v.z : y < v.y : x < v.x;}
   Vector3 lerp(T t, const Vector3& v) const {return *this * (1 - t) + v * t;}
   
   friend std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
      return os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
   }
   
   static Vector3<float> ParseFloat(const std::string &str) {
      Vector3<float> v;
      int res = sscanf(str.c_str(), " %f , %f , %f", &v.x, &v.y, &v.z);
      return res == 3 ? v : v.x;
   }
   static Vector3<int> ParseInt(const std::string &str) {
      Vector3<int> v;
      int res = sscanf(str.c_str(), " %i , %i , %i", &v.x, &v.y, &v.z);
      return res == 3 ? v : v.x;
   }
   
   union {T x; T r;};
   union {T y; T g;};
   union {T z; T b;};
};

template <typename T>
struct Vector4 {
   Vector4(): x(0), y(0), z(0), w(0) {}
   Vector4(T v): x(v), y(v), z(v), w(v) {}
   Vector4(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}
   
   Vector4(const Vector2<T> &v, T z, T w): x(v.x), y(v.y), z(z), w(w) {}
   Vector4(T x, const Vector2<T> &v, T w): x(x), y(v.y), z(v.z), w(w) {}
   Vector4(T x, T y, const Vector2<T> &v): x(x), y(y), z(v.z), w(v.w) {}
   Vector4(const Vector2<T> &v1, const Vector2<T> &v2): x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
   
   Vector4(const Vector3<T> &v, T w): x(v.x), y(v.y), z(v.z), w(w) {}
   Vector4(T x, const Vector3<T> &v): x(x), y(v.x), z(v.y), w(v.z) {}
   
   template <typename I>
   Vector4(const Vector4<I> &v): x(v.x), y(v.y), z(v.z), w(v.w) {}
   
   template <typename I>
   void operator=(const Vector4<I> v) {x = v.x; y = v.y; z = v.z; w = v.w;}
   
   Vector2<T> xy() const {return Vector2<T>(x, y);}
   Vector2<T> yx() const {return Vector2<T>(y, x);}
   Vector3<T> xyz() const {return Vector3<T>(x, y, z);}
   Vector3<T> zyx() const {return Vector3<T>(z, y, x);}
   Vector4 wzyx() const {return Vector4(w, z, y, x);}
   T dot(const Vector4& v) const {return x * v.x + y * v.y + z * v.z + w * v.w;}
   Vector4 max(const Vector4 &v) const {
      Vector4 vec;
      vec.x = x < v.x ? v.x : x;
      vec.y = y < v.y ? v.y : y;
      vec.z = z < v.z ? v.z : z;
      vec.w = w < v.w ? v.w : w;
      return vec;
   }
   Vector4 min(const Vector4 &v) const {
      Vector4 vec;
      vec.x = x < v.x ? x : v.x;
      vec.y = y < v.y ? y : v.y;
      vec.z = z < v.z ? y : v.z;
      vec.w = w < v.w ? w : v.w;
      return vec;
   }
   T *ptr() {return &x;}
   const T *ptr() const {return &x;}
   
   Vector4 operator+(const Vector4& v) const {return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);}
   Vector4 operator-(const Vector4& v) const {return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);}
   Vector4 operator*(const Vector4& v) const {return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);}
   Vector4 operator/(const Vector4& v) const {
      if (v.x && v.y && v.z && v.w)
         return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
      std::cerr << "divide by zero" << std::endl;
      return *this;
   }
   
   Vector4 operator*(T s) const {return Vector4(x * s, y * s, z * s, w * s);}
   Vector4 operator/(T s) const {
      if (s)
         return Vector4(x / s, y / s, z / s, w / s);
      std::cerr << "divide by zero" << std::endl;
      return *this;
   }
   
   void operator+=(const Vector4& v) {*this = *this + v;}
   void operator-=(const Vector4& v) {*this = *this - v;}
   void operator*=(const Vector4& v) {*this = *this * v;}
   void operator/=(const Vector4& v) {*this = *this / v;}
   
   void operator*=(T s) {*this = *this * s;}
   void operator/=(T s) {*this = *this / s;}
   
   T lengthSquared()    const {return x * x + y * y + z * z + w * w;}
   T length()           const {return sqrt(lengthSquared());}
   Vector4 normalized() const {return *this / length();}
   
   void normalize() {this = *this / length();}
   void swap(Vector4 &other) {
      Vector4 tmp = *this;
      *this = other;
      other = tmp;
   }
   
   bool operator==(const Vector4& v) const {
      return x == v.x && y == v.y && z == v.z && w == v.w;
   }
   bool operator<(const Vector4& v) const {
      return x == v.x ? y == v.y ? z == v.z ? w < v.w : z < v.z : y < v.y : x < v.x;}
   Vector4 lerp(T t, const Vector4& v) const {return *this * (1 - t) + v * t;}
   
   friend std::ostream &operator<<(std::ostream &os, const Vector4 &v) {
      return os << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
   }
   
   static Vector4<float> ParseFloat(const std::string &str) {
      Vector4<float> v;
      int res = sscanf(str.c_str(), " %f , %f , %f , %f", &v.x, &v.y, &v.z, &v.w);
      return res == 4 ? v : v.x;
   }
   
   union {T x; T r;};
   union {T y; T g;};
   union {T z; T b;};
   union {T w; T a;};
};
         
typedef Vector2<int> ivec2;
typedef Vector3<int> ivec3;
typedef Vector4<int> ivec4;
      
typedef Vector2<float> vec2;
typedef Vector3<float> vec3;
typedef Vector4<float> vec4;

#endif
