#ifndef _LOCALWEIGHTING_H_
#define _LOCALWEIGHTING_H_
#include <Eigen/Core>

Eigen::MatrixXd localWeighting(const Eigen::MatrixXd &W, bool isFull, bool isSymmetric);
#endif