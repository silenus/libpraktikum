#include "linearregression.h"

LinearRegression::LinearRegression(const double *_x, const double *_y, const double *_xErrors, const double *_yErrors, const unsigned int _length) : x(_x), y(_y), xErrors(_xErrors), yErrors(_yErrors), length(_length) {
	canvas = new TCanvas("canvas", "Linear Regression");
	linearPad = new TPad("linear", "Linear Regression", 0, 0.5, 1, 1);
	linearPad->SetGrid();
	linearPad->cd();

	// Create the graph of the datapoints
	linearGraph = new TGraphErrors(length, x, y, xErrors, yErrors);
	linearGraph->SetName("linearGraph");
	linearGraph->SetTitle("Linear Regression");
	// Fit a linear function to the graph with the minuit fitter, so the errors in x are taken into account.
	linearGraph->Fit("pol1", "F");
	linearFunction = linearGraph->GetFunction("pol1");

	// Compute the residuals and the pulls
	double *residualsArray = new double[length];
	double *pullsArray = new double[length];
	for (unsigned int i = 0; i < length; i++) {
		residualsArray[i] = y[i] - linearFunction->Eval(x[i]);
		pullsArray[i] = (y[i] - linearFunction->Eval(x[i])) / yErrors[i];
	}

	// Draw the residuals
	canvas->cd();
	residualsPad = new TPad("residuals", "Residuals", 0, 0, 1, 0.5);
	residualsPad->cd();
	residualsGraph = new TGraphErrors(length, x, residualsArray, NULL, yErrors);
	residualsGraph->SetName("residualsGraph");
	residualsGraph->SetTitle("Residuals");

	// Draw the pull distribution
	canvas->cd();
	pullsPad = new TPad("pad3", "Pad3", 0.5, 0, 1, 0.5);
	pullsPad->cd();
	pullsHisto = new TH1F("pullHisto", "Pull Distribution", 50, -5, 5);
	pullsHisto->FillN(length, pullsArray, NULL);
	pullsHisto->GetXaxis()->SetTitle("#sigma");
	pullsHisto->GetYaxis()->SetTitle("n");
}

LinearRegression::LinearRegression(const double *_x, const double *_y, const unsigned int _length) : x(_x), y(_y), length(_length) {
	canvas = new TCanvas("canvas", "Linear Regression");
	linearPad = new TPad("linear", "Linear Regression", 0, 1, 1, 1);
	linearPad->SetGrid();
	linearPad->cd();

	// Create the graph of the datapoints
	TGraphErrors *linearGraph = new TGraphErrors(length, x, y, xErrors, yErrors);
	linearGraph->SetName("linearGraph");
	linearGraph->SetTitle("Linear Regression");
	// Fit a linear function to the graph
	linearGraph->Fit("pol1");
	linearFunction = linearGraph->GetFunction("pol1");

	residualsPad = NULL;
	residualsGraph = NULL;
	pullsPad = NULL;
	pullsHisto = NULL;
}
