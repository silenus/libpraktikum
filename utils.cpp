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

double utils::min(double *numbers, const unsigned int length) {
	double min = numbers[0];
	for (unsigned int i = 0; i < length; i++) {
		if (min > numbers[i])
			min = numbers[i];
	}
	return min;
}

double utils::max(double *numbers, const unsigned int length) {
	double max = numbers[0];
	for (unsigned int i = 0; i < length; i++) {
		if (max < numbers[i])
			max = numbers[i];
	}
	return max;
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

double utils::weightedMean(const double* numbers, const double* errors, const unsigned int length, double &innerError, double &outerError) {
	double *weights = new double[length];
	// Calculate the weights
	for (unsigned int i = 0; i < length; i++)
		weights[i] = 1/ (errors[i] * errors[i]);

	double sum;
	double weightSum;
	for (unsigned int i = 0; i < length; i++) {
		sum += numbers[i] * weights[i];
		weightSum += errors[i];
	}
	double mean = sum / weightSum;

	innerError = sqrt(1 / weightSum);

	double errorMean = 0;
	outerError = 0;
	for (unsigned int i = 0; i < length; i++)
		errorMean += pow(numbers[i] - mean, 2) * weights[i];
	outerError = sqrt(errorMean / ((length - 1) * weightSum));

	delete weights;
	return mean;
}

unsigned int utils::countZeros(const double* y, const unsigned int length){
	int nZeros = 0;
	for(unsigned int i = 0; i < length; i++) {
		if(y[i]*y[i+1] < 0)
			nZeros++;
	}
	return nZeros;
}

unsigned int getZeros(const double* y,const double* x,const double* yErrors,const double* xErrors,const unsigned int length, double* xZeros, double* xZeroErrors, const unsigned int xZerosLength) {
	unsigned int nZeros = 0;//will count the zeros

	for (unsigned int i = 0; i < length && nZeros < xZerosLength; i++){
		if(y[i]*y[i+1] < 0) {
			xZeros[nZeros] = (x[i]*y[i+1]-x[i+1]*y[i])/(y[i+1]-y[i]);
			xZeroErrors[nZeros] = 1/(y[i+1]-y[i])*sqrt(pow((y[i+1]*xErrors[i]),2)+pow((y[i]*xErrors[i+1]),2)+pow((x[i]-x[i+1])/(y[i+1]-y[i]),2)*(pow((y[i+1]*yErrors[i]),2)+pow((x[i+1]*yErrors[i+1]),2)));
			//Here we still did not consider the errors due to linear approximation
			nZeros++;
		}
	}
	return nZeros;
}

string utils::printNumber(double number, double error) {
	ostringstream result;
	const short numberMagnitude = magnitude(number);
	const short errorMagnitude = magnitude(error);
	number = roundTo(number, pow(10, errorMagnitude - 1));
	error = roundTo(error, pow(10, errorMagnitude - 1));

	result.setf(ios::showpoint);
	result.setf(ios::fixed);
	result.precision( -(numberMagnitude - errorMagnitude + 1) );
	if (numberMagnitude > 5 || numberMagnitude < -3)
		result << number / pow(10, numberMagnitude) << "*10^" << numberMagnitude << " #pm " << error / pow(numberMagnitude, 10) << "*10^" << numberMagnitude;
	else
		result << number << " #pm " << error;

	return result.str();
}

short utils::magnitude(double number) {
	short magnitude;
	number = TMath::Abs(number);
	if (number >= 1)
		for (magnitude = 0; number / pow(10, magnitude) >= 10; magnitude++);
	else
		for (magnitude = -1; number / pow(10, magnitude) <= 1; magnitude--);
	return magnitude;
}

double utils::roundTo(const double number, const double roundTo) {
	double remainder = number - static_cast<int>(number / roundTo) * roundTo;
	if ( remainder / pow(10, magnitude(remainder)) < 0.5)
		return floor(number / roundTo) * roundTo;
	else
		return floor(number / roundTo + 1) * roundTo;
}
