/*
 * amath.h
 *
 *  Created on: 12 lut 2016
 *      Author: crm
 */

#pragma once

#include <cmath>
#include <immintrin.h>

/**
 *  Matrix[ROW_ID][COLUMN_ID]
 */
/**
 * Układ współrzędnych:
 *   |y
 *   |
 *   |___x
 *  /
 * /z
 *
 */
/** Składanie macierzy:
 *  [r.x r.y r.z t.x] r - right
 *  [f.x f.y f.z t.y] f - forward
 *  [u.x u.y u.z t.z] u - up
 *  [s.x s.y s.z 1.0] t - translation; s - scale
 *
 *  https://stackoverflow.com/a/16507631/3531262
 */

#define LOG_STR_VECTOR(v) "AVector " #v ": [%7.4f %7.4f %7.4f %7.4f]", v[0], v[1], v[2], v[3]
#define LOG_STR_MATRIX(m) "AMatrix " #m ":\n    [%5.2f %5.2f %5.2f %5.2f]\n    [%5.2f %5.2f %5.2f %5.2f]\n    [%5.2f %5.2f %5.2f %5.2f]\n    [%5.2f %5.2f %5.2f %5.2f]",\
		m[0][0], m[0][1], m[0][2], m[0][3],\
		m[1][0], m[1][1], m[1][2], m[1][3],\
		m[2][0], m[2][1], m[2][2], m[2][3],\
		m[3][0], m[3][1], m[3][2], m[3][3]

