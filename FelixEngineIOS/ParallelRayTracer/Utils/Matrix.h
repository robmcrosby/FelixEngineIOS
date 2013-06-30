//
//  Matrix.h
//  TackyEngine
//
//  Created by Robert Crosby on 12/9/12.
//  Copyright (c) 2012 Robert Crosby. All rights reserved.
//

#ifndef TackyEngine_Matrix_h
#define TackyEngine_Matrix_h

#include "Vector.h"

#define SWAP_ROWS(a, b) {float *tmp = a; a = b; b = tmp;}

template <typename T>
struct Matrix2 {
   Matrix2(): x(1, 0), y(0, 1) {}
   Matrix2(T s): x(s, 0), y(0, s) {}
   Matrix2(const T* m) {
      x.x = m[0]; x.y = m[1];
      y.x = m[2]; y.y = m[3];
   }
   template <typename I>
   Matrix2(const Matrix2<I> &m): x(m.x), y(m.y) {}
   
   Matrix2 transposed() const {
      Matrix2 m = *this;
      m.x.y = y.x;
      m.y.x = x.y;
      return m;
   }
   Matrix2 scaled(T s) const {
      Matrix2 m = *this;
      m.x.x *= s;
      m.y.y *= s;
      return m;
   }
   template <typename I>
   Matrix2 scaled(const Vector2<I> &s) const {
      Matrix2 m = *this;
      m.x.x *= s.x;
      m.y.y *= s.y;
      return m;
   }
   Matrix2 operator*(const Matrix2 &m) const {
      Matrix2 ret;
      ret.x.x = x.x*m.x.x + y.x*m.x.y;
      ret.y.x = x.x*m.y.x + y.x*m.y.y;
      ret.x.y = x.y*m.x.x + y.y*m.x.y;
      ret.y.y = x.y*m.y.x + y.y*m.y.y;
      return ret;
   }
   template <typename I>
   Vector2<I> operator*(const Vector2<I> &b) const {
      Vector2<I> v;
      v.x = x.x*b.x + y.x*b.y;
      v.y = x.y*b.x + y.y*b.y;
      return v;
   }
   bool operator==(const Matrix2 &m) const {
      return x == m.x && y == m.y;
   }
   bool operator<(const Matrix2 &m) const {
      return x == m.x ? y < m.y : x < m.x;
   }
   void operator*=(const Matrix2 &m) {*this = *this * m;}
   
   friend std::ostream &operator<<(std::ostream &os, const Matrix2 &m) {
      os << "[" << m.x.x << ", " << m.y.x << "]" << std::endl;
      os << "[" << m.x.y << ", " << m.y.y << "]" << std::endl;
      return os;
   }
   
   Vector2<T> x;
   Vector2<T> y;
};

