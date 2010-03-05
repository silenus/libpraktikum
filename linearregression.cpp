#include "linearregression.h"

LinearRegression::LinearRegression(const double *_x, const double *_y, const double *_xErrors, const double *_yErrors, const unsigned int _length) : x(_x), y(_y), xErrors(_xErrors), yErrors(_yErrors), length(_length) {
	if (x == NULL || y == NULL) {
		std::cerr << "Error: x and y must not be NULL" << std::endl;
		return;
	}
	canvas = new TCanvas("canvas", "Linear Regression");
	linearPad = new TPad("linear", "Linear Regression", 0, 0.5, 1, 1);
	linearPad->SetGrid();
	linearPad->Draw();
	linearPad->cd();

	// Create the graph of the datapoints
	linearGraph = new TGraphErrors(length, x, y, xErrors, yErrors);
	linearGraph->SetName("linearGraph");
	linearGraph->SetTitle("Linear Regression");
	linearGraph->SetMarkerStyle(MARKERSTYLE);
	// Fit a linear function to the graph with the minuit fitter, so the errors in x are taken into account.
	linearGraph->Fit("pol1", "F");
	linearGraph->Draw("APL");
	linearFunction = linearGraph->GetFunction("pol1");
	linearFunction->SetLineWidth(2);
	linearFunction->SetLineColor(kBlue);

	// Display a statistic box with the fit parameters
	linearStatistics = new TPaveText(0.7, 0.7, 1, 1, "NDC" );
	char buf[64];
	snprintf(buf, sizeof(buf), "m = %s",
			utils::printNumber(linearFunction->GetParameter(1), linearFunction->GetParError(1)).c_str());
	linearStatistics->AddText(buf);
	snprintf(buf, sizeof(buf), "n = %s",
			utils::printNumber(linearFunction->GetParameter(0), linearFunction->GetParError(0)).c_str());
	linearStatistics->AddText(buf);
	sprintf(buf, "#frac{#chi^{2}}{ndf} = %.1g",
			linearFunction->GetChisquare() / linearFunction->GetNDF());
	linearStatistics->AddText(buf);
	linearStatistics->Draw();

	// Compute the residuals and the pulls
	double *residualsArray = new double[length];
	double *pullsArray = new double[length];
	for (unsigned int i = 0; i < length; i++) {
		residualsArray[i] = y[i] - linearFunction->Eval(x[i]);
		pullsArray[i] = (y[i] - linearFunction->Eval(x[i])) / yErrors[i];
	}

	// Draw the residuals
	canvas->cd();
	residualsPad = new TPad("residualsPad", "Residuals", 0, 0, 0.5, 0.5);
	residualsPad->SetGrid();
	residualsPad->Draw();
	residualsVisible = true;
	residualsPad->cd();
	residualsGraph = new TGraphErrors(length, x, residualsArray, NULL, yErrors);
	residualsGraph->SetName("residualsGraph");
	residualsGraph->SetTitle("Residuals");
	residualsGraph->Draw("AP");
	// Make the zero line at the residuals graph more apparent
	TF1 *zero = new TF1("zero", "0");
	zero->SetLineColor(kBlue);
	zero->Draw("SAME");

	// Draw the pull distribution
	canvas->cd();
	pullsPad = new TPad("pullsPad", "Pulls", 0.5, 0, 1, 0.5);
	pullsPad->SetGrid();
	pullsPad->Draw();
	pullsVisible = true;
	pullsPad->cd();
	pullsHisto = new TH1F("Pulls", "Pull Distribution", 50, -5, 5);
	pullsHisto->FillN(length, pullsArray, NULL);
	pullsHisto->GetXaxis()->SetTitle("#sigma");
	pullsHisto->GetYaxis()->SetTitle("n");
	pullsHisto->Draw();
}

LinearRegression::LinearRegression(const double *_x, const double *_y, const unsigned int _length) : x(_x), y(_y), length(_length) {
	canvas = new TCanvas("canvas", "Linear Regression");
	linearPad = new TPad("linear", "Linear Regression", 0.5, 0, 1, 1);
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

void LinearRegression::hideResiduals() {
	if (pullsVisible)
		pullsPad->SetPad(0, 0, 1, 0.5);
	else
		linearPad->SetPad(0, 0, 1, 1);
	// Remove the reference to the residualsPad from the canvas, so it isn't shown
	canvas->GetListOfPrimitives()->Remove(residualsPad);
	canvas->Update();
	residualsVisible = false;
}

void LinearRegression::showResiduals() {
	if (pullsVisible) {
		pullsPad->SetPad(0.5, 0, 1, 0.5);
		residualsPad->SetPad(0, 0, 0.5, 0.5);
	} else {
		linearPad->SetPad(0, 0.5, 1, 1);
		residualsPad->SetPad(0, 0, 1, 0.5);
	}
	residualsPad->Draw();
	residualsVisible = true;
}

void LinearRegression::showPulls() {
	if (residualsVisible) {
		residualsPad->SetPad(0, 0, 0.5, 0.5);
		pullsPad->SetPad(0.5, 0, 1, 0.5);
	} else {
		linearPad->SetPad(0, 0.5, 1, 1);
		pullsPad->SetPad(0, 0, 1, 0.5);
	}
	pullsPad->Draw();
	residualsVisible = true;
}

void LinearRegression::hidePulls() {
	if (residualsVisible)
		residualsPad->SetPad(0, 0, 1, 0.5);
	else
		linearPad->SetPad(0, 0, 1, 1);
	// Remove the reference to the pullsPad from the canvas, so it isn't shown
	canvas->GetListOfPrimitives()->Remove(pullsPad);
	canvas->Update();
	pullsVisible = false;
}
