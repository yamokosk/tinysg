#include "pose_estimation.h"
#include "matrix.h"

#include <stdexcept>

using namespace sceneml;

PoseEstimatorBase::PoseEstimatorBase()
{
	dTSetIdentity(tmatrix_);
}

void PoseEstimatorBase::getPose(dMatrix4 T)
{
	memcpy(T, tmatrix_, sizeof(dMatrix4));
}

// From: http://www.kwon3d.com/theory/jkinem/rotmat.html	
void SVDEstimator::estimate(const dReal *pX1, const dReal *pX2, int nMarkers)
{
/*
	// Normally to compute the rotation matrix from a set of marker data, you
	// would have 12 unknowns (9 for the rotation matrix and 3 for the location
	// of the center of rotation. 12 unknowns would then necessitate 4 markers.
	// To reduce this to three markers, we will calculate the rotational 
	// transformation about the mean-coordinate point within the body. See kwon
	// for more details
	dVector3 P1_1o_mo, P2_2o_mo;
	memset(P1_1o_mo, 0, sizeof(dVector3)); memset(P2_2o_mo, 0, sizeof(dVector3));
	for (int dim=0; dim < 3; ++dim) {
		for (int n=0; n < nMarkers; ++n) {
			P1_1o_mo[dim] += pX1[n*3 + dim];
			P2_2o_mo[dim] += pX2[n*3 + dim];
		}
		P1_1o_mo[dim] /= (dReal)nMarkers;
		P2_2o_mo[dim] /= (dReal)nMarkers;
	}
	
	dReal *Y1 = new dReal[nMarkers*3], *Y2 = new dReal[nMarkers*3];
	memset(Y1, 0, sizeof(Y1)); memset(Y2, 0, sizeof(Y2));
	for (int n=0; n < nMarkers; ++n) {
		for (int dim=0; dim < 3; ++dim) {
			Y1[n*3 + dim] = pX1[n*3 + dim] - P1_1o_mo[dim];
			Y2[n*3 + dim] = pX2[n*3 + dim] - P2_2o_mo[dim];
		}
	}
	
	// Compute the cross-dispersion matrix or the correlation matrix, C
	// 	C  = (1/N) * sum_1_N (y2i * y1i.transpose)
	dMatrix3 C; memset(C, 0, sizeof(dMatrix3));
	for (int n=0; n < nMarkers; ++n) dMULTIPLYOPV_333(C, +=, (Y2 + n*3), (Y1 + n*3));
	for (int n=0; n < 12; ++n) C[n] *= (REAL(1.0) / (float)nMarkers);
	delete [] Y1;
	delete [] Y2;

	// Singular value decomposition of the correlation matrix
	//	(U,D,V) = svd(C);
	dMatrix3 U, V; dVector3 d;
	memset(U, 0, sizeof(dMatrix3)); memset(V, 0, sizeof(dMatrix3));
	memset(d, 0, sizeof(dVector3));
	dSVD(U, V, d, C);

	// One problem that may arise is that the rotation matrix can have a 
	// determinant of -1. In other words, one can obtain a reflection matrix of 
	// the desired rotation matrix. To solve this problem define W,	
	dMatrix3 W, temp; dRSetIdentity(W);
	dMULTIPLY2_333(temp,U,V); // temp = U * V'
	W[10] = dDeterminant3(temp);
		
	// Finally the rotation matrix R = U * W * V.t'
	dMULTIPLY2_333(temp, W, V);
	dMatrix3 R_1_2;
	dMULTIPLY0_333(R_1_2, U, temp);
	
	// And translation
	dVector3 P1_2o_mo;
	dMULTIPLY0_331(P1_2o_mo, R_1_2, P2_2o_mo);
	dVector3 P1_1o_2o;
	for (int n=0; n < 3; ++n) P1_1o_2o[n] = P1_1o_mo[n] - P1_2o_mo[n];
	
	// Copy estimate into class variable
	dTFromRAndPos(tmatrix_, R_1_2, P1_1o_2o);
*/
//	std::cout << "Entering alternate SVD method" << std::endl;
	
	Tracer et("Pose Estimator Algorithm");
	
	Try {
		// ALTERNATE METHOD USING NEWMAT
		et.ReName("Block A");
		Matrix X1(3,nMarkers), X2(3,nMarkers);
		for (int c=0; c < nMarkers; ++c) {
			for (int r=0; r < 3; ++r) {
				X1(r+1,c+1) = pX1[c*3 + r];
				X2(r+1,c+1) = pX2[c*3 + r];
			}
		}
//		std::cout << "X1:" << std::endl << X1 << std::endl;
//		std::cout << "X2:" << std::endl << X2 << std::endl;
		
		// Normally to compute the rotation matrix from a set of marker data, you
		// would have 12 unknowns (9 for the rotation matrix and 3 for the location
		// of the center of rotation. 12 unknowns would then necessitate 4 markers.
		// To reduce this to three markers, we will calculate the rotational 
		// transformation about the mean-coordinate point within the body. See kwon
		// for more details
		et.ReName("Block B");
		ColumnVector P1_1o_mo(3), P2_2o_mo(3);
		Matrix Y1(3, nMarkers), Y2(3, nMarkers);
		P1_1o_mo = 0; P2_2o_mo = 0; Y1 = 0; Y2 = 0;
		for (int dim=1; dim <= 3; ++dim) {
			P1_1o_mo(dim) = X1.row(dim).Sum() / (float)nMarkers;
			P2_2o_mo(dim) = X2.row(dim).Sum() / (float)nMarkers;
			
			for (int n=1; n <= nMarkers; ++n) {
				Y1(dim, n) = X1(dim, n) - P1_1o_mo(dim);
				Y2(dim, n) = X2(dim, n) - P2_2o_mo(dim);
			}
		}
		
		// Compute the cross-dispersion matrix or the correlation matrix, C
		// 	C  = (1/N) * sum_1_N (y2i * y1i.transpose)
		et.ReName("Block C");
		Matrix C(3,3); C = 0;
		for (int n=1; n <= nMarkers; ++n) {
			C += ( Y2.column(n) * Y1.column(n).t() );
		}
		C *= (1.0 / (float)nMarkers);
		
		// Singular value decomposition of the correlation matrix
		//	(U,D,V) = svd(C);
		et.ReName("Block D");
		Matrix U(3,3), V(3,3), W(3,3); DiagonalMatrix D(3);
		SVD(C, D, U, V);

		// One problem that may arise is that the rotation matrix can have a 
		// determinant of -1. In other words, one can obtain a reflection matrix of 
		// the desired rotation matrix. To solve this problem define W,	
		et.ReName("Block E");
		W = IdentityMatrix(3);
		//W(3,3) = (U * V.t()).determinant();
		
		// Finally the rotation matrix R is,
		et.ReName("Block F");
		Matrix R_2_1 = U * W * V.t();
		Matrix R_1_2 = R_2_1.t();
		//Matrix R_1_2 = U * V.t();
		
		// And translation
		et.ReName("Block G");
		ColumnVector P1_1o_2o = P1_1o_mo - R_1_2 * P2_2o_mo;
		
//		std::cout << "R_1_2" << std::endl << R_1_2 << std::endl;
//		std::cout << "P1_1o_2o: " << P1_1o_2o.t() << std::endl;
		
		// Convert answer into tmatrix_
		et.ReName("Block H");
		int k = 1, j = 1;
		for (int r=0; r < 3; ++r) 
		{
			for (int c=0; c < 4; ++c)
			{
				if (c == 3) {
					this->tmatrix_[r*4 + c] = P1_1o_2o(k);	
					k++;
				} else {
					this->tmatrix_[r*4 + c] = R_1_2(r+1,j);
					j++;
				}
			}
			j = 1;
		}
//		std::cout << "tmatrix_: " << std::endl;
//		dTPrint(this->tmatrix_);
		std::cout << std::endl;
	} CatchAll { throw std::runtime_error( BaseException::what() ); }
		
	return;
}