template <typename T>
struct Matrix3 {
   Matrix3(): x(1, 0, 0), y(0, 1, 0), z(0, 0, 1) {}
   Matrix3(T s): x(s, 0, 0), y(0, s, 0), z(0, 0, s) {}
   Matrix3(const T* m) {
      x.x = m[0]; x.y = m[1]; x.z = m[2];
      y.x = m[3]; y.y = m[4]; y.z = m[5];
      z.x = m[6]; z.y = m[7]; z.z = m[8];
   }
   template <typename I>
   Matrix3(const Matrix2<I> &m): x(m.x, 0), y(m.y, 0), z(0, 0, 1) {}
   template <typename I>
   Matrix3(const Matrix3<I> &m): x(m.x), y(m.y), z(m.z) {}
   
   
   Matrix3 transposed() const {
      Matrix3 m = *this;
      m.x.y = y.x;
      m.x.z = z.x;
      m.y.x = x.y;
      m.y.z = z.y;
      m.z.x = x.z;
      m.z.y = y.z;
      return m;
   }
   Matrix3 scaled(T s) const {
      Matrix3 m = *this;
      m.x.x *= s;
      m.y.y *= s;
      m.z.z *= s;
      return m;
   }
   template <typename I>
   Matrix3 scaled(const Vector3<I> &s) const {
      Matrix3 m = *this;
      m.x.x *= s.x;
      m.y.y *= s.y;
      m.z.z *= s.z;
      return m;
   }
   Matrix2<T> toMat2() const {
      Matrix2<T> m;
      m.x = x.XY();
      m.y = y.XY();
      return m;
   }
   Matrix3 operator*(const Matrix3 &m) const {
      Matrix3 ret;
      ret.x.x = x.x*m.x.x + y.x*m.x.y + z.x*m.x.z;
      ret.y.x = x.x*m.y.x + y.x*m.y.y + z.x*m.y.z;
      ret.z.x = x.x*m.z.x + y.x*m.z.y + z.x*m.z.z;
      
      ret.x.y = x.y*m.x.x + y.y*m.x.y + z.y*m.x.z;
      ret.y.y = x.y*m.y.x + y.y*m.y.y + z.y*m.y.z;
      ret.z.y = x.y*m.z.x + y.y*m.z.y + z.y*m.z.z;
      
      ret.x.z = x.z*m.x.x + y.z*m.x.y + z.z*m.x.z;
      ret.y.z = x.z*m.y.x + y.z*m.y.y + z.z*m.y.z;
      ret.z.z = x.z*m.z.x + y.z*m.z.y + z.z*m.z.z;
      return ret;
   }
   template <typename I>
   Vector3<I> operator*(const Vector3<I> &b) const {
      Vector3<I> v;
      v.x = x.x*b.x + y.x*b.y + z.x*b.z;
      v.y = x.y*b.x + y.y*b.y + z.y*b.z;
      v.z = x.z*b.x + y.z*b.y + z.z*b.z;
      return v;
   }
   bool operator==(const Matrix3 &m) const {
      return x == m.x && y == m.y && z == m.z;
   }
   bool operator<(const Matrix3 &m) const {
      return x == m.x ? y == m.y ? z < m.z : y < m.y : x < m.x;
   }
   void operator*=(const Matrix3 &m) {*this = *this * m;}
   
   static Matrix3 RotX(T r) {
      Matrix3 m;
      m.y.y = m.z.z = cos(r);
      m.y.z = sin(r);
      m.z.y = -m.y.z;
      return m;
   }
   static Matrix3 RotY(T r) {
      Matrix3 m;
      m.x.x = m.z.z = cos(r);
      m.z.x = sin(r);
      m.x.z = -m.z.x;
      return m;
   }
   static Matrix3 RotZ(T r) {
      Matrix3 m;
      m.x.x = m.y.y = cos(r);
      m.x.y = sin(r);
      m.y.x = -m.x.y;
      return m;
   }
   static Matrix3 Rotate(Vector3<T> r) {
      Matrix3 m = RotX(r.x);
      m = RotY(r.y) * m;
      m = RotZ(r.z) * m;
      return m;
   }
   static Matrix3 Trans2d(Vector2<T> t) {
      Matrix3 m;
      m.z = Vector3<T>(t, 1);
      return m;
   }
   static Matrix3 Scale(Vector2<T> s) {
      Matrix3 m;
      m.x.x = s.x;
      m.y.y = s.y;
      return m;
   }
   static Matrix3 Scale(Vector3<T> s) {
      Matrix3 m;
      m.x.x = s.x;
      m.y.y = s.y;
      m.z.z = s.z;
      return m;
   }
   
   friend std::ostream &operator<<(std::ostream &os, const Matrix3 &m) {
      os << "[" << m.x.x << ", " << m.y.x << ", " << m.z.x << "]" << std::endl;
      os << "[" << m.x.y << ", " << m.y.y << ", " << m.z.y << "]" << std::endl;
      os << "[" << m.x.z << ", " << m.y.z << ", " << m.z.z << "]" << std::endl;
      return os;
   }
   
   Vector3<T> x;
   Vector3<T> y;
   Vector3<T> z;
};

template <typename T>
struct Matrix4 {
   Matrix4(): x(1, 0, 0, 0), y(0, 1, 0, 0), z(0, 0, 1, 0), w(0, 0, 0, 1) {}
   Matrix4(T s): x(s, 0, 0, 0), y(0, s, 0, 0), z(0, 0, s, 0), w(0, 0, 0, 1) {}
   Matrix4(const T* m) {
      x.x = m[0]; x.y = m[1]; x.z = m[2]; x.w = m[3];
      y.x = m[4]; y.y = m[5]; y.z = m[6]; y.w = m[7];
      z.x = m[8]; z.y = m[9]; z.z = m[10]; z.w = m[11];
      w.x = m[12]; w.y = m[13]; w.z = m[14]; w.w = m[15];
   }
   template <typename I>
   Matrix4(const Matrix2<I> &m): x(m.x, 0, 0), y(m.y, 0, 0), z(0, 0, 1, 0), w(0, 0, 0, 1) {}
   template <typename I>
   Matrix4(const Matrix3<I> &m): x(m.x, 0), y(m.y, 0), z(m.z, 0), w(0, 0, 0, 1) {}
   template <typename I>
   Matrix4(const Matrix4<I> &m): x(m.x), y(m.y), z(m.z) {}
   
   Matrix4 transposed() const {
      Matrix4 m = *this;
      m.x.y = y.x;
      m.x.z = z.x;
      m.x.w = w.x;
      
      m.y.x = x.y;
      m.y.z = z.y;
      m.y.w = w.y;
      
      m.z.x = x.z;
      m.z.y = y.z;
      m.z.w = w.z;
      
      m.w.x = x.w;
      m.w.y = y.w;
      m.w.z = z.w;
      return m;
   }
   
