#ifndef UTILS_H
#define UTILS_H

#include "Eigen/Core"

using namespace Eigen;

#define PI 3.141592653589793

Array<double, 1, Dynamic> logsumexp(const ArrayXXd& array);
Array<double, 1, Dynamic> logmeanexp(const ArrayXXd& array);

ArrayXXd sampleNormal(int m = 1, int n = 1);

#endif