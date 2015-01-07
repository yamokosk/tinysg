/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License, 
 * or (at your option) any later version. The text of the GNU Lesser General 
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
 * more details.
 *
 *************************************************************************/

#include "matrix.h"
#include <stdexcept>

namespace sceneml {

#define _T(i,j) T[(i)*4+(j)]
#define _R(i,j) R[(i)*4+(j)]

#define SET_4x4_IDENTITY \
  _T(0,0) = REAL(1.0); \
  _T(0,1) = REAL(0.0); \
  _T(0,2) = REAL(0.0); \
  _T(0,3) = REAL(0.0); \
  _T(1,0) = REAL(0.0); \
  _T(1,1) = REAL(1.0); \
  _T(1,2) = REAL(0.0); \
  _T(1,3) = REAL(0.0); \
  _T(2,0) = REAL(0.0); \
  _T(2,1) = REAL(0.0); \
  _T(2,2) = REAL(1.0); \
  _T(2,3) = REAL(0.0); \
  _T(3,0) = REAL(0.0); \
  _T(3,1) = REAL(0.0); \
  _T(3,2) = REAL(0.0); \
  _T(3,3) = REAL(1.0);

void dTSetIdentity(dMatrix4 T)
{
	dAASSERT(T);
	SET_4x4_IDENTITY;
}

void dTFromTrans(dMatrix4 T, dReal x, dReal y, dReal z)
{
	dTSetIdentity(T);
	_T(0,3) = x;
	_T(1,3) = y;
	_T(2,3) = z;
}

void dTFromAxisAndAngle (dMatrix4 T, dReal ax, dReal ay, dReal az, dReal angle)
{
	dAASSERT (T);
	dQuaternion q;
	dQFromAxisAndAngle (q,ax,ay,az,angle);
	dTfromQ (T,q);
}

void dTfromQ (dMatrix4 T, const dQuaternion q)
{
	dAASSERT (q && T);
	// q = (s,vx,vy,vz)
	dReal qq1 = 2*q[1]*q[1];
	dReal qq2 = 2*q[2]*q[2];
	dReal qq3 = 2*q[3]*q[3];
	_T(0,0) = 1 - qq2 - qq3;
	_T(0,1) = 2*(q[1]*q[2] - q[0]*q[3]);
	_T(0,2) = 2*(q[1]*q[3] + q[0]*q[2]);
	_T(0,3) = REAL(0.0);
	_T(1,0) = 2*(q[1]*q[2] + q[0]*q[3]);
	_T(1,1) = 1 - qq1 - qq3;
	_T(1,2) = 2*(q[2]*q[3] - q[0]*q[1]);
	_T(1,3) = REAL(0.0);
	_T(2,0) = 2*(q[1]*q[3] - q[0]*q[2]);
	_T(2,1) = 2*(q[2]*q[3] + q[0]*q[1]);
	_T(2,2) = 1 - qq1 - qq2;
	_T(2,3) = REAL(0.0);
	_T(3,0) = REAL(0.0);
	_T(3,1) = REAL(0.0);
	_T(3,2) = REAL(0.0);
	_T(3,3) = REAL(1.0);	
}

void dTFromEuler123(dMatrix4 T, dReal x, dReal y, dReal z)
{
	dReal cx,cy,cz,sx,sy,sz;
	dAASSERT (T);
	cx = cos(x);
	cy = cos(y);
	cz = cos(z);
	sx = sin(x);
	sy = sin(y);
	sz = sin(z);
	
	_T(0,0) = cy*cz;
	_T(0,1) = -cy*sz;
	_T(0,2) = sy;
	_T(0,3) = REAL(0.0);
	_T(1,0) = sx*sy*cz+cx*sz;
	_T(1,1) = -sx*sy*sz+cx*cz;
	_T(1,2) = -sx*cy;
	_T(1,3) = REAL(0.0);
	_T(2,0) = -cx*sy*cz+sx*sz;
	_T(2,1) = cx*sy*sz+sx*cz;
	_T(2,2) = cx*cy;
	_T(2,3) = REAL(0.0);
	_T(3,0) = REAL(0.0);
	_T(3,1) = REAL(0.0);
	_T(3,2) = REAL(0.0);
	_T(3,3) = REAL(1.0);
}

void dTFromRAndPos(dMatrix4 T, const dMatrix3 rot, const dVector3 pos)
{
	dAASSERT(T);
	memcpy(T,	rot, sizeof(dMatrix3));
	_T(0,3) = pos[0];
	_T(1,3) = pos[1];
	_T(2,3) = pos[2];
	_T(3,0) = REAL(0.0);
	_T(3,1) = REAL(0.0);
	_T(3,2) = REAL(0.0);
	_T(3,3) = REAL(1.0);
}

void dRFromT(dMatrix3 R, const dMatrix4 T)
{
	dAASSERT(R);
	memcpy(R, T, sizeof(dMatrix3));
}

dReal dDeterminant3(const dMatrix3 R)
{
	return ( _R(0,0)*_R(1,1)*_R(2,2) 
			 - _R(0,0)*_R(1,2)*_R(2,1)
			 - _R(1,0)*_R(0,1)*_R(2,2)
			 + _R(1,0)*_R(0,2)*_R(2,1)
			 + _R(2,0)*_R(0,1)*_R(1,2)
			 - _R(2,0)*_R(0,2)*_R(1,1) );
}

void dTPrint(dMatrix4 T)
{
	dAASSERT(T);
	printf("\t[%g, %g, %g, %g]\n", _T(0,0), _T(0,1), _T(0,2), _T(0,3));
	printf("\t[%g, %g, %g, %g]\n", _T(1,0), _T(1,1), _T(1,2), _T(1,3));
	printf("\t[%g, %g, %g, %g]\n", _T(2,0), _T(2,1), _T(2,2), _T(2,3));
	printf("\t[%g, %g, %g, %g]\n", _T(3,0), _T(3,1), _T(3,2), _T(3,3));
}

void dRPrint(dMatrix3 T)
{
	dAASSERT(T);
	printf("\t[%g, %g, %g]\n", _T(0,0), _T(0,1), _T(0,2));
	printf("\t[%g, %g, %g]\n", _T(1,0), _T(1,1), _T(1,2));
	printf("\t[%g, %g, %g]\n", _T(2,0), _T(2,1), _T(2,2));
}

void dVPrint(dVector3 V)
{
	dAASSERT(V);
	printf("\t[%g, %g, %g]\n", V[0], V[1], V[2]);
}

// From nrutil.h
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))
static float maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))
static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

