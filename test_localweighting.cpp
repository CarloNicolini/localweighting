#include "localweighting.h"

int main(int argc, char *argv[])
{
	Eigen::MatrixXd W;
	int n=atoi(argv[1]);

	W.setZero(9,9);	

	W << 0, 2, 2, 0, 1, 0, 0, 0, 0
	, 2, 0, 2, 2, 0, 0, 0, 0, 0
	, 2, 2, 0, 2, 0, 0, 0, 0, 1
	, 0, 2, 2, 0, 0, 0, 1, 0, 0
	, 1, 0, 0, 0, 0, 2, 0, 0, 2
	, 0, 0, 0, 0, 2, 0, 2, 2, 0
	, 0, 0, 0, 1, 0, 2, 0, 2, 0
	, 0, 0, 0, 0, 0, 2, 2, 0, 2
	, 0, 0, 1, 0, 2, 0, 0, 2, 0 ;

/*
	W << 4, 2, 2, 4, 1, 4, 4, 4, 4
	, 2, 4, 2, 2, 4, 4, 4, 4, 4
	, 2, 2, 4, 2, 4, 4, 4, 4, 1
	, 4, 2, 2, 4, 4, 4, 1, 4, 4
	, 1, 4, 4, 4, 4, 2, 4, 4, 2
	, 4, 4, 4, 4, 2, 4, 2, 2, 4
	, 4, 4, 4, 1, 4, 2, 4, 2, 4
	, 4, 4, 4, 4, 4, 2, 2, 4, 2
	, 4, 4, 1, 4, 2, 4, 4, 2, 4 ;
*/
	if (n!=9)
	{
		srand(0);
		W.setRandom(n,n);
	}
	//std::cout << W << std::endl << std::endl;
	Eigen::MatrixXd G(n,n);
	bool isDense = true;
	bool isSymmetric = true;
	G = localWeighting(W,true,isSymmetric);
	return 0;
}