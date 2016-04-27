#include <iostream>
#include "localweighting.h"
#include "mex.h"

#define IS_REAL_2D_FULL_DOUBLE(P) (!mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P) && mxIsDouble(P))
#define IS_REAL_SCALAR(P) (IS_REAL_2D_FULL_DOUBLE(P) && mxGetNumberOfElements(P) == 1)

void mexFunction( int nOutputArgs, mxArray *outputArgs[], int nInputArgs, const mxArray * inputArgs[])
{	
	if( nInputArgs > 3 ) // Check the number of arguments
		mexErrMsgTxt("Too many input arguments.");
	if (nOutputArgs > 1)
		mexErrMsgTxt("Too many output arguments.");
	
	if(!IS_REAL_2D_FULL_DOUBLE(inputArgs[0])) // Check A
		mexErrMsgTxt("A must be a real 2D full double array.");
	
	int M = mxGetM(inputArgs[0]);
	int N = mxGetN(inputArgs[0]);

	// Create the input matrix W as Eigen Matrix mapping the input matrix
	Eigen::Map<Eigen::MatrixXd> W( mxGetPr(inputArgs[0]) ,M,N);
	// Allocate space for the output matrix G
	Eigen::MatrixXd G = localWeighting(W,true,true); // Implementazione corretta, restituisce i risultati giusti
	//std::cout << G << std::endl;
	outputArgs[0] = mxCreateDoubleMatrix(M,N,mxREAL);
	memcpy(mxGetPr(outputArgs[0]), G.data(), sizeof(double)*M*N);
	return;
}
