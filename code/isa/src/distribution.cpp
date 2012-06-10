#include "distribution.h"
#include <cmath>

using std::log;

double Distribution::evaluate(const MatrixXd& data) {
	return -logLikelihood(data).mean() / log(2.) / dim();
}
