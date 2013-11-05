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

#include "GLMatrix.h"
#ifdef GLMATRIX_USE_SSE
typedef float sse_v4sf __attribute__ ((mode(V4SF)));
typedef union { sse_v4sf v4sf; float f[4]; } sse_vector;
#endif



template <>
void GLMatrix<GLdouble>::glMultMatrix(void) const
{ ::glMultMatrixd(m); }

template <>
void GLMatrix<GLfloat>::glMultMatrix(void) const
{ ::glMultMatrixf(m); }

template <>
void GLMatrix<GLdouble>::glLoadMatrix(void) const
{ ::glLoadMatrixd(m); }

template <>
void GLMatrix<GLfloat>::glLoadMatrix(void) const
{ ::glLoadMatrixf(m); }

template <>
void GLMatrix<GLdouble>::glGet(GLenum pname)
{ ::glGetDoublev(pname,m); }

template <>
void GLMatrix<GLfloat>::glGet(GLenum pname)
{ ::glGetFloatv(pname,m); }

template<>
GLMatrix<GLdouble> GLMatrix<GLdouble>::glScale(GLdouble _x, GLdouble _y, GLdouble _z)
{
    GLMatrix<GLdouble> ret;
    ret.m0 = _x;    ret.m4 = 0;     ret.m8 = 0;     ret.m12 = 0;
    ret.m1 = 0;     ret.m5 = _y;    ret.m9 = 0;     ret.m13 = 0;
    ret.m2 = 0;     ret.m6 = 0;     ret.m10 = _z;   ret.m14 = 0;
    ret.m3 = 0;     ret.m7 = 0;     ret.m11 = 0;    ret.m15 = 1;
    return ret;
}

template<>
GLMatrix<GLfloat> GLMatrix<GLfloat>::glScale(GLfloat _x, GLfloat _y, GLfloat _z)
{
    GLMatrix<GLfloat> ret;
    ret.m0 = _x;    ret.m4 = 0;     ret.m8 = 0;     ret.m12 = 0;
    ret.m1 = 0;     ret.m5 = _y;    ret.m9 = 0;     ret.m13 = 0;
    ret.m2 = 0;     ret.m6 = 0;     ret.m10 = _z;   ret.m14 = 0;
    ret.m3 = 0;     ret.m7 = 0;     ret.m11 = 0;    ret.m15 = 1;
    return ret;
}

template<>
GLMatrix<GLdouble> GLMatrix<GLdouble>::glTranslate(GLdouble _x, GLdouble _y, GLdouble _z)
{
    GLMatrix<GLdouble> ret;
    ret.m0 = 1; ret.m4 = 0; ret.m8 = 0;  ret.m12 = _x;
    ret.m1 = 0; ret.m5 = 1; ret.m9 = 0;  ret.m13 = _y;
    ret.m2 = 0; ret.m6 = 0; ret.m10 = 1; ret.m14 = _z;
    ret.m3 = 0; ret.m7 = 0; ret.m11 = 0; ret.m15 = 1;
    return ret;
}

template<>
GLMatrix<GLfloat> GLMatrix<GLfloat>::glTranslate(GLfloat _x, GLfloat _y, GLfloat _z)
{
    GLMatrix<GLfloat> ret;
    ret.m0 = 1; ret.m4 = 0; ret.m8 = 0;  ret.m12 = _x;
    ret.m1 = 0; ret.m5 = 1; ret.m9 = 0;  ret.m13 = _y;
    ret.m2 = 0; ret.m6 = 0; ret.m10 = 1; ret.m14 = _z;
    ret.m3 = 0; ret.m7 = 0; ret.m11 = 0; ret.m15 = 1;
    return ret;
}

