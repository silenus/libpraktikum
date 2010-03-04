#include <stdlib.h>
#include <string.h>
// ROOT includes
#include <TApplication.h>

#include "../linearregression.h"
#include "../data.h"

using namespace std;

int main(int argc, char **argv) {
	Data *data = new Data(argv[1]);
	TApplication app("Aufladen", &argc, argv);

	double *x = data->getCol(1);
	double *y = data->getCol(3);
	unsigned int n_datasets = data->length();

	//double *ye = libprakt::createArray(n_datasets, 20/ pow(2, 12));
	double *ye = new double[n_datasets];
	double binWidth = 20 / pow(2, 12);
	for (uint k = 0; k < n_datasets; k++)
		ye[k] = sqrt(2) * binWidth / (y[n_datasets-1] - y[k]);

	int n = 0;
	for (uint k = 0; k < n_datasets; k++) {
		if (y[n_datasets-1] - y[k] > 0.1) {
			y[k] = log(y[n_datasets-1] - y[k]);
			n++;
		}
	}

	LinearRegression *linear = new LinearRegression(x, y, NULL, ye, n);
	if (linear);
//}}}
/*
//{{{ Systematische Fehler
	double *y_plus = new double[n];
	double *y_minus = new double[n];
	for (int k = 0; k < n; k++)
	{
		y_plus[k] = y[k] + 0.01 * y[k] + 0.005 * 10;
		y_minus[k] = y[k] - 0.01 * y[k] - 0.005 * 10;
	}

	double a_plus, ae_plus, b_plus, be_plus, chiq_plus, cov_plus;
	libprakt::linearRegression(n, x, y_plus, ye, a_plus, ae_plus, b_plus, be_plus, chiq_plus, cov_plus);
	double a_minus, ae_minus, b_minus, be_minus, chiq_minus, cov_minus;
	libprakt::linearRegression(n, x, y_minus, ye, a_minus, ae_minus, b_minus, be_minus, chiq_minus, cov_minus);
	double a_sys = ( (a - a_plus) + (a_minus - a) ) / 2;
	c_sys_a[i] = 1e6 / (pow(a, 2) * r) * a_sys;
	c_sys_r[i] = -1e6 / (a * pow(r, 2)) * re_sys;
//}}}
	}
	double c_mean, c_err_i, c_err_o, c_sys_a_mean, c_sys_r_mean, chiq_mean;
	c_mean = libprakt::WeightedMean(c, ce, n_data, c_err_i, c_err_o);
	c_sys_a_mean = libprakt::Mean(c_sys_a, n_data);
	c_sys_r_mean = libprakt::Mean(c_sys_r, n_data);
	chiq_mean = libprakt::Mean(chiq, n_data);
	cout << "Mittlere Kapazität: " << c_mean << " +- " << c_err_i << endl;
	cout << "Systematischer Fehler (a): " << c_sys_a_mean << endl;
	cout << "Systematischer Fehler (R): " << c_sys_r_mean << endl;
	cout << "Mittleres chiq: " << chiq_mean << endl;
	cout << "outer error: " << c_err_o << endl;
//	cout << "Mittlere Kapazität: " << c_mean << " +- " << c_err_o << endl;
*/
	app.Run();
	return 0;
}

//vim: foldmethod=marker
