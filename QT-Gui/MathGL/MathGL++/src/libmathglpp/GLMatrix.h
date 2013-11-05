/***************************************************************************
 *   Copyright (C) 2004 by Jacques Gasselin                                *
 *   jacquesgasselin@hotmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef GL_MATRIX_H
#define GL_MATRIX_H



#include "GLVector.h"
#include <cstring>
#include <cassert>

#ifdef GLMATRIX_IOSTREAM
#include <iostream>
#endif

//!column major matrix class for OpenGL
template <typename T>
class GLMatrix
{
public:
    //!Create an uninitialised matrix
    GLMatrix()
    { }

    //!Create an initialised matrix
    GLMatrix(T val)
    { m0=m1=m2=m3=m4=m5=m6=m7=m8=m9=m10=m11=m12=m13=m14=m15=val; }

    //!Create a matrix from an array*/
    GLMatrix(const T* val)
    { memmove(m,val,16*sizeof(T)); }

    //!Copy a matrix
    GLMatrix(const GLMatrix& mat)
    { memmove(m,mat.m,16*sizeof(T)); }


    //!Default destructor
    ~GLMatrix()
    {}

    //!Get the matrix determinant
    inline T det() const { return m0*cofactorm0() - m1*cofactorm1() + m2*cofactorm2() - m3*cofactorm3(); }

    //!Get the adjoint matrix
    GLMatrix adjoint() const
    {
        GLMatrix ret;

        ret[0] = cofactorm0();
        ret[1] = -cofactorm4();
        ret[2] = cofactorm8();
        ret[3] = -cofactorm12();

        ret[4] = -cofactorm1();
        ret[5] = cofactorm5();
        ret[6] = -cofactorm9();
        ret[7] = cofactorm13();

        ret[8] = cofactorm2();
        ret[9] = -cofactorm6();
        ret[10] = cofactorm10();
        ret[11] = -cofactorm14();

        ret[12] = -cofactorm3();
        ret[13] = cofactorm7();
        ret[14] = -cofactorm11();
        ret[15] = cofactorm15();

        return ret;
    }

    //!Adjoint method inverse, constant time inversion implementation
    GLMatrix inverse() const
    {
        GLMatrix ret(adjoint());

        T determinant = m0*ret[0] + m1*ret[4] + m2*ret[8] + m3*ret[12];
        assert(determinant!=0 && "Singular matrix has no inverse");

        ret/=determinant;
        return ret;
    }


    //!Assign this matrix from another one
    inline GLMatrix& operator= (const GLMatrix& mat)
    { memmove(m,mat.m,16*sizeof(T)); return *this; }

    //!Equality check. NB. May not be constant time, depending on memcmp.
    inline bool operator== (const GLMatrix& mat)
    { return memcmp(m,mat.m,16*sizeof(T))==0;  }

    //!Direct access to the matrix elements, just remember they are in column major format!!
    inline T& operator[] (int ind)
    { assert(ind > -1 && ind < 16); return m[ind]; }

    //!Implicit cast vector access as suggested by maquinn
    inline operator const T*(void) const { return m; }

    //!Implicit cast vector access as suggested by maquinn
    inline operator T*(void) { return m; }

    //!Multiply this matrix by a scalar
    inline GLMatrix& operator*= (T val)
    { for(register unsigned i = 0; i < 16; ++i) m[i] *= val; return *this; }

    //!Divide this matrix by a scalar
    inline GLMatrix& operator/= (T val)
    { for(register unsigned i = 0; i < 16; ++i) m[i] /= val; return *this; }

    //!Add a matrix to this matrix
    inline GLMatrix& operator+= (const GLMatrix& mat)
    { for(register unsigned i = 0; i < 16; ++i) m[i] += mat.m[i]; return *this; }

    //!Subtract a matrix from this matrix
    inline GLMatrix& operator-= (const GLMatrix& mat)
    { for(register unsigned i = 0; i < 16; ++i) m[i] -= mat.m[i]; return *this; }

    //!Get the matrix dot product, most commonly used form of matrix multiplication
    inline GLMatrix operator* (const GLMatrix& mat)
    {
        GLMatrix ret;
        for(register unsigned j = 0; j < 4; ++j)
        {
            ret.m[j] = m[j]*mat.m0
                     + m[j+4]*mat.m1
                     + m[j+8]*mat.m2
                     + m[j+12]*mat.m3;

            ret.m[j+4] = m[j]*mat.m4
                       + m[j+4]*mat.m5
                       + m[j+8]*mat.m6
                       + m[j+12]*mat.m7;

            ret.m[j+8] = m[j]*mat.m8
                       + m[j+4]*mat.m9
                       + m[j+8]*mat.m10
                       + m[j+12]*mat.m11;

            ret.m[j+12] = m[j]*mat.m12
                        + m[j+4]*mat.m13
                        + m[j+8]*mat.m14
                        + m[j+12]*mat.m15;
        }
        return ret;
    }

    //!Apply the matrix dot product to this matrix
    inline GLMatrix& operator*= (const GLMatrix& mat)
    {
        GLMatrix temp(*this);
        for(register unsigned j = 0; j < 4; ++j)
        {
            m[j] = temp.m[j]*mat.m0
                 + temp.m[j+4]*mat.m1
                 + temp.m[j+8]*mat.m2
                 + temp.m[j+12]*mat.m3;

            m[j+4] = temp.m[j]*mat.m4
                   + temp.m[j+4]*mat.m5
                   + temp.m[j+8]*mat.m6
                   + temp.m[j+12]*mat.m7;

            m[j+8] = temp.m[j]*mat.m8
                   + temp.m[j+4]*mat.m9
                   + temp.m[j+8]*mat.m10
                   + temp.m[j+12]*mat.m11;

            m[j+12] = temp.m[j]*mat.m12
                    + temp.m[j+4]*mat.m13
                    + temp.m[j+8]*mat.m14
                    + temp.m[j+12]*mat.m15;
        }
        return *this;
    }

    //!Apply the matrix dot product to this matrix
    //!unrolling by sebastien bloc
    inline GLMatrix& mult3by3(const GLMatrix& mat)
    {
        GLMatrix temp(*this);
        m0 = temp.m0*mat.m0+temp.m4*mat.m1+temp.m8*mat.m2;
        m4 = temp.m0*mat.m4+temp.m4*mat.m5+temp.m8*mat.m6;
        m8 = temp.m0*mat.m8+temp.m4*mat.m9+temp.m8*mat.m10;

        m1 = temp.m1*mat.m0+temp.m5*mat.m1+temp.m9*mat.m2;
        m5 = temp.m1*mat.m4+temp.m5*mat.m5+temp.m9*mat.m6;
        m9 = temp.m1*mat.m8+temp.m5*mat.m9+temp.m9*mat.m10;

        m2 = temp.m2*mat.m0+temp.m6*mat.m1+temp.m10*mat.m2;
        m6 = temp.m2*mat.m4+temp.m6*mat.m5+temp.m10*mat.m6;
        m10 = temp.m2*mat.m8+temp.m6*mat.m9+temp.m10*mat.m10;

        m3 = temp.m3*mat.m0+temp.m7*mat.m1+temp.m11*mat.m2;
        m7 = temp.m3*mat.m4+temp.m7*mat.m5+temp.m11*mat.m6;
        m11 = temp.m3*mat.m8+temp.m7*mat.m9+temp.m11*mat.m10;
        return *this;
    }

    //!Get the matrix vector dot product, used to transform vertecies
    inline GLVector4<T> operator* (const GLVector4<T>& vec)
    {
        GLVector4<T> ret;
        for(register unsigned j = 0; j < 4; ++j)
        {
            ret.val[j] = vec.x*m[j]
                       + vec.y*m[j+4]
                       + vec.z*m[j+8]
                       + vec.w*m[j+12];
        }
        return ret;
    }

    //!Get the matrix vector dot4 product, used to transform vertecies
    inline GLVector4<T> operator* (const T* v_arr)
    {
        GLVector4<T> ret;
        for(register unsigned j = 0; j < 4; ++j)
        {
            ret.val[j] = v_arr[0]*m[j]
                       + v_arr[1]*m[j+4]
                       + v_arr[2]*m[j+8]
                       + v_arr[3]*m[j+12];
        }
        return ret;
    }


    //!Get the matrix vector dot3 product, used to transform non-4D vertecies
    inline GLVector3<T> dot3 (const T* v_arr)
    {
        GLVector3<T> ret;
        for(register unsigned j = 0; j < 3; ++j)
        {
            ret.val[j] = v_arr[0]*m[j]
                       + v_arr[1]*m[j+4]
                       + v_arr[2]*m[j+8]
                       + m[j+12];
            //scale translate and rotate disregarding w scaling
        }

        //do w scaling

        register T resip = 1/(v_arr[0]*m[3] + v_arr[1]*m[7] + v_arr[2]*m[11] + m[15]);

        ret[0] *= resip;
        ret[1] *= resip;
        ret[2] *= resip;

        return ret;
    }

    //!Get the matrix vector dot3 product, used to transform non-4D vertecies
    inline GLVector3<T> dot3 (T x, T y, T z)
    {
        GLVector3<T> ret;
        for(register unsigned j = 0; j < 3; ++j)
        {
            ret.val[j] = x*m[j] + y*m[j+4] +z*m[j+8] + m[j+12];
            //scale translate and rotate disregarding w scaling
        }
        //do w scaling

        register T resip = 1/(x*m[3] + y*m[7] + z*m[11] + m[15]);

        ret[0] *= resip;
        ret[1] *= resip;
        ret[2] *= resip;

        return ret;
    }

    //!Get the matrix vector dot4 product, used to transform 4D vertecies
    inline GLVector4<T> dot4 (const T* v_arr)
    {
        GLVector4<T> ret;
        for(register unsigned j = 0; j < 4; ++j)
        {
            ret.val[j] = 0;
            for(register unsigned i = 0; i < 4; ++i)
                ret.val[j] += v_arr[i]*m[j+i*4];
        }
        return ret;
    }

    //!Get the matrix vector dot4 product, used to transform 4D vertecies
    inline void vdot4 (T* v_arr) const
    {
        GLVector4<T> temp(v_arr);
        for(register unsigned j = 0; j < 4; ++j)
        {
            v_arr[j] = 0;
            for(register unsigned i = 0; i < 4; ++i)
                v_arr[j] += temp.val[i]*m[j+i*4];
        }
    }

    //!Get the matrix vector dot4 product, used to transform 4D vertecies
    inline GLVector4<T> dot4 (T x, T y, T z, T w)
    {
        GLVector4<T> ret;
        for(register unsigned j = 0; j < 4; ++j)
            ret.val[j] = x*m[j] + y*m[j+4] +z*m[j+8] + w*m[j+12];

        return ret;
    }

    //!Get the matrix vector dot product with w = 1, use for transforming non 4D vectors*/
    inline GLVector3<T> operator* (const GLVector3<T>& vec)
    {
        GLVector3<T> ret(T(0));
        for(register unsigned j = 0; j < 3; ++j)
            for(register unsigned i = 0; i < 3; ++i)
                ret.val[j] += vec.val[i]*m[j+i*4]; //scale and rotate disregarding w scaling

        for(register unsigned i = 0; i < 3; ++i)
            ret.val[i] += m[i+3*4]; //translate

        //do w scaling
        T w = m[15];
        for(register unsigned i = 0; i < 3; ++i)
            w += vec.val[i]*m[3+i*4];

        register T resip = 1/w;

        for(register unsigned i = 0; i < 3; ++i)
            ret[i] *= resip;

        return ret;
    }

    //!Transform the vertex and send it to OpenGL*/
    inline void glVertex3v(const T* v_arr)
    {
        (this->dot3(v_arr)).glVertex();
    }

    //!Transform the vertex and send it to OpenGL*/
    inline void glVertex3(T x, T y, T z)
    {
        (this->dot3(x,y,z)).glVertex();
    }

    //!Transform the vertex and send it to OpenGL*/
    inline void glVertex4v(const T* v_arr)
    {
        (this->operator *(v_arr)).glVertex();
    }

    //!Transform the vertex and send it to OpenGL*/
    inline void glVertex4(T x, T y, T z, T w)
    {
        (this->dot4(x,y,z,w)).glVertex();
    }

    //!Transform a run of vertecies and send them to OpenGL*/
    void glVertex3v(int num, const T* v_arr);

    //!Transform a run of vertecies and send them to OpenGL*/
    void glVertex4v(int num, const T* v_arr);

    //!GL interface, glMultMatrix
    void glMultMatrix(void) const;
    //!GL interface, glLoadMatrix
    void glLoadMatrix(void) const;
    //!GL interface, glGet, gets a matrix from OpenGL
    void glGet(GLenum pname);

    //!Transpose the matrix
    //!04-2005, bugfix submitted by Jordi Rovira, fixed double transpose error.
    inline GLMatrix& transpose(void)
    {
        register T temp;
        for(register int i = 0; i < 4; ++i)
            for(register int j = 0; j < i; ++j)
            {
                temp = m[j+i*4];
                m[j+i*4] = m[i+j*4];
                m[i+j*4] = temp;
            };

        return *this;
    }

    //!Return the transpose
    inline GLMatrix getTranspose(void)
    {
        GLMatrix temp;
        for(register int i = 0; i < 4; ++i)
            for(register int j = 0; j < 4; ++j)
                temp.m[j+i*4] = m[i+j*4];
        return temp;
    }

    //!Special glMatricies
    //!Identity matrix
    static GLMatrix identity(void)
    {
        GLMatrix ret;

        ret.m0 = 1;   ret.m4 = 0;   ret.m8  = 0;  ret.m12 = 0;
        ret.m1 = 0;   ret.m5 = 1;   ret.m9  = 0;  ret.m13 = 0;
        ret.m2 = 0;   ret.m6 = 0;   ret.m10 = 1;  ret.m14 = 0;
        ret.m3 = 0;   ret.m7 = 0;   ret.m11 = 0;  ret.m15 = 1;

        return ret;
    }

    //!Make this an identity matrix
    inline GLMatrix& loadIdentity(void)
    {
        m0 = 1;   m4 = 0;   m8  = 0;  m12 = 0;
        m1 = 0;   m5 = 1;   m9  = 0;  m13 = 0;
        m2 = 0;   m6 = 0;   m10 = 1;  m14 = 0;
        m3 = 0;   m7 = 0;   m11 = 0;  m15 = 1;

        return *this;
    }

    //!OpenGL rotation matrix
    static GLMatrix glRotate(T angle, T x, T y, T z);

    //!Make this an OpenGL rotation matrix
    inline GLMatrix& loadRotate(T angle, T x, T y, T z)
    {
        register T magSqr = x*x + y*y + z*z;
        if(magSqr != 1.0)
        {
            register T mag = sqrt(magSqr);
            x/=mag;
            y/=mag;
            z/=mag;
        }
        T c = cos(angle*M_PI/180);
        T s = sin(angle*M_PI/180);
        m0 = x*x*(1-c)+c;
        m1 = y*x*(1-c)+z*s;
        m2 = z*x*(1-c)-y*s;
        m3 = 0;

        m4 = x*y*(1-c)-z*s;
        m5 = y*y*(1-c)+c;
        m6 = z*y*(1-c)+x*s;
        m7 = 0;

        m8 = x*z*(1-c)+y*s;
        m9 = y*z*(1-c)-x*s;
        m10 = z*z*(1-c)+c;
        m11 = 0;

        m12 = 0;
        m13 = 0;
        m14 = 0;
        m15 = 1;

        return *this;
    }

    //!Load rotate[X,Y,Z,XYZ] specialisations by sebastien bloc
    //!Make this an OpenGL rotation matrix: same as loadRotate(angle,1,0,0)
    inline GLMatrix& loadRotateX(T angle)
    {
        T c = cos(angle*M_PI/180);
        T s = sin(angle*M_PI/180);
        m0 = 1;
        m1 = 0;
        m2 = 0;
        m3 = 0;

        m4 = 0;
        m5 = c;
        m6 = s;
        m7 = 0;

        m8 = 0;
        m9 = -s;
        m10 = c;
        m11 = 0;

        m12 = 0;
        m13 = 0;
        m14 = 0;
        m15 = 1;

        return *this;
    }

    //!Make this an OpenGL rotation matri0: same as loadRotate(angle,0,1,0)
    inline GLMatrix& loadRotateY(T angle)
    {
        T c = cos(angle*M_PI/180);
        T s = sin(angle*M_PI/180);
        m0 = c;
        m1 = 0;
        m2 = -s;
        m3 = 0;

        m4 = 0;
        m5 = 1;
        m6 = 0;
        m7 = 0;

        m8 = s;
        m9 = 0;
        m10 = c;
        m11 = 0;

        m12 = 0;
        m13 = 0;
        m14 = 0;
        m15 = 1;

        return *this;
    }

    //!Make this an OpenGL rotation matrix: same as loadRotateZ(angle,0,0,1)
    inline GLMatrix& loadRotateZ(T angle)
    {
        T c = cos(angle*M_PI/180);
        T s = sin(angle*M_PI/180);
        m0 = c;
        m1 = s;
        m2 = 0;
        m3 = 0;

        m4 = -s;
        m5 = c;
        m6 = 0;
        m7 = 0;

        m8 = 0;
        m9 = 0;
        m10 = 1;
        m11 = 0;

        m12 = 0;
        m13 = 0;
        m14 = 0;
        m15 = 1;

        return *this;
    }

    //!Apply an OpenGL rotation matrix to this
    inline GLMatrix& applyRotate(T angle, T x, T y, T z)
    {
        static GLMatrix temp;
        temp.loadRotate(angle,x,y,z);
        return mult3by3(temp);
    }

    //!Apply rotate[X,Y,Z,XYZ] specialisations by sebastien bloc
    //!Apply an OpenGL rotation matrix to this
    inline GLMatrix& applyRotateX(T angle)
    {
        static GLMatrix temp;
        temp.loadRotateX(angle);
        return mult3by3(temp);
    }

    //!Apply an OpenGL rotation matrix to this
    inline GLMatrix& applyRotateY(T angle)
    {
        static GLMatrix temp;
        temp.loadRotateY(angle);
        return mult3by3(temp);
    }

    //!Apply an OpenGL rotation matrix to this
    inline GLMatrix& applyRotateZ(T angle)
    {
        static GLMatrix temp;
        temp.loadRotateZ(angle);
        return mult3by3(temp);
    }

    //!Apply an OpenGL rotation matrix to this
    GLMatrix& applyRotateXYZ(T x,T y,T z)
    {
        static GLMatrix temp;
        temp.loadRotateX(x);
        mult3by3(temp);
        temp.loadRotateY(y);
        mult3by3(temp);
        temp.loadRotateZ(z);
        return mult3by3(temp);
    }



    //!OpenGL scale matrix
    static GLMatrix glScale(T x, T y, T z);

    //!Make this an OpenGL scale matrix
    inline GLMatrix& loadScale(T x, T y, T z = 1)
    {
        m0 = x;   m4 = 0;   m8  = 0;  m12 = 0;
        m1 = 0;   m5 = y;   m9  = 0;  m13 = 0;
        m2 = 0;   m6 = 0;   m10 = z;  m14 = 0;
        m3 = 0;   m7 = 0;   m11 = 0;  m15 = 1;

        return *this;
    }

    //!Apply an OpenGL scale matrix to this
    inline GLMatrix& applyScale(T x, T y)
    {
        /*improved version*/
        //assuming z = 1
        m0*=x;    m1*=x;    m2*=x;    m3*=x;
        m4*=y;    m5*=y;    m6*=y;    m7*=y;
        return *this;
    }

    //!Apply an OpenGL scale matrix to this
    inline GLMatrix& applyScale(T x, T y, T z)
    {
        /*improved version*/
        m0*=x;    m1*=x;    m2*=x;    m3*=x;
        m4*=y;    m5*=y;    m6*=y;    m7*=y;
        m8*=z;    m9*=z;    m10*=z;   m11*=z;
        return *this;
    }

    //!Apply an OpenGL scale matrix to this
    inline GLMatrix& applyScale(const GLVector2<T>& scale)
    {
        /*improved version*/
        //Assuming z = 1
        m0*=scale.x;    m1*=scale.x;    m2*=scale.x;    m3*=scale.x;
        m4*=scale.y;    m5*=scale.y;    m6*=scale.y;    m7*=scale.y;
        return *this;
    }

    //!Apply an OpenGL scale matrix to this
    inline GLMatrix& applyScale(const GLVector3<T>& scale)
    {
        /*improved version*/
        m0*=scale.x;    m1*=scale.x;    m2*=scale.x;    m3*=scale.x;
        m4*=scale.y;    m5*=scale.y;    m6*=scale.y;    m7*=scale.y;
        m8*=scale.z;    m9*=scale.z;    m10*=scale.z;   m11*=scale.z;
        return *this;
    }

    //!OpenGL translate matrix
    static GLMatrix glTranslate(T x, T y, T z);

    //!Make this an OpenGL translate matrix
    inline GLMatrix& loadTranslate(T x, T y, T z)
    {
        m0 = 1;   m4 = 0;   m8  = 0;  m12 = x;
        m1 = 0;   m5 = 1;   m9  = 0;  m13 = y;
        m2 = 0;   m6 = 0;   m10 = 1;  m14 = z;
        m3 = 0;   m7 = 0;   m11 = 0;  m15 = 1;

        return *this;
    }

    //!Apply an OpenGL translate matrix to this
    inline GLMatrix& applyTranslate(T x, T y)
    {
        /*improved version*/
        //assuming z = 0
        m12 += m0*x + m4*y;
        m13 += m1*x + m5*y;
        m14 += m2*x + m6*y;
        return *this;
    }

    //!Apply an OpenGL translate matrix to this
    inline GLMatrix& applyTranslate(T x, T y, T z)
    {
        /*improved version*/
        m12 += m0*x + m4*y + m8*z;
        m13 += m1*x + m5*y + m9*z;
        m14 += m2*x + m6*y + m10*z;
        return *this;
    }

    //!Apply an OpenGL translate matrix to this
    inline GLMatrix& applyTranslate(const GLVector2<T>& trans)
    {
        /*improved version*/
        //assuming z = 0
        m12 += m0*trans.x + m4*trans.y;
        m13 += m1*trans.x + m5*trans.y;
        m14 += m2*trans.x + m6*trans.y;
        return *this;
    }

    //!Apply an OpenGL translate matrix to this
    inline GLMatrix& applyTranslate(const GLVector3<T>& trans)
    {
        /*improved version*/
        m12 += m0*trans.x + m4*trans.y + m8*trans.z;
        m13 += m1*trans.x + m5*trans.y + m9*trans.z;
        m14 += m2*trans.x + m6*trans.y + m10*trans.z;
        return *this;
    }

    #define glFrustrum glFrustum
    //!OpenGL frustum matrix
    static GLMatrix glFrustum(GLdouble left, GLdouble right,
                       GLdouble bottom, GLdouble top,
                   GLdouble zNear, GLdouble zFar)
    {
        GLMatrix ret;
        ret.m0 = 2*zNear/(right-left);
        ret.m1 = 0;
        ret.m2 = 0;
        ret.m3 = 0;

        ret.m4 = 0;
        ret.m5 = 2*zNear/(top-bottom);
        ret.m6 = 0;
        ret.m7 = 0;

        ret.m8 = (right+left)/(right-left);
        ret.m9 = (top+bottom)/(top-bottom);
        ret.m10 = -(zFar+zNear)/(zFar-zNear);
        ret.m11 = -1;

        ret.m12 = 0;
        ret.m13 = 0;
        ret.m14 = -2*zFar*zNear/(zFar-zNear);
        ret.m15 = 0;

        return ret;
    }

    #define loadFrustrum loadFrustum
    //!Make this with an OpenGL frustum matrix
    inline GLMatrix& loadFrustum(GLdouble left, GLdouble right,
                     GLdouble bottom, GLdouble top,
                 GLdouble zNear, GLdouble zFar)
    {
        m0 = 2*zNear/(right-left);
        m1 = 0;
        m2 = 0;
        m3 = 0;

        m4 = 0;
        m5 = 2*zNear/(top-bottom);
        m6 = 0;
        m7 = 0;

        m8 = (right+left)/(right-left);
        m9 = (top+bottom)/(top-bottom);
        m10 = -(zFar+zNear)/(zFar-zNear);
        m11 = -1;

        m12 = 0;
        m13 = 0;
        m14 = -2*zFar*zNear/(zFar-zNear);
        m15 = 0;

        return *this;
    }

    //!OpenGL orthogonal matrix
    static GLMatrix glOrtho(GLdouble left, GLdouble right,
                    GLdouble bottom, GLdouble top,
                GLdouble zNear, GLdouble zFar)
    {
        GLMatrix ret;

        ret.m0 = 2/(right-left);
        ret.m1 = 0;
        ret.m2 = 0;
        ret.m3 = 0;

        ret.m4 = 0;
        ret.m5 = 2/(top-bottom);
        ret.m6 = 0;
        ret.m7 = 0;

        ret.m8 = 0;
        ret.m9 = 0;
        ret.m10 = -2/(zFar-zNear);
        ret.m11 = 0;

        ret.m12 = -(right+left)/(right-left);
        ret.m13 = -(top+bottom)/(top-bottom);
        ret.m14 = -(zFar+zNear)/(zFar-zNear);
        ret.m15 = 1;

        return ret;
    }

    //!OpenGL orthogonal matrix
    inline GLMatrix& loadOrtho(GLdouble left, GLdouble right,
                    GLdouble bottom, GLdouble top,
                GLdouble zNear, GLdouble zFar)
    {
        m0 = 2/(right-left);
        m1 = 0;
        m2 = 0;
        m3 = 0;

        m4 = 0;
        m5 = 2/(top-bottom);
        m6 = 0;
        m7 = 0;

        m8 = 0;
        m9 = 0;
        m10 = -2/(zFar-zNear);
        m11 = 0;

        m12 = -(right+left)/(right-left);
        m13 = -(top+bottom)/(top-bottom);
        m14 = -(zFar+zNear)/(zFar-zNear);
        m15 = 1;

        return *this;
    }

    //!OpenGL View Matrix.
    inline GLMatrix& loadView(const GLVector3<T>& front, const GLVector3<T>& up, const GLVector3<T> side)
    {
        m0 = side.x;
        m1 = up.x;
        m2 = -front.x;
        m3 = 0;

        m4 = side.y;
        m5 = up.y;
        m6 = -front.y;
        m7 = 0;

        m8 = side.z;
        m9 = up.z;
        m10 = -front.z;
        m11 = 0;

        m12 = 0;
        m13 = 0;
        m14 = 0;
        m15 = 1;

        return *this;
    }


    #ifdef GLMATRIX_IOSTREAM
    //!iostream interface for input from char stream, enable by defining GLMATRIX_IOSTREAM
    //!Row major order is expected to conform with standard output
    friend std::basic_istream<char, std::char_traits<char> >& operator >>
      ( std::basic_istream<char, std::char_traits<char> >& in, GLMatrix& mat)
    {
        for(int j = 0; j < 4; ++j)
            for(int i = 0; i < 4; ++i)
                in>>mat[i*4+j];
        return in;
    }

    //!iostream interface for output to char stream, enable by defining GLMATRIX_IOSTREAM
    //!Row major order is expected to conform with standard input
    friend std::basic_ostream<char, std::char_traits<char> >& operator <<
      ( std::basic_ostream<char, std::char_traits<char> >& out, GLMatrix& mat)
    {
        for(int j = 0; j < 4; ++j)
        {
            for(int i = 0; i < 4; ++i)
            {
                out<<mat[i*4+j]<<" ";
            }
            out<<std::endl;
        }
        return out;
    }
    #endif