template<>
GLMatrix<GLdouble> GLMatrix<GLdouble>::glRotate(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    GLMatrix<GLdouble> ret;
    GLdouble mag = sqrt(x*x + y*y + z*z);
    x/=mag;
    y/=mag;
    z/=mag;
    GLdouble c = cos(angle*M_PI/180);
    GLdouble s = sin(angle*M_PI/180);
    ret.m0 = x*x*(1-c)+c;
    ret.m1 = y*x*(1-c)+z*s;
    ret.m2 = z*x*(1-c)-y*s;
    ret.m3 = 0;

    ret.m4 = x*y*(1-c)-z*s;
    ret.m5 = y*y*(1-c)+c;
    ret.m6 = z*y*(1-c)+x*s;
    ret.m7 = 0;

    ret.m8 = x*z*(1-c)+y*s;
    ret.m9 = y*z*(1-c)-x*s;
    ret.m10 = z*z*(1-c)+c;
    ret.m11 = 0;

    ret.m12 = 0;
    ret.m13 = 0;
    ret.m14 = 0;
    ret.m15 = 1;

    return ret;
}

template<>
GLMatrix<GLfloat> GLMatrix<GLfloat>::glRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLMatrix<GLfloat> ret;
    GLfloat mag = sqrt(x*x + y*y + z*z);
    x/=mag;
    y/=mag;
    z/=mag;
    GLfloat c = cos(angle*M_PI/180);
    GLfloat s = sin(angle*M_PI/180);
    ret.m0 = x*x*(1-c)+c;
    ret.m1 = y*x*(1-c)+z*s;
    ret.m2 = z*x*(1-c)-y*s;
    ret.m3 = 0;

    ret.m4 = x*y*(1-c)-z*s;
    ret.m5 = y*y*(1-c)+c;
    ret.m6 = z*y*(1-c)+x*s;
    ret.m7 = 0;

    ret.m8 = x*z*(1-c)+y*s;
    ret.m9 = y*z*(1-c)-x*s;
    ret.m10 = z*z*(1-c)+c;
    ret.m11 = 0;

    ret.m12 = 0;
    ret.m13 = 0;
    ret.m14 = 0;
    ret.m15 = 1;

    return ret;
}


template<>
void GLMatrix<GLfloat>::glVertex3v(int num, const GLfloat* v_arr)
{
    #ifdef GLMATRIX_USE_SSE
    __builtin_prefetch(v_arr);

    sse_vector r0,r1,r2;
    register sse_v4sf m_col0,m_col1,m_col2,m_col3;

    m_col0 = __builtin_ia32_loadaps(m);
    m_col1 = __builtin_ia32_loadaps(m+4);
    m_col2 = __builtin_ia32_loadaps(m+8);
    m_col3 = __builtin_ia32_loadaps(m+12);

    for(register int k = 0; k < num; ++k)
    {
        //load x,y,z
        r0.v4sf = __builtin_ia32_loadss(v_arr);
        r1.v4sf = __builtin_ia32_loadss(v_arr+1);
        r2.v4sf = __builtin_ia32_loadss(v_arr+2);
        //extend into all 4 single floats
        r0.v4sf = __builtin_ia32_shufps(r0.v4sf,r0.v4sf,0x00);
        r1.v4sf = __builtin_ia32_shufps(r1.v4sf,r1.v4sf,0x00);
        r2.v4sf = __builtin_ia32_shufps(r2.v4sf,r2.v4sf,0x00);

        //do the mults
        r0.v4sf = __builtin_ia32_mulps(r0.v4sf,m_col0);
        v_arr+=3;
        r1.v4sf = __builtin_ia32_mulps(r1.v4sf,m_col1);
        //add it all up and, voila
        r2.v4sf = __builtin_ia32_mulps(r2.v4sf,m_col2);
        r0.v4sf = __builtin_ia32_addps(r0.v4sf,r1.v4sf);
        r2.v4sf = __builtin_ia32_addps(r2.v4sf,m_col3);
        r0.v4sf = __builtin_ia32_addps(r0.v4sf,r2.v4sf);

        ::glVertex4fv(r0.f);
    }

    #else
    register GLfloat ret[3];
    register GLfloat recip;
    for(register int k = 0; k < num; ++k)
    {
        ret[0] = v_arr[k*3]*m0 + v_arr[1+k*3]*m4 + v_arr[2+k*3]*m8 + m12;
        ret[1] = v_arr[k*3]*m1 + v_arr[1+k*3]*m5 + v_arr[2+k*3]*m9 + m13;
        ret[2] = v_arr[k*3]*m2 + v_arr[1+k*3]*m6 + v_arr[2+k*3]*m10 + m14;

        recip = 1/(v_arr[k*3]*m3 + v_arr[1+k*3]*m7 + v_arr[2+k*3]*m11 + m15);

        ret[0] *= recip;
        ret[1] *= recip;
        ret[2] *= recip;
        ::glVertex3fv(ret);
    }
    #endif
}