   // I don't like this function but it works.
   Matrix4 inverse() const {
      float tmp[4][8];
      float m0, m1, m2, m3, s;
      float *r0, *r1, *r2, *r3;
      Matrix4 m;
      
      r0 = tmp[0];
      r1 = tmp[1];
      r2 = tmp[2];
      r3 = tmp[3];
      
      r0[0] = x.x;
      r0[1] = x.y;
      r0[2] = x.z;
      r0[3] = x.w;
      r0[4] = 1.0;
      r0[5] = r0[6] = r0[7] = 0.0;
      r1[0] = y.x;
      r1[1] = y.y;
      r1[2] = y.z;
      r1[3] = y.w;
      r1[5] = 1.0;
      r1[4] = r1[6] = r1[7] = 0.0;
      r2[0] = z.x;
      r2[1] = z.y;
      r2[2] = z.z;
      r2[3] = z.w;
      r2[6] = 1.0;
      r2[4] = r2[5] = r2[7] = 0.0;
      r3[0] = w.x;
      r3[1] = w.y;
      r3[2] = w.z;
      r3[3] = w.w;
      r3[7] = 1.0;
      r3[4] = r3[5] = r3[6] = 0.0;
      
      //Chose first pivit
      if (fabsf(r3[0]) > fabsf(r2[0]))
         SWAP_ROWS(r3, r2);
      if (fabsf(r2[0]) > fabsf(r1[0]))
         SWAP_ROWS(r2, r1);
      if (fabsf(r1[0]) > fabsf(r0[0]))
         SWAP_ROWS(r1, r0);
      if (r0[0] == 0.0) {
         return m;
      }
      //Elimate first varible
      m1 = r1[0] / r0[0];
      m2 = r2[0] / r0[0];
      m3 = r3[0] / r0[0];
      s = r0[1];
      r1[1] -= m1 * s;
      r2[1] -= m2 * s;
      r3[1] -= m3 * s;
      s = r0[2];
      r1[2] -= m1 * s;
      r2[2] -= m2 * s;
      r3[2] -= m3 * s;
      s = r0[3];
      r1[3] -= m1 * s;
      r2[3] -= m2 * s;
      r3[3] -= m3 * s;
      s = r0[4];
      if (s != 0.0) {
         r1[4] -= m1 * s;
         r2[4] -= m2 * s;
         r3[4] -= m3 * s;
      }
      s = r0[5];
      if (s != 0.0) {
         r1[5] -= m1 * s;
         r2[5] -= m2 * s;
         r3[5] -= m3 * s;
      }
      s = r0[6];
      if (s != 0.0) {
         r1[6] -= m1 * s;
         r2[6] -= m2 * s;
         r3[6] -= m3 * s;
      }
      s = r0[7];
      if (s != 0.0) {
         r1[7] -= m1 * s;
         r2[7] -= m2 * s;
         r3[7] -= m3 * s;
      }
      //Next Pivit
      if (fabsf(r3[1]) > fabsf(r2[1]))
         SWAP_ROWS(r3, r2);
      if (fabsf(r2[1]) > fabsf(r1[1]))
         SWAP_ROWS(r2, r1);
      if (r1[1] == 0.0)
         return m;
      //Eliminate the next varible
      m2 = r2[1] / r1[1];
      m3 = r3[1] / r1[1];
      r2[2] -= m2 * r1[2];
      r3[2] -= m3 * r1[2];
      r2[3] -= m2 * r1[3];
      r3[3] -= m3 * r1[3];
      s = r1[4];
      if (0.0 != s) {
         r2[4] -= m2 * s;
         r3[4] -= m3 * s;
      }
      s = r1[5];
      if (0.0 != s) {
         r2[5] -= m2 * s;
         r3[5] -= m3 * s;
      }
      s = r1[6];
      if (0.0 != s) {
         r2[6] -= m2 * s;
         r3[6] -= m3 * s;
      }
      s = r1[7];
      if (0.0 != s) {
         r2[7] -= m2 * s;
         r3[7] -= m3 * s;
      }
      //Chose the last pivit
      if (fabsf(r3[2]) > fabsf(r2[2]))
         SWAP_ROWS(r3, r2);
      if (r2[2] == 0.0)
         return m;
      //Eliminate the thrid varible
      m3 = r3[2] / r2[2];
      r3[3] -= m3 * r2[3];
      r3[4] -= m3 * r2[4];
      r3[5] -= m3 * r2[5];
      r3[6] -= m3 * r2[6];
      r3[7] -= m3 * r2[7];
      //Check one last time
      if (r3[3] == 0.0)
         return m;
      //Back substitute row 3
      s = 1.0 / r3[3];
      r3[4] *= s;
      r3[5] *= s;
      r3[6] *= s;
      r3[7] *= s;
      //Back substitue row 2
      m2 = r2[3];
      s = 1.0 / r2[2];
      r2[4] = s * (r2[4] - r3[4] * m2);
      r2[5] = s * (r2[5] - r3[5] * m2);
      r2[6] = s * (r2[6] - r3[6] * m2);
      r2[7] = s * (r2[7] - r3[7] * m2);
      m1 = r1[3];
      r1[4] -= r3[4] * m1;
      r1[5] -= r3[5] * m1;
      r1[6] -= r3[6] * m1;
      r1[7] -= r3[7] * m1;
      m0 = r0[3];
      r0[4] -= r3[4] * m0;
      r0[5] -= r3[5] * m0;
      r0[6] -= r3[6] * m0;
      r0[7] -= r3[7] * m0;
      //Back substitue row 1
      m1 = r1[2];
      s = 1.0 / r1[1];
      r1[4] = s * (r1[4] - r2[4] * m1);
      r1[5] = s * (r1[5] - r2[5] * m1);
      r1[6] = s * (r1[6] - r2[6] * m1);
      r1[7] = s * (r1[7] - r2[7] * m1);
      m0 = r0[2];
      r0[4] -= r2[4] * m0;
      r0[5] -= r2[5] * m0;
      r0[6] -= r2[6] * m0;
      r0[7] -= r2[7] * m0;
      //Back substitue row 0
      m0 = r0[1];
      s = 1.0 / r0[0];
      r0[4] = s * (r0[4] - r1[4] * m0);
      r0[5] = s * (r0[5] - r1[5] * m0);
      r0[6] = s * (r0[6] - r1[6] * m0);
      r0[7] = s * (r0[7] - r1[7] * m0);
      //Assign values to the return matrix
      m.x.x = r0[4];
      m.x.y = r0[5];
      m.x.z = r0[6];
      m.x.w = r0[7];
      m.y.x = r1[4];
      m.y.y = r1[5];
      m.y.z = r1[6];
      m.y.w = r1[7];
      m.z.x = r2[4];
      m.z.y = r2[5];
      m.z.z = r2[6];
      m.z.w = r2[7];
      m.w.x = r3[4];
      m.w.y = r3[5];
      m.w.z = r3[6];
      m.w.w = r3[7];
      return m;
   }
   Matrix4 scaled(T s) const {
      Matrix4 m = *this;
      m.x.x *= s;
      m.y.y *= s;
      m.z.z *= s;
      return m;
   }
   template <typename I>
   Matrix4 scaled(const Vector3<I> &s) const {
      Matrix4 m = *this;
      m.x.x *= s.x;
      m.y.y *= s.y;
      m.z.z *= s.z;
      return m;
   }
   Matrix2<T> toMat2() const {
      Matrix2<T> m;
      m.x = x.XY();
      m.y = y.XY();
      return m;
   }
   Matrix3<T> toMat3() const {
      Matrix3<T> m;
      m.x = x.xyz();
      m.y = y.xyz();
      m.z = z.xyz();
      return m;
   }
   Matrix4 operator*(const Matrix4 &m) const {
      Matrix4 ret;
      ret.x.x = x.x*m.x.x + y.x*m.x.y + z.x*m.x.z + w.x*m.x.w;
      ret.y.x = x.x*m.y.x + y.x*m.y.y + z.x*m.y.z + w.x*m.y.w;
      ret.z.x = x.x*m.z.x + y.x*m.z.y + z.x*m.z.z + w.x*m.z.w;
      ret.w.x = x.x*m.w.x + y.x*m.w.y + z.x*m.w.z + w.x*m.w.w;
      
      ret.x.y = x.y*m.x.x + y.y*m.x.y + z.y*m.x.z + w.y*m.x.w;
      ret.y.y = x.y*m.y.x + y.y*m.y.y + z.y*m.y.z + w.y*m.y.w;
      ret.z.y = x.y*m.z.x + y.y*m.z.y + z.y*m.z.z + w.y*m.z.w;
      ret.w.y = x.y*m.w.x + y.y*m.w.y + z.y*m.w.z + w.y*m.w.w;
      
      ret.x.z = x.z*m.x.x + y.z*m.x.y + z.z*m.x.z + w.z*m.x.w;
      ret.y.z = x.z*m.y.x + y.z*m.y.y + z.z*m.y.z + w.z*m.y.w;
      ret.z.z = x.z*m.z.x + y.z*m.z.y + z.z*m.z.z + w.z*m.z.w;
      ret.w.z = x.z*m.w.x + y.z*m.w.y + z.z*m.w.z + w.z*m.w.w;
      
      ret.x.w = x.w*m.x.x + y.w*m.x.y + z.w*m.x.z + w.w*m.x.w;
      ret.y.w = x.w*m.y.x + y.w*m.y.y + z.w*m.y.z + w.w*m.y.w;
      ret.z.w = x.w*m.z.x + y.w*m.z.y + z.w*m.z.z + w.w*m.z.w;
      ret.w.w = x.w*m.w.x + y.w*m.w.y + z.w*m.w.z + w.w*m.w.w;
      return ret;
   }
   template <typename I>
   Vector4<I> operator*(const Vector4<I> &b) const {
      Vector4<I> v;
      v.x = x.x*b.x + y.x*b.y + z.x*b.z + w.x*b.w;
      v.y = x.y*b.x + y.y*b.y + z.y*b.z + w.y*b.w;
      v.z = x.z*b.x + y.z*b.y + z.z*b.z + w.z*b.w;
      v.w = x.w*b.x + y.w*b.y + z.w*b.z + w.w*b.w;
      return v;
   }
   template <typename I>
   Vector3<I> operator*(const Vector3<I> &b) const {
      Vector4<I> v = *this * Vector4<I>(b, 1);
      return v.xyz()/v.w;
   }
   bool operator==(const Matrix4 &m) const {
      return x == m.x && y == m.y && z == m.z && w == m.w;
   }
   bool operator<(const Matrix4 &m) const {
      return x == m.x ? y == m.y ? z == m.z ? w < m.w : z < m.z : y < m.y : x < m.x;
   }
   void operator*=(const Matrix4 &m) {*this = *this * m;}
   