protected:

    /**Cofactor maker after the looping determinant theory I'm sure we all learnt in high-school
      *  factor1 |   ^          |
      *  ------------|----------v----
      *          | major   |
      *          |         |  minor
      *              ^          |
      *  factor2 |   |          |
      *  ------------|----------v----
      *          | major   |
      *          |         |  minor
      *              ^          |
      *  factor3 |   |          |
      *  ------------|----------v----
      *          | major   |
      *          |         |  minor
      *              ^          |
      *              |          v
      */
    #define cofactor_maker(f1,mj1,mi1, f2,mj2,mi2, f3,mj3,mi3) \
    f1*(mj1*mi1-mj2*mi3) + f2*(mj2*mi2-mj3*mi1) + f3*(mj3*mi3-mj1*mi2)

    //inline T cofactorm0()const { return m5*(m10*m15-m11*m14)+m6*(m11*m13-m8*m15)+m7*(m8*m14*m10*m13);  }
    //!Cofactor of m0
    inline T cofactorm0() const { return cofactor_maker(m5,m10,m15, m6,m11,m13, m7,m9,m14); }
    //!Cofactor of m1
    inline T cofactorm1() const { return cofactor_maker(m6,m11,m12, m7,m8,m14, m4,m10,m15); }
    //!Cofactor of m2
    inline T cofactorm2() const { return cofactor_maker(m7,m8,m13, m4,m9,m15, m5,m11,m12); }
    //!Cofactor of m3
    inline T cofactorm3() const { return cofactor_maker(m4,m9,m14, m5,m10,m12, m6,m8,m13); }

    //!Cofactor of m4
    inline T cofactorm4() const { return cofactor_maker(m9,m14,m3, m10,m15,m1, m11,m13,m2); }
    //!Cofactor of m5
    inline T cofactorm5() const { return cofactor_maker(m10,m15,m0, m11,m12,m2, m8,m14,m3); }
    //!Cofactor of m6
    inline T cofactorm6() const { return cofactor_maker(m11,m12,m1, m8,m13,m3, m9,m15,m0); }
    //!Cofactor of m7
    inline T cofactorm7() const { return cofactor_maker(m8,m13,m2, m9,m14,m0, m10,m12,m1); }

    //!Cofactor of m8
    inline T cofactorm8() const { return cofactor_maker(m13,m2,m7, m14,m3,m5, m15,m1,m6); }
    //!Cofactor of m9
    inline T cofactorm9() const { return cofactor_maker(m14,m13,m4, m15,m0,m6, m12,m2,m7); }
    //!Cofactor of m10
    inline T cofactorm10() const { return cofactor_maker(m15,m0,m5, m12,m1,m7, m13,m3,m4); }
    //!Cofactor of m11
    inline T cofactorm11() const { return cofactor_maker(m12,m1,m6, m13,m2,m4, m14,m0,m5); }

    //!Cofactor of m12
    inline T cofactorm12() const { return cofactor_maker(m1,m6,m11, m2,m7,m9, m3,m5,m10); }
    //!Cofactor of m13
    inline T cofactorm13() const { return cofactor_maker(m2,m7,m8, m3,m4,m10, m10,m6,m11); }
    //!Cofactor of m14
    inline T cofactorm14() const { return cofactor_maker(m3,m4,m9, m0,m5,m11, m1,m7,m8); }
    //!Cofactor of m15
    inline T cofactorm15() const { return cofactor_maker(m0,m5,m10, m1,m6,m8, m2,m4,m9); }

private:
    //!value array
    //!Anonymous union and struct by sebastien bloc (sebastien.bloc@free.fr)
    union
    {
        T m[16];
        struct
        {
            T m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15;
        };
    };
};

typedef GLMatrix<GLfloat>  GLMatrix4f;



#endif
