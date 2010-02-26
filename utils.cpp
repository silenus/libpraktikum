#include "utils.h"


double *utils::createArray(const int length, const double value) {
	double *array = new double[length];
	for (int i = 0; i < length; i++)
		array[i] = value;
	return array;
}

void utils::transform(double *array, const unsigned int length, const string &expression) {
	TF1 *function = new TF1("transformFunction", expression.c_str());
	transform(array, length, function);
	delete function;
}

void utils::transform(double *array, const unsigned int length, TF1 *function) {
	for (unsigned int i = 0; i < length; i++)
		array[i] = function->Eval(array[i]);
}

double utils::sum(const double *numbers, const unsigned int length) {
	double sum = 0;
	for (unsigned int i = 0; i < length; i++)
		sum += numbers[i];
	return sum;
}

double utils::mean(const double *numbers, const unsigned int length, double &errorMean, double &rms) {
	double mean = sum(numbers, length) / length;

	rms = 0;
	for (unsigned int i = 0; i < length; i++)
		rms += (mean - numbers[i]) * (mean - numbers[i]);
	rms = sqrt(rms / (length - 1));

	errorMean = rms / sqrt(length);
	return mean;
}