   static Matrix4 RotX(T r) {
      Matrix4 m;
      m.y.y = m.z.z = cos(r);
      m.y.z = sin(r);
      m.z.y = -m.y.z;
      return m;
   }
   static Matrix4 RotY(T r) {
      Matrix4 m;
      m.x.x = m.z.z = cos(r);
      m.z.x = sin(r);
      m.x.z = -m.z.x;
      return m;
   }
   static Matrix4 RotZ(T r) {
      Matrix4 m;
      m.x.x = m.y.y = cos(r);
      m.x.y = sin(r);
      m.y.x = -m.x.y;
      return m;
   }
   static Matrix4 Rotate(Vector3<T> r) {
      Matrix4 m = RotX(r.x);
      m = RotY(r.y) * m;
      m = RotZ(r.z) * m;
      return m;
   }
   
   static Matrix4 Trans3d(Vector3<T> t) {
      Matrix4 m;
      m.w = Vector4<T>(t, 1);
      return m;
   }
   static Matrix4 Scale(Vector2<T> s) {
      Matrix4 m;
      m.x.x = s.x;
      m.y.y = s.y;
      return m;
   }
   static Matrix4 Scale(Vector3<T> s) {
      Matrix4 m;
      m.x.x = s.x;
      m.y.y = s.y;
      m.z.z = s.z;
      return m;
   }
   static Matrix4 Scale(Vector4<T> s) {
      Matrix4 m;
      m.x.x = s.x;
      m.y.y = s.y;
      m.z.z = s.z;
      m.w.w = s.w;
      return m;
   }
   static Matrix4 Frustum(T left, T right, T bottom, T top, T near, T far) {
    	T a = (2 * near) / (right - left);
      T b = (2 * near) / (top - bottom);
      T c = (right + left) / (right - left);
      T d = (top + bottom) / (top - bottom);
      T e = -(far + near) / (far - near);
      T f = (-2 * far * near) / (far - near);
      
      Matrix4 m;
      m.x = Vector4<T>(a, 0, 0, 0);
      m.y = Vector4<T>(0, b, 0, 0);
      m.z = Vector4<T>(c, d, e, -1);
      m.w = Vector4<T>(0, 0, f, 1);
      return m;
   }
   static Matrix4 Ortho(T left, T right, T bottom, T top, T near, T far) {
    	T a = 2 / (right - left);
      T b = 2 / (top - bottom);
      T c = -2 / (far - near);
      T d = -(right + left) / (right - left);
      T e = -(top + bottom) / (top - bottom);
      T f = -(far + near) / (far - near);
      
      Matrix4 m;
      m.x = Vector4<T>(a, 0, 0, 0);
      m.y = Vector4<T>(0, b, 0, 0);
      m.z = Vector4<T>(0, 0, c, 0);
      m.w = Vector4<T>(d, e, f, 1);
      return m;
   }
   static Matrix4<T> Magic(const Vector3<T> &fwd, const Vector3<T> &up, const Vector3<T> &pos) {
      vec3 f = fwd.normalized();
      vec3 s = fwd.cross(up).normalized();
      vec3 u = s.cross(f);
      
      Matrix4 m;
      m.x.x = s.x; m.y.x = u.x; m.z.x = f.x;
      m.x.y = s.y; m.y.y = u.y; m.z.y = f.y;
      m.x.z = s.z; m.y.z = u.z; m.z.z = f.z;
      
      return Trans3d(pos) * m;
   }
   
   friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m) {
      os << "[" << m.x.x << ", " << m.y.x << ", " << m.z.x << ", " << m.w.x << "]" << std::endl;
      os << "[" << m.x.y << ", " << m.y.y << ", " << m.z.y << ", " << m.w.y << "]" << std::endl;
      os << "[" << m.x.z << ", " << m.y.z << ", " << m.z.z << ", " << m.w.z << "]" << std::endl;
      os << "[" << m.x.w << ", " << m.y.w << ", " << m.z.w << ", " << m.w.w << "]" << std::endl;
      return os;
   }
   
   Vector4<T> x;
   Vector4<T> y;
   Vector4<T> z;
   Vector4<T> w;
};

typedef Matrix2<double> mat2;
typedef Matrix3<double> mat3;
typedef Matrix4<double> mat4;

#endif
