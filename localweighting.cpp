#include <iostream>
#include "localweighting.h"

using namespace std;

void computeC_symmetric_full( Eigen::MatrixXd &W, Eigen::MatrixXd &C, const Eigen::MatrixXd &D, int n)
{
			#pragma omp parallel for schedule(dynamic)
			for (int i=0; i<n; ++i)
			{
				for (int j=0; j<n; ++j)
				{
					double s=0.0;
					s = 0.5*(W.col(j)+W.col(i)).sum(); // if the matrix is dense this option is incredibly FAST!
					C.coeffRef(i,j) += 2.0*(W.coeffRef(i,j)+s)/(D.coeffRef(i)+D.coeffRef(j));
				}
			}
}

void computeC_symmetric_sparse(Eigen::MatrixXd &W, Eigen::MatrixXd &C, const Eigen::MatrixXd &D, int n)
{
			#pragma omp parallel for schedule(dynamic)
			for (int i=0; i<n; ++i)
			{
				for (int j=0; j<n; ++j)
				{
					double s=0.0;
					for (int k=0; k<n; ++k)
					{
						// if the matrix is symmetric this option is much faster, access k 
						// as rows instead as columns
						double Wjk = W.coeffRef(j,k);	
						int delta_jk = (Wjk!=0);
						if (delta_jk==0)
							continue;
						double Wik = W.coeffRef(i,k);
						int delta_ik = (Wik!=0);
						if (delta_ik==0)
							continue;
						s+= (delta_ik*delta_jk*(Wik+Wjk)*0.5);
					}
					C.coeffRef(i,j) += 2.0*(W.coeffRef(i,j)+s)/(D.coeffRef(i)+D.coeffRef(j));
				}
			}
}

void compute_C_asymmetric_full(Eigen::MatrixXd &W, Eigen::MatrixXd &C, const Eigen::MatrixXd &D, int n)
{
			#pragma omp parallel for schedule(dynamic)
			for (int i=0; i<n; ++i)
			{
				for (int j=0; j<n; ++j)
				{
					double s=0.0;
					s = 0.5*(W.row(j)+W.row(i)).sum();// row access is slower than columns access
					C.coeffRef(i,j) += 2.0*(W.coeffRef(i,j)+s)/(D.coeffRef(i)+D.coeffRef(j));
				}
			}
}

void compute_C_asymmetric_sparse(Eigen::MatrixXd &W, Eigen::MatrixXd &C, const Eigen::MatrixXd &D, int n)
{
			#pragma omp parallel for schedule(dynamic)
			for (int i=0; i<n; ++i)
			{
				for (int j=0; j<n; ++j)
				{
					double s=0.0;
					for (int k=0; k<n; ++k)
					{
						// if the matrix is symmetric this option is much faster, access k 
						// as rows instead as columns
						double Wjk = W.coeffRef(k,j);	
						int delta_jk = (Wjk!=0);
						if (delta_jk==0)
							continue;
						double Wik = W.coeffRef(k,i);
						int delta_ik = (Wik!=0);
						if (delta_ik==0)
							continue;
						s+= (delta_ik*delta_jk*(Wik+Wjk)*0.5);
					}
					C.coeffRef(i,j) += 2.0*(W.coeffRef(i,j)+s)/(D.coeffRef(i)+D.coeffRef(j));
				}
			}
}


Eigen::MatrixXd localWeighting( const Eigen::MatrixXd &W, bool isFull, bool isSymmetric)
{
	int n = W.rows();
	double Ls = (W.count()-n)/2; //count number of edges of the graph

	Eigen::MatrixXd C = Eigen::MatrixXd::Zero(n,n);
	double Ws = 0.5*W.sum();

	Eigen::VectorXd D = W.colwise().sum();

	if (isFull)
	{
		if (isSymmetric)
		{
			computeC_symmetric_full(const_cast<Eigen::MatrixXd &>(W),C,D,n);
		}
		else
		{
			// this is a trick to ensure vectorizatoin and no cache misses! some tranpositions have to be made though
			const_cast<Eigen::MatrixXd &>(W).transposeInPlace();
			computeC_symmetric_full(const_cast<Eigen::MatrixXd &>(W),C,D,n);
			const_cast<Eigen::MatrixXd &>(W).transposeInPlace();
			C.transposeInPlace();
			// the original code use vertical access to rows, but is slower
			//compute_C_asymmetric_full(const_cast<Eigen::MatrixXd &>(W),C,D,n);
		}
	}
	else
	{
		if (isSymmetric)
		{
			computeC_symmetric_sparse(const_cast<Eigen::MatrixXd &>(W),C,D,n);
		}
		else
		{
			compute_C_asymmetric_sparse(const_cast<Eigen::MatrixXd &>(W),C,D,n);
		}
	}

	Eigen::MatrixXd G = ((Ls/Ws)*W).cwiseProduct(C);

	Eigen::VectorXd DG = G.colwise().sum();

	for (int i=0; i<n; i++)
	{
		G.row(i)/=DG(i);
	}
	
	return G;
}