template<>
void GLMatrix<GLdouble>::glVertex3v(int num, const GLdouble* v_arr)
{
    register GLdouble ret[3];
    register GLdouble recip;

    for(register int k = 0; k < num; ++k)
    {
        ret[0] = v_arr[k*3]*m0 + v_arr[1+k*3]*m4 + v_arr[2+k*3]*m8 + m12;
        ret[1] = v_arr[k*3]*m1 + v_arr[1+k*3]*m5 + v_arr[2+k*3]*m9 + m13;
        ret[2] = v_arr[k*3]*m2 + v_arr[1+k*3]*m6 + v_arr[2+k*3]*m10 + m14;

        recip = 1/(v_arr[k*3]*m[3] + v_arr[1+k*3]*m[7] + v_arr[2+k*3]*m[11] + m[15]);

        ret[0] *= recip;
        ret[1] *= recip;
        ret[2] *= recip;
        ::glVertex3dv(ret);
    };
}

template<>
void GLMatrix<GLfloat>::glVertex4v(int num, const GLfloat* v_arr)
{
    #ifdef GLMATRIX_USE_SSE
    __builtin_prefetch(m);

    sse_vector r0,r1,r2,r3;
    register sse_v4sf m_col0,m_col1,m_col2,m_col3;

    __builtin_prefetch(v_arr);

    m_col0 = __builtin_ia32_loadaps(m);
    m_col1 = __builtin_ia32_loadaps(m+4);
    m_col2 = __builtin_ia32_loadaps(m+8);
    m_col3 = __builtin_ia32_loadaps(m+12);

    for(register int k = 0; k < num; ++k)
    {
        //load x,y,z,w into v
        r0.v4sf = __builtin_ia32_loadss(v_arr);
        r1.v4sf = __builtin_ia32_loadss(v_arr+1);
        r2.v4sf = __builtin_ia32_loadss(v_arr+2);
        r3.v4sf = __builtin_ia32_loadss(v_arr+3);
        r0.v4sf = __builtin_ia32_shufps(r0.v4sf,r0.v4sf,0x00);
        r1.v4sf = __builtin_ia32_shufps(r1.v4sf,r1.v4sf,0x00);
        r2.v4sf = __builtin_ia32_shufps(r2.v4sf,r2.v4sf,0x00);
        r3.v4sf = __builtin_ia32_shufps(r3.v4sf,r3.v4sf,0x00);

        r0.v4sf = __builtin_ia32_mulps(m_col0,r0.v4sf);
        v_arr+=4;
        r1.v4sf = __builtin_ia32_mulps(m_col1,r1.v4sf);
        r2.v4sf = __builtin_ia32_mulps(m_col2,r2.v4sf);
        r3.v4sf = __builtin_ia32_mulps(m_col3,r3.v4sf);
        r0.v4sf = __builtin_ia32_addps(r0.v4sf,r1.v4sf);
        r2.v4sf = __builtin_ia32_addps(r2.v4sf,r3.v4sf);
        r0.v4sf = __builtin_ia32_addps(r0.v4sf,r2.v4sf);

        ::glVertex4fv(r0.f);
    }

    #else
    register GLfloat ret[4];
    for(register int k = 0; k < num; ++k)
    {
        for(register unsigned j = 0; j < 4; ++j)
        {
            ret[j] = 0;
            for(register unsigned i = 0; i < 4; ++i)
                ret[j] += v_arr[i+k*4]*m[j+i*4];
        }
        ::glVertex4fv(ret);
    }
    #endif
}

template<>
void GLMatrix<GLdouble>::glVertex4v(int num, const GLdouble* v_arr)
{
    register GLdouble ret[4];
    for(register int k = 0; k < num; ++k)
    {
        for(register unsigned j = 0; j < 4; ++j)
        {
            ret[j] = 0;
            for(register unsigned i = 0; i < 4; ++i)
                ret[j] += v_arr[i+k*4]*m[j+i*4];
        };
        ::glVertex4dv(ret);
    };
}