// Taken from NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
dReal pythag(dReal a, dReal b)
{
	// Computes (a2 + b2)1/2 without destructive underflow or overflow.
	dReal absa,absb;
	absa=dFabs(a);
	absb=dFabs(b);
	if (absa > absb) return absa*dSqrt(REAL(1.0)+SQR(absb/absa));
	else return (absb == REAL(0.0) ? REAL(0.0) : absb*dSqrt(REAL(1.0)+SQR(absa/absb)));
}

/*template<class T>
T signum(T x)
{
	if(x>0) return 1;
	else if(x<0) return -1;
	else return 0;
}

template<class T>
T max(T x, T y)
{
	if(x>=y) return x;
	else return y;
}

template<class T>
T min(T x, T y)
{
	if(x>=y) return y;
	else return x;
}*/

#define _a(i,j) a[(i)*4+(j)]
#define _v(i,j) v[(i)*4+(j)]
// Adopted from NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
void svdcmp(dReal *a, dReal *w, dReal *v)
{
	int m=3, n=3;
	/* Given a matrix a[0..m-1][0..n-1], this routine computes its singular value decomposition, A = U·W·V'. The
	  * matrix U replaces a on output. The diagonal matrix of singular values W is output
	  * as a vector w[1..n]. The matrix V (not the transpose V T ) is output as v[1..n][1..n].
	  */
	int flag,i,its,j,jj,k,l,nm;
	dReal anorm,c,f,g,h,s,scale,x,y,z,*rv1;
	rv1=new dReal[n];
	g=scale=anorm=0.0; // Householder reduction to bidiagonal form.
	for (i=0;i<n;i++) {
		l=i+1;
		rv1[i]=scale*g;
		g=s=scale=0.0;
		if (i < m) 
		{
			//for (k=i;k<m;k++) scale += dFabs(a[k][i]);
			for (k=i;k<m;k++) scale += dFabs(_a(k,i));
			if (scale) 
			{
				for (k=i;k<m;k++) 
				{
					//a[k][i] /= scale;
					_a(k,i) /= scale;
					//s += a[k][i]*a[k][i];
					s += _a(k,i)*_a(k,i);
				}
				//f=a[i][i];
				f=_a(i,i);
				g = -SIGN(dSqrt(s),f);
				h=f*g-s;
				//a[i][i]=f-g;
				_a(i,i)=f-g;
				for (j=l;j<n;j++) 
				{
					for (s=0.0,k=i;k<m;k++) s += _a(k,i)*_a(k,j);//a[k][i]*a[k][j];
					f=s/h;
					for (k=i;k<m;k++) _a(k,j) += f*_a(k,i);//a[k][j] += f*a[k][i];
				}
				for (k=i;k<m;k++) _a(k,i) *= scale;//a[k][i] *= scale;
			}
		}
		w[i]=scale *g;
		g=s=scale=0.0;
		if (i < m && i != n) 
		{
			for (k=l;k<n;k++) scale += dFabs(_a(i,k));//dFabs(a[i][k]);
			if (scale) 
			{
				for (k=l;k<n;k++) 
				{
					_a(i,k) /= scale;//a[i][k] /= scale;
					s += _a(i,k)*_a(i,k);//a[i][k]*a[i][k];
				}
				f=_a(i,l);//a[i][l];
				g = -SIGN(dSqrt(s),f);
				h=f*g-s;
				_a(i,l)=f-g;//a[i][l]=f-g;
				for (k=l;k<n;k++) rv1[k]=_a(i,k)/h;//a[i][k]/h;
				for (j=l;j<m;j++) 
				{
					for (s=0.0,k=l;k<n;k++) s += _a(j,k)*_a(i,k);//a[j][k]*a[i][k];
					for (k=l;k<n;k++) _a(j,k) += s*rv1[k];//a[j][k] += s*rv1[k];
				}
				for (k=l;k<n;k++) _a(i,k) *= scale;//a[i][k] *= scale;
			}
		}
		anorm=FMAX(anorm,(dFabs(w[i])+dFabs(rv1[i])));
	}
	
	for (i=n-1;i>=0;i--) //Accumulation of right-hand transformations.
	{ 
		if (i < n) 
		{
			if (g) 
			{
				for (j=l;j<n;j++) //Double division to avoid possible underflow.
					_v(j,i)=(_a(i,j)/_a(i,l))/g;//(a[i][j]/a[i][l])/g;//v[j][i]=(_a(i,j)/_a(i,l))/g;//(a[i][j]/a[i][l])/g;
				for (j=l;j<n;j++) 
				{
					for (s=0.0,k=l;k<n;k++) s += _a(i,k)*_v(k,j);//s += a[i][k]*v[k][j];
					for (k=l;k<n;k++) _v(k,j) += s*_v(k,i);//v[k][j] += s*v[k][i];
				}
			}
			for (j=l;j<n;j++) _v(i,j)=_v(j,i)=0.0;//v[i][j]=v[j][i]=0.0;
		}
		_v(i,i)=1.0;//v[i][i]=1.0;
		g=rv1[i];
		l=i;
	}
	
	for (i=IMIN(m,n)-1;i>=0;i--) // Accumulation of left-hand transformations.
	{ 
		l=i+1;
		g=w[i];
		for (j=l;j<n;j++) _a(i,j)=0.0;//a[i][j]=0.0;
		if (g) 
		{
			g=REAL(1.0)/g;
			for (j=l;j<n;j++) 
			{
				for (s=0.0,k=l;k<m;k++) s += _a(k,i)*_a(k,j);//a[k][i]*a[k][j];
				f=(s/_a(i,i))*g;//f=(s/a[i][i])*g;
				for (k=i;k<m;k++) _a(k,j) += f*_a(k,i);//a[k][j] += f*a[k][i];
			}
			for (j=i;j<m;j++) _a(j,i) *= g;//a[j][i] *= g;
		} else for (j=i;j<m;j++) _a(j,i)=0.0;//a[j][i]=0.0;
		//++a[i][i];
		++_a(i,i);
	}
	
	// Diagonalization of the bidiagonal form: Loop over
	//singular values, and over allowed iterations.
	for (k=n-1;k>=0;k--) { 
		for (its=1;its<=30;its++) 
		{
			flag=1;
			for (l=k;l>=0;l--) //Test for splitting.
			{
				nm=l-1; // Note that rv1[1] is always zero.
				if ((float)(dFabs(rv1[l])+anorm) == anorm) 
				{
					flag=0;
					break;
				}
				if ((float)(dFabs(w[nm])+anorm) == anorm) break;
			}
			if (flag) 
			{
				c=0.0; // Cancellation of rv1[l], if l > 1.
				s=1.0;
				for (i=l;i<k;i++) 
				{
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if ((float)(dFabs(f)+anorm) == anorm) break;
					g=w[i];
					h=pythag(f,g);
					w[i]=h;
					h=REAL(1.0)/h;
					c=g*h;
					s = -f*h;
					for (j=0;j<m;j++) 
					{
						y=_a(j,nm);//a[j][nm];
						z=_a(j,i);//a[j][i];
						_a(j,nm)=y*c+z*s;//a[j][nm]=y*c+z*s;
						_a(j,i)=z*c-y*s;//a[j][i]=z*c-y*s;
					}
				}
			}
			z=w[k];
			if (l == k) // Convergence.
			{
				if (z < 0.0) // Singular value is made nonnegative.
				{
					w[k] = -z;
					for (j=0;j<n;j++) _v(j,k) = -_v(j,k);//v[j][k] = -v[j][k];
				}
				break;
			}
			
			if (its == 30) 
				throw std::runtime_error("no convergence in 30 svdcmp iterations");
			x=w[l]; // Shift from bottom 2-by-2 minor.
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=((y-z)*(y+z)+(g-h)*(g+h))/(REAL(2.0)*h*y);
			g=pythag(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			c=s=1.0; // Next QR transformation:
			for (j=l;j<nm;j++) 
			{
				i=j+1;
				g=rv1[i];
				y=w[i];
				h=s*g;
				g=c*g;
				z=pythag(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=0;jj<n;jj++) 
				{
					x=_v(jj,j);//v[jj][j];
					z=_v(jj,j);//v[jj][j];
					_v(jj,j)=x*c+z*s;//v[jj][j]=x*c+z*s;
					_v(jj,i)=z*c-x*s;//v[jj][i]=z*c-x*s;
				}
				z=pythag(f,h);
				w[j]=z; // Rotation can be arbitrary if z = 0.
				if (z) 
				{
					z=REAL(1.0)/z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (jj=0;jj<m;jj++) 
				{
					y=_a(jj,j);//a[jj][j];
					z=_a(jj,i);//a[jj][i];
					_a(jj,j)=y*c+z*s;//a[jj][j]=y*c+z*s;
					_a(jj,i)=z*c-y*s;//a[jj][i]=z*c-y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			w[k]=x;
		}
	}
	delete [] rv1;
}

void dSVD(dMatrix3 U, dMatrix3 V, dVector3 D, const dMatrix3 C) 
{
	memcpy(U, C, sizeof(dMatrix3));
	svdcmp(U, D, V);
}

};