namespace Engine
	{
	namespace Math
		{
		#define ASMMATH_NO_SSE41

		// Stale
		#undef M_PI
		const double M_PI=3.14159265358979323846;

		// Funkcje
		void sincos(float val, float& s, float& c);

		// Struktury
		struct AVector
			{
			explicit AVector(float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f):
				x(x), y(y), z(z), w(w) {}
			AVector(const AVector& a, float w):
				x(a.x), y(a.y), z(a.z), w(w) {}
			AVector(const AVector& a, float z, float w):
				x(a.x), y(a.y), z(z), w(w) {}
			AVector(const AVector& a, float y, float z, float w):
				x(a.x), y(y), z(z), w(w) {}
			//AVector(float v): x(v), y(v), z(v), w(v) {}
			AVector(const AVector& a):
				x(a.x), y(a.y), z(a.z), w(a.w) {}

			AVector& operator=(const AVector& a) {x=a.x; y=a.y; z=a.z; w=a.w; return *this;}
			float& operator[](int i)
				{
				switch(i)
					{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					default: return w;
					}
				}
			const float& operator[](int i) const
				{
				switch(i)
					{
					case 0: return x;
					case 1: return y;
					case 2: return z;
					default: return w;
					}
				}

			float x, y, z, w;
			};

		struct AMatrix
			{
			// Row-major
			explicit AMatrix(const AVector& a=AVector(), const AVector& b=AVector(), const AVector& c=AVector(), const AVector& d=AVector())
				{row[0]=a; row[1]=b; row[2]=c; row[3]=d;}
			AMatrix(const AMatrix& m) {row[0]=m.row[0]; row[1]=m.row[1]; row[2]=m.row[2]; row[3]=m.row[3];}

			AMatrix& operator=(const AMatrix& m) {row[0]=m.row[0]; row[1]=m.row[1]; row[2]=m.row[2]; row[3]=m.row[3]; return *this;}
			AVector& operator[](int i)
				{
				switch(i)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						return row[i];
					default:
						return row[3];
					}
				}
			const AVector& operator[](int i) const
				{
				switch(i)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						return row[i];
					default:
						return row[3];
					}
				}

			AVector row[4];
			};

		// Operacje wektorowe
		AVector AVectorAdd(const AVector& a, const AVector& b);
		AVector AVectorSubtract(const AVector& a, const AVector& b);
		AVector AVectorMultiply(const AVector& a, const AVector& b); // Hadamard product, Schur product, entrywise product
		AVector AVectorDivide(const AVector& a, const AVector& b);   // jw
		AVector AVectorNormalize(const AVector& a);
		AVector AVectorLength(const AVector& a);
		AVector AVectorLengthSqr(const AVector& a);

		AVector AVectorDot(const AVector& a, const AVector& b);
		AVector AVectorCross(const AVector& a, const AVector& b);

		AVector AVectorFloor(const AVector& a);
		AVector AVectorAbsolute(const AVector& a);
		AVector AVectorMin(const AVector& a, const AVector& b);
		AVector AVectorMax(const AVector& a, const AVector& b);
		AVector AVectorClamp(const AVector& min, const AVector& a, const AVector& max);

		AVector AVectorZero();
		AVector AVectorOne();
		AVector AVectorReplicate(float val);

		AVector AVectorTransform(const AVector& a, const AMatrix& m);
		AVector AVectorMirror(const AVector& a, const AVector& mirror, const AVector& forward, const AVector& up=AVector(0, 0, 1));

		AVector AVectorLerp(const AVector& a, const AVector& b, float p);
		AVector AVectorSlerp(const AVector& a, const AVector& b, float p);

		// Operacje macierzowe
		AMatrix AMatrixTranspose(const AMatrix& m);
		AMatrix AMatrixAdd(const AMatrix& a, const AMatrix& b);
		AMatrix AMatrixSubtract(const AMatrix& a, const AMatrix& b);
		AMatrix AMatrixMultiply(const AMatrix& a, const AMatrix& b);

		AMatrix AMatrixAbsolute(const AMatrix& a);

		AMatrix AMatrixInverse(const AMatrix& m);
		AVector AMatrixDeterminant(const AMatrix& m);
		AMatrix AMatrixLU(const AMatrix& m);

		AMatrix AMatrixRotX(float ang);
		AMatrix AMatrixRotY(float ang);
		AMatrix AMatrixRotZ(float ang);
		AMatrix AMatrixRotate(float angle, const AVector& dir);
		AMatrix AMatrixTranslate(const AVector& v);
		AMatrix AMatrixSkew(const AVector& a);

		AMatrix AMatrixZero();
		AMatrix AMatrixIdentity();
		AMatrix AMatrixFill(float val);
		AMatrix AMatrixDiagonal(float val);

		// operatory
		inline AVector operator-(const AVector& a) {return AVector(-a.x, -a.y, -a.z, -a.w);}
		inline AVector operator+(const AVector& a, float v) {return AVectorAdd(a, AVectorReplicate(v));}
		inline AVector operator-(const AVector& a, float v) {return AVectorSubtract(a, AVectorReplicate(v));}
		inline AVector operator*(const AVector& a, float v) {return AVectorMultiply(a, AVectorReplicate(v));}
		inline AVector operator/(const AVector& a, float v) {return AVectorDivide(a, AVectorReplicate(v));}
		inline AVector operator+(float v, const AVector& a) {return AVectorAdd(a, AVectorReplicate(v));}
		inline AVector operator-(float v, const AVector& a) {return AVectorSubtract(a, AVectorReplicate(v));}
		inline AVector operator*(float v, const AVector& a) {return AVectorMultiply(a, AVectorReplicate(v));}
		inline AVector operator/(float v, const AVector& a) {return AVectorDivide(a, AVectorReplicate(v));}
		inline AVector operator+(const AVector& a, const AVector& b) {return AVectorAdd(a, b);}
		inline AVector operator-(const AVector& a, const AVector& b) {return AVectorSubtract(a, b);}
		inline AVector operator*(const AVector& a, const AVector& b) {return AVectorMultiply(a, b);}
		inline AVector operator/(const AVector& a, const AVector& b) {return AVectorDivide(a, b);}
		inline float operator|(const AVector& a, const AVector& b) {return AVectorDot(a, b).w;}
		inline AVector operator^(const AVector& a, const AVector& b) {return AVectorCross(a, b);}
		inline AMatrix operator-(const AMatrix& a) {return AMatrix(-a.row[0], -a.row[1], -a.row[2], -a.row[3]);}
		inline AMatrix operator+(const AMatrix& m, const AMatrix& n) {return AMatrixAdd(m, n);}
		inline AMatrix operator-(const AMatrix& m, const AMatrix& n) {return AMatrixSubtract(m, n);}
		inline AMatrix operator*(const AMatrix& m, const AMatrix& n) {return AMatrixMultiply(m, n);}
		inline AMatrix operator*(const AMatrix& m, float v) {return AMatrixMultiply(m, AMatrixFill(v));}
		inline AMatrix operator*(float v, const AMatrix& m) {return AMatrixMultiply(AMatrixFill(v), m);}
		inline AVector operator*(const AMatrix& m, const AVector& a) {return AVectorTransform(a, m);}

		inline AVector operator+=(AVector& a, const AVector& b) {return a=a+b;}
		inline AVector operator-=(AVector& a, const AVector& b) {return a=a-b;}
		inline AVector operator*=(AVector& a, const AVector& b) {return a=a*b;}
		inline AVector operator*=(AVector& a, float v) {return a=a*v;}
		inline AVector operator/=(AVector& a, const AVector& b) {return a=a/b;}
		inline AVector operator/=(AVector& a, float v) {return a=a/v;}

		// inline
		inline void sincos(float val, float& s, float& c)
			{
			asm volatile(
				"fsincos\n\t"
				: "=t" (c), "=u" (s)
				: "0" (val)
				);
			}


		inline AVector AVectorAdd(const AVector& a, const AVector& b)
			{
			return AVector(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
			}

		inline AVector AVectorSubtract(const AVector& a, const AVector& b)
			{
			return AVector(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
			}

		inline AVector AVectorMultiply(const AVector& a, const AVector& b)
			{
			return AVector(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
			}

		inline AVector AVectorDivide(const AVector& a, const AVector& b)
			{
			return AVector(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
			}

		inline AVector AVectorNormalize(const AVector& a)
			{
			const double len=sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w);

			return AVector(a.x/len, a.y/len, a.z/len, a.w/len);
			}

		inline AVector AVectorLength(const AVector& a)
			{
			const double len=sqrt(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w);

			return AVector(len, len, len, len);
			}

		inline AVector AVectorLengthSqr(const AVector& a)
			{
			const double len=a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w;

			return AVector(len, len, len, len);
			}


		inline AVector AVectorDot(const AVector& a, const AVector& b)
			{
			const double val=a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;

			return AVector(val, val, val, val);
			}

		inline AVector AVectorCross(const AVector& a, const AVector& b)
			{
			return AVector(a.y*b.z-b.y*a.z, b.x*a.z-a.x*b.z, a.x*b.y-b.x*a.y, 0.0f);
			}


		inline AVector AVectorFloor(const AVector& a)
			{
			return AVector((int)a.x, (int)a.y, (int)a.z, (int)a.w);
			}

		inline AVector AVectorAbsolute(const AVector& a)
			{
			return AVector(fabs(a.x), fabs(a.y), fabs(a.z), fabs(a.w));
			}

		inline AVector AVectorMin(const AVector& a, const AVector& b)
			{
			return AVector( (a.x<b.x)?a.x:b.x,
							(a.y<b.y)?a.y:b.y,
							(a.z<b.z)?a.z:b.z,
							(a.w<b.w)?a.w:b.w);
			}

		inline AVector AVectorMax(const AVector& a, const AVector& b)
			{
			return AVector( (a.x>b.x)?a.x:b.x,
							(a.y>b.y)?a.y:b.y,
							(a.z>b.z)?a.z:b.z,
							(a.w>b.w)?a.w:b.w);
			}

		inline AVector AVectorClamp(const AVector& min, const AVector& a, const AVector& max)
			{
			return AVectorMin(max, AVectorMax(min, a));
			}


		inline AVector AVectorZero()
			{
			return AVector(0.0f, 0.0f, 0.0f, 0.0f);
			}

		inline AVector AVectorOne()
			{
			return AVector(1.0f, 1.0f, 1.0f, 1.0f);
			}

		inline AVector AVectorReplicate(float v)
			{
			return AVector(v, v, v, v);
			}


		inline AVector AVectorTransform(const AVector& a, const AMatrix& m)
			{
			AVector c;

			//c.x=AVectorDot(a, m.row[0]).w;
			//c.y=AVectorDot(a, m.row[1]).w;
			//c.z=AVectorDot(a, m.row[2]).w;
			//c.w=AVectorDot(a, m.row[3]).w;
			c.x=a.x*m.row[0].x+a.y*m.row[0].y+a.z*m.row[0].z+a.w*m.row[0].w;
			c.y=a.x*m.row[1].x+a.y*m.row[1].y+a.z*m.row[1].z+a.w*m.row[1].w;
			c.z=a.x*m.row[2].x+a.y*m.row[2].y+a.z*m.row[2].z+a.w*m.row[2].w;
			c.w=a.x*m.row[3].x+a.y*m.row[3].y+a.z*m.row[3].z+a.w*m.row[3].w;

			return c;
			}

		inline AVector AVectorMirror(const AVector& a, const AVector& mirror, const AVector& normal)
			{
			//AVector n=forward^up;
			AVector mp=normal*((mirror-a)|normal);

			return a+2.0f*mp;
			}

		inline AVector AVectorLerp(const AVector& a, const AVector& b, float p)
			{
			return a+(b-a)*p;
			}

		inline AVector AVectorSlerp(const AVector& a, const AVector& b, float p)
			{
			const float DOTP=a|b;
			const float THETA=acos(DOTP)*p;

			if(THETA<0.0001f && THETA>-0.0001f)
				return b;

			return (a*cos(THETA)) + AVectorNormalize(b-a*DOTP)*sin(THETA);
			}


		inline AMatrix AMatrixTranspose(const AMatrix& m)
			{
			return AMatrix(
				AVector(m.row[0].x, m.row[1].x, m.row[2].x, m.row[3].x),
				AVector(m.row[0].y, m.row[1].y, m.row[2].y, m.row[3].y),
				AVector(m.row[0].z, m.row[1].z, m.row[2].z, m.row[3].z),
				AVector(m.row[0].w, m.row[1].w, m.row[2].w, m.row[3].w)
				);
			}

		inline AMatrix AMatrixAdd(const AMatrix& a, const AMatrix& b)
			{
			return AMatrix(
				AVectorAdd(a.row[0], b.row[0]),
				AVectorAdd(a.row[1], b.row[1]),
				AVectorAdd(a.row[2], b.row[2]),
				AVectorAdd(a.row[3], b.row[3]));
			}

		inline AMatrix AMatrixSubtract(const AMatrix& a, const AMatrix& b)
			{
			return AMatrix(
				AVectorSubtract(a.row[0], b.row[0]),
				AVectorSubtract(a.row[1], b.row[1]),
				AVectorSubtract(a.row[2], b.row[2]),
				AVectorSubtract(a.row[3], b.row[3]));
			}

		inline AMatrix AMatrixMultiply(const AMatrix& a, const AMatrix& b)
			{
			/*AMatrix c;

			const AMatrix at=AMatrixTranspose(a);

			for(int i=0; i<4; ++i)
				{
				c.row[i].x=AVectorDot(at.row[0], b.row[i]).w;
				c.row[i].y=AVectorDot(at.row[1], b.row[i]).w;
				c.row[i].z=AVectorDot(at.row[2], b.row[i]).w;
				c.row[i].w=AVectorDot(at.row[3], b.row[i]).w;
				}

			return c;*/

			return AMatrix(
				AVector(
					a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0] + a[0][3]*b[3][0],
					a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1] + a[0][3]*b[3][1],
					a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2] + a[0][3]*b[3][2],
					a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3]*b[3][3]),
				AVector(
					a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0] + a[1][3]*b[3][0],
					a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1] + a[1][3]*b[3][1],
					a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2] + a[1][3]*b[3][2],
					a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3]*b[3][3]),
				AVector(
					a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0] + a[2][3]*b[3][0],
					a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1] + a[2][3]*b[3][1],
					a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2] + a[2][3]*b[3][2],
					a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3]*b[3][3]),
				AVector(
					a[3][0]*b[0][0] + a[3][1]*b[1][0] + a[3][2]*b[2][0] + a[3][3]*b[3][0],
					a[3][0]*b[0][1] + a[3][1]*b[1][1] + a[3][2]*b[2][1] + a[3][3]*b[3][1],
					a[3][0]*b[0][2] + a[3][1]*b[1][2] + a[3][2]*b[2][2] + a[3][3]*b[3][2],
					a[3][0]*b[0][3] + a[3][1]*b[1][3] + a[3][2]*b[2][3] + a[3][3]*b[3][3]));
			}

		inline AMatrix AMatrixAbsolute(const AMatrix& a)
			{
			return AMatrix(
				AVectorAbsolute(a.row[0]),
				AVectorAbsolute(a.row[1]),
				AVectorAbsolute(a.row[2]),
				AVectorAbsolute(a.row[3]));
			}

		inline AMatrix AMatrixInverse(const AMatrix& mm)
			{
			AMatrix mi;

			float *m=(float*)&mm;
			float *inv=(float*)&mi;
			float det;

			inv[ 0]= m[ 5]*m[10]*m[15] - m[ 5]*m[11]*m[14] - m[ 9]*m[ 6]*m[15] + m[9]*m[ 7]*m[14] + m[13]*m[ 6]*m[11] - m[13]*m[ 7]*m[10];
			inv[ 4]=-m[ 4]*m[10]*m[15] + m[ 4]*m[11]*m[14] + m[ 8]*m[ 6]*m[15] - m[8]*m[ 7]*m[14] - m[12]*m[ 6]*m[11] + m[12]*m[ 7]*m[10];
			inv[ 8]= m[ 4]*m[ 9]*m[15] - m[ 4]*m[11]*m[13] - m[ 8]*m[ 5]*m[15] + m[8]*m[ 7]*m[13] + m[12]*m[ 5]*m[11] - m[12]*m[ 7]*m[ 9];
			inv[12]=-m[ 4]*m[ 9]*m[14] + m[ 4]*m[10]*m[13] + m[ 8]*m[ 5]*m[14] - m[8]*m[ 6]*m[13] - m[12]*m[ 5]*m[10] + m[12]*m[ 6]*m[ 9];
			inv[ 1]=-m[ 1]*m[10]*m[15] + m[ 1]*m[11]*m[14] + m[ 9]*m[ 2]*m[15] - m[9]*m[ 3]*m[14] - m[13]*m[ 2]*m[11] + m[13]*m[ 3]*m[10];
			inv[ 5]= m[ 0]*m[10]*m[15] - m[ 0]*m[11]*m[14] - m[ 8]*m[ 2]*m[15] + m[8]*m[ 3]*m[14] + m[12]*m[ 2]*m[11] - m[12]*m[ 3]*m[10];
			inv[ 9]=-m[ 0]*m[ 9]*m[15] + m[ 0]*m[11]*m[13] + m[ 8]*m[ 1]*m[15] - m[8]*m[ 3]*m[13] - m[12]*m[ 1]*m[11] + m[12]*m[ 3]*m[ 9];
			inv[13]= m[ 0]*m[ 9]*m[14] - m[ 0]*m[10]*m[13] - m[ 8]*m[ 1]*m[14] + m[8]*m[ 2]*m[13] + m[12]*m[ 1]*m[10] - m[12]*m[ 2]*m[ 9];
			inv[ 2]= m[ 1]*m[ 6]*m[15] - m[ 1]*m[ 7]*m[14] - m[ 5]*m[ 2]*m[15] + m[5]*m[ 3]*m[14] + m[13]*m[ 2]*m[ 7] - m[13]*m[ 3]*m[ 6];
			inv[ 6]=-m[ 0]*m[ 6]*m[15] + m[ 0]*m[ 7]*m[14] + m[ 4]*m[ 2]*m[15] - m[4]*m[ 3]*m[14] - m[12]*m[ 2]*m[ 7] + m[12]*m[ 3]*m[ 6];
			inv[10]= m[ 0]*m[ 5]*m[15] - m[ 0]*m[ 7]*m[13] - m[ 4]*m[ 1]*m[15] + m[4]*m[ 3]*m[13] + m[12]*m[ 1]*m[ 7] - m[12]*m[ 3]*m[ 5];
			inv[14]=-m[ 0]*m[ 5]*m[14] + m[ 0]*m[ 6]*m[13] + m[ 4]*m[ 1]*m[14] - m[4]*m[ 2]*m[13] - m[12]*m[ 1]*m[ 6] + m[12]*m[ 2]*m[ 5];
			inv[ 3]=-m[ 1]*m[ 6]*m[11] + m[ 1]*m[ 7]*m[10] + m[ 5]*m[ 2]*m[11] - m[5]*m[ 3]*m[10] - m[ 9]*m[ 2]*m[ 7] + m[ 9]*m[ 3]*m[ 6];
			inv[ 7]= m[ 0]*m[ 6]*m[11] - m[ 0]*m[ 7]*m[10] - m[ 4]*m[ 2]*m[11] + m[4]*m[ 3]*m[10] + m[ 8]*m[ 2]*m[ 7] - m[ 8]*m[ 3]*m[ 6];
			inv[11]=-m[ 0]*m[ 5]*m[11] + m[ 0]*m[ 7]*m[ 9] + m[ 4]*m[ 1]*m[11] - m[4]*m[ 3]*m[ 9] - m[ 8]*m[ 1]*m[ 7] + m[ 8]*m[ 3]*m[ 5];
			inv[15]= m[ 0]*m[ 5]*m[10] - m[ 0]*m[ 6]*m[ 9] - m[ 4]*m[ 1]*m[10] + m[4]*m[ 2]*m[ 9] + m[ 8]*m[ 1]*m[ 6] - m[ 8]*m[ 2]*m[ 5];

			det=m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];

			if(det==0.0f)
				return AMatrixIdentity();

			det=1.0f/det;

			for(int i=0; i<16; i++)
				inv[i]*=det;

			return mi;
			}

		inline AVector AMatrixDeterminant(const AMatrix& mm)
			{
			float *m=(float*)&mm;

			const float da= m[ 5]*m[10]*m[15]-m[ 5]*m[11]*m[14]-m[ 9]*m[ 6]*m[15]+m[9]*m[ 7]*m[14]+m[13]*m[ 6]*m[11]-m[13]*m[ 7]*m[10];
			const float db=-m[ 4]*m[10]*m[15]+m[ 4]*m[11]*m[14]+m[ 8]*m[ 6]*m[15]-m[8]*m[ 7]*m[14]-m[12]*m[ 6]*m[11]+m[12]*m[ 7]*m[10];
			const float dc= m[ 4]*m[ 9]*m[15]-m[ 4]*m[11]*m[13]-m[ 8]*m[ 5]*m[15]+m[8]*m[ 7]*m[13]+m[12]*m[ 5]*m[11]-m[12]*m[ 7]*m[ 9];
			const float dd=-m[ 4]*m[ 9]*m[14]+m[ 4]*m[10]*m[13]+m[ 8]*m[ 5]*m[14]-m[8]*m[ 6]*m[13]-m[12]*m[ 5]*m[10]+m[12]*m[ 6]*m[ 9];

			return AVectorReplicate(m[0]*da+m[1]*db+m[2]*dc+m[3]*dd);
			}

		inline AMatrix AMatrixLU(const AMatrix& mm)
			{
			AMatrix mlu;

			float *m=(float*)&mm;
			float *lu=(float*)&mlu;

			/*lu[ 0]=m[ 0];
			lu[ 1]=m[ 1];
			lu[ 2]=m[ 2];
			lu[ 3]=m[ 3];*/
			lu[ 4]= m[ 4]/m[ 0];
			lu[ 8]= m[ 8]/m[ 0];
			lu[12]= m[12]/m[ 0];
			lu[ 5]= m[ 5]-(m[ 4]*m[ 1]);
			lu[ 6]= m[ 6]-(m[ 4]*m[ 2]);
			lu[ 7]= m[ 7]-(m[ 4]*m[ 3]);
			lu[ 9]=(m[ 9]- m[ 8]*m[ 1])/m[ 5];
			lu[10]= m[10]-(m[ 8]*m[ 2]+m[ 9]*m[ 6]);
			lu[11]= m[11]-(m[ 8]*m[ 3]+m[ 9]*m[ 7]);
			lu[13]=(m[13]- m[12]*m[ 1])/m[ 5];
			lu[14]=(m[14]- m[12]*m[ 2]-m[13]*m[ 6])/m[10];
			lu[15]= m[15]-(m[12]*m[ 3]+m[13]*m[ 7]+m[14]*m[15]);

			return mlu;
			}

		inline AMatrix AMatrixRotX(float ang)
			{
			float s, c;
			sincos(ang*M_PI/180, s, c);

			return AMatrix(
				AVector( 1,  0,  0,  0),
				AVector( 0,  c, -s,  0),
				AVector( 0,  s,  c,  0),
				AVector( 0,  0,  0,  1));
			}

		inline AMatrix AMatrixRotY(float ang)
			{
			float s, c;
			sincos(ang*M_PI/180, s, c);

			return AMatrix(
				AVector( c,  0,  s,  0),
				AVector( 0,  1,  0,  0),
				AVector(-s,  0,  c,  0),
				AVector( 0,  0,  0,  1));
			}

		inline AMatrix AMatrixRotZ(float ang)
			{
			float s, c;
			sincos(ang*M_PI/180, s, c);

			return AMatrix(
				AVector( c, -s,  0,  0),
				AVector( s,  c,  0,  0),
				AVector( 0,  0,  1,  0),
				AVector( 0,  0,  0,  1));
			}

		inline AMatrix AMatrixRotate(float angle, const AVector& dir)
			{
			AMatrix m;

			float s, c;
			sincos(angle*M_PI/180.0f, s, c);

			AVector u=AVectorNormalize(dir);
			float uxy=u.x*u.y*(1-c);
			float uyz=u.y*u.z*(1-c);
			float uxz=u.x*u.z*(1-c);
			float ux2=u.x*u.x*(1-c);
			float uy2=u.y*u.y*(1-c);
			float uz2=u.z*u.z*(1-c);
			float uxsin=u.x*s;
			float uysin=u.y*s;
			float uzsin=u.z*s;

			m.row[0]=AVector(c+ux2, uxy+uzsin, uxz-uysin);
			m.row[1]=AVector(uxy-uzsin, c+uy2, uyz+uxsin);
			m.row[2]=AVector(uxz+uysin, uyz-uxsin, c+uz2);
			m.row[3]=AVector(0, 0, 0, 1);

			return m;
			}

		inline AMatrix AMatrixTranslate(const AVector& v)
			{
			return AMatrix(
				AVector(1, 0, 0, v.x),
				AVector(0, 1, 0, v.y),
				AVector(0, 0, 1, v.z),
				AVector(0, 0, 0,   1));
			}

		inline AMatrix AMatrixSkew(const AVector& a)
			{
			// AVectorCross(AVector(), AVector) == AVectorTransform(AMatrixSkew(AVector()), AVector())

			return AMatrix(
				AVector(   0, -a.z,  a.y, 0),
				AVector( a.z,    0, -a.x, 0),
				AVector(-a.y,  a.x,    0, 0),
				AVector(   0,    0,    0, 1));
			}


		inline AMatrix AMatrixZero()
			{
			return AMatrix(
				AVector(0.0f, 0.0f, 0.0f, 0.0f),
				AVector(0.0f, 0.0f, 0.0f, 0.0f),
				AVector(0.0f, 0.0f, 0.0f, 0.0f),
				AVector(0.0f, 0.0f, 0.0f, 0.0f));
			}

		inline AMatrix AMatrixIdentity()
			{
			return AMatrix(
				AVector(1.0f, 0.0f, 0.0f, 0.0f),
				AVector(0.0f, 1.0f, 0.0f, 0.0f),
				AVector(0.0f, 0.0f, 1.0f, 0.0f),
				AVector(0.0f, 0.0f, 0.0f, 1.0f));
			}

		inline AMatrix AMatrixFill(float v)
			{
			return AMatrix(
				AVector(v, v, v, v),
				AVector(v, v, v, v),
				AVector(v, v, v, v),
				AVector(v, v, v, v));
			}

		inline AMatrix AMatrixDiagonal(float v)
			{
			return AMatrix(
				AVector(v, 0.0f, 0.0f, 0.0f),
				AVector(0.0f, v, 0.0f, 0.0f),
				AVector(0.0f, 0.0f, v, 0.0f),
				AVector(0.0f, 0.0f, 0.0f, v));
			}
		}
	}
