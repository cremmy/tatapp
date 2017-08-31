/*
 * aquaternion.h
 *
 *  Created on: 26 lut 2017
 *      Author: crm
 */

#pragma once

#include "amath.h"

#define LOG_STR_QUATERNION(q) "AQuaternion " #q ": [%7.4f %7.4f %7.4f %7.4f]", q.r, q.i, q.j, q.k

// QUATERNION_ASSUME_ALWAYS_NORMALIZED - do liczenia odwrotnosci, usuwa dzielenie przez kwadrat normy kwaternionu
// (...bo zakladamy, ze jest rowna 1)
#define QUATERNION_ASSUME_ALWAYS_NORMALIZED

namespace Engine
	{
	namespace Math
		{
		/** Kwaternion
		 * Do obliczania obrot�w u�ywa� prawej r�ki: kciuk w kierunku osi obrotu, zgi�te palce pokazuj� kierunek obrotu wektor�w
		 */
		struct AQuaternion
			{
			explicit AQuaternion(float r=0.0f, float i=0.0f, float j=0.0f, float k=0.0f):
				r(r), i(i), j(j), k(k) {}
			AQuaternion(const AQuaternion& a): r(a.r), i(a.i), j(a.j), k(a.k) {}

			inline AQuaternion& operator=(const AQuaternion& a) {r=a.r; i=a.i; j=a.j; k=a.k; return *this;}

			inline operator AMatrix() {return matrix();}
			inline AMatrix matrix() const
				{
				return AMatrix(AVector(1.0f-2.0f*j*j-2.0f*k*k,      2.0f*i*j-2.0f*k*r,      2.0f*i*k+2.0f*j*r, 0.0f),
							   AVector(     2.0f*i*j+2.0f*k*r, 1.0f-2.0f*i*i-2.0f*k*k,      2.0f*j*k-2.0f*i*r, 0.0f),
							   AVector(     2.0f*i*k-2.0f*j*r,      2.0f*j*k+2.0f*i*r, 1.0f-2.0f*i*i-2.0f*j*j, 0.0f),
							   AVector(0.0f, 0.0f, 0.0f, 1.0f));
				}

			float r, i, j, k;
			} __attribute__ ((aligned(32)));

		AQuaternion AQuaternionAdd(const AQuaternion& a, const AQuaternion& b);
		AQuaternion AQuaternionSub(const AQuaternion& a, const AQuaternion& b);
		AQuaternion AQuaternionMultiply(const AQuaternion& a, const AQuaternion& b);
		AQuaternion AQuaternionNormalize(const AQuaternion& a);

		AQuaternion AQuaternionConjugate(const AQuaternion& a);  // Koniunkcja ( a* )
		float AQuaternionLength(const AQuaternion& a);           // Norma ( ||a|| )
		AQuaternion AQuaternionReciprocal(const AQuaternion& a); // Odwrotnosc ( a^-1 / len^2)

		AVector AVectorTransform(const AVector& a, const AQuaternion& q);

		AQuaternion AQuaternionSlerp(const AQuaternion& a, const AQuaternion& b, float p);

		AQuaternion AQuaternionFromVectors(const AVector& a, const AVector& b);
		AQuaternion AQuaternionRotate(float angle, const AVector& dir);

		// operatory
		inline AQuaternion operator+(const AQuaternion& a, const AQuaternion& b) {return AQuaternionAdd(a, b);}
		inline AQuaternion operator-(const AQuaternion& a, const AQuaternion& b) {return AQuaternionAdd(a, b);}
		inline AQuaternion operator*(const AQuaternion& a, const AQuaternion& b) {return AQuaternionMultiply(a, b);}
		inline AQuaternion operator*(const AQuaternion& a) {return AQuaternionConjugate(a);}
		inline AVector operator*(const AQuaternion& q, const AVector& a) {return AVectorTransform(a, q);}
		inline AQuaternion operator*(const AQuaternion& a, float v) {return AQuaternion(a.r*v, a.i*v, a.j*v, a.k*v);}
		inline AQuaternion operator/(const AQuaternion& a, float v) {return AQuaternion(a.r/v, a.i/v, a.j/v, a.k/v);}
		inline AQuaternion operator+(AQuaternion& a, const AQuaternion& b) {return a=AQuaternionAdd(a, b);}
		inline AQuaternion operator*=(AQuaternion& a, const AQuaternion& b) {return a=AQuaternionMultiply(a, b);}

		// inline
		inline AQuaternion AQuaternionAdd(const AQuaternion& a, const AQuaternion& b)
			{
			return AQuaternion(a.r+b.r, a.i+b.i, a.j+b.j, a.k+b.k);
			}

		inline AQuaternion AQuaternionSub(const AQuaternion& a, const AQuaternion& b)
			{
			return AQuaternion(a.r-b.r, a.i-b.i, a.j-b.j, a.k-b.k);
			}

		inline AQuaternion AQuaternionMultiply(const AQuaternion& a, const AQuaternion& b)
			{
			/*return AQuaternion(a.k*b.r+a.r*b.k+a.i*b.j-a.j*b.i, // Doooobra... skad ja wytrzasnalem ten *niepoprawny* wzor?
							   a.k*b.i-a.r*b.j+a.i*b.k+a.j*b.r,
							   a.k*b.j+a.r*b.i-a.i*b.r+a.j*b.k,
							   a.k*b.k-a.r*b.r-a.i*b.i-a.j*b.j);*/
			return AQuaternion(a.r*b.r - a.i*b.i - a.j*b.j - a.k*b.k,
							   a.r*b.i + a.i*b.r + a.j*b.k - a.k*b.j,
							   a.r*b.j - a.i*b.k + a.j*b.r + a.k*b.i,
							   a.r*b.k + a.i*b.j - a.j*b.i + a.k*b.r);
			}

		inline AQuaternion AQuaternionNormalize(const AQuaternion& a)
			{
			const float len=AQuaternionLength(a);

			return AQuaternion(a.r/len, a.i/len, a.j/len, a.k/len);
			}

		inline AQuaternion AQuaternionConjugate(const AQuaternion& a)
			{
			return AQuaternion(a.r, -a.i, -a.j, -a.k);
			}

		inline float AQuaternionLength(const AQuaternion& a)
			{
			return sqrt(a.r*a.r + a.i*a.i + a.j*a.j + a.k*a.k);
			}

		inline AQuaternion AQuaternionReciprocal(const AQuaternion& a)
			{
			// a^-1 = a*/||a||^2 // Koniunkcja podzielona przez kwadrat normy

#ifndef QUATERNION_ASSUME_ALWAYS_NORMALIZED
			const float len=AQuaternionLength(a);
			const float len2=len*len;

			return AQuaternion(a.r/len2, -a.i/len2, -a.j/len2, -a.k/len2);
#else
			return AQuaternion(a.r, -a.i, -a.j, -a.k);
#endif
			}

		inline AVector AVectorTransform(const AVector& a, const AQuaternion& q)
			{
			const AQuaternion b=AQuaternionMultiply(AQuaternionMultiply(q, AQuaternion(0.0f, a.x, a.y, a.z)), AQuaternionReciprocal(q));

			return AVector(b.i, b.j, b.k, a.w);
			}

		inline AQuaternion AQuaternionSlerp(const AQuaternion& a, const AQuaternion& b, float p)
			{
			const float DOTP=(AVector(a.r, a.i, a.j, a.k)|AVector(b.r, b.i, b.j, b.k));
			const float THETA=acos(DOTP)*p;

			if(DOTP>0.9995f)
				return AQuaternionNormalize(a+(b-a)*p);

			return (a*cos(THETA)) + AQuaternionNormalize(b-a*DOTP)*sin(THETA);
			}

		inline AQuaternion AQuaternionFromVectors(const AVector& a, const AVector& b)
			{
			// XXX Yhhh... to chyba nie działa najlepiej
			float ablen=sqrt((a|a)*(b|b));
			float qreal=ablen+(a|b);

			AVector w;

			if(qreal<1e-6)
				{
				w=(fabs(a.x)>fabs(a.z))?AVector(-a.y, a.x, 0.0f):AVector(0.0f, -a.z, a.y);
				qreal=0.0f;
				}
			else
				{
				w=a^b;
				}

			AQuaternion q(qreal, w.x, w.y, w.z);

			return AQuaternionNormalize(q);
			}

		inline AQuaternion AQuaternionRotate(float angle, const AVector& dir)
			{
			float s, c;

			sincos(angle*M_PI/360.0f, s, c); // *0.5*M_PI/180.0f == *M_PI/360.0f

			return AQuaternion(c, dir.x*s, dir.y*s, dir.z*s);
			}
		}
	}
