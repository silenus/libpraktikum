#include "options.h"
#include "utils.h"

#include <iostream>
#include <stdio.h>
#include <TStyle.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1F.h>
#include <TPaveText.h>
#include <TList.h>


class LinearRegression {
	public:

		LinearRegression(const double *_x, const double *_y, const double *_xErrors, const double *_yErrors, const unsigned int _length);

		LinearRegression(const double *_x, const double *_y, const unsigned int _length);

		void hideResiduals();

		void showResiduals();

		void hidePulls();

		void showPulls();

		inline TCanvas *getCanvas() const {
			return canvas;
		}

		inline TPad *getLinearPad() const {
			return linearPad;
		}

		inline TPad *getResidualsPad() const {
			return residualsPad;
		}

		inline TPad *getPullsPad() const {
			return pullsPad;
		}

		inline TF1 *getLinearFunction() const {
			return linearFunction;
		}

		inline TGraph *getLinearGraph() const {
			return linearGraph;
		}

		inline TGraphErrors *getResidualsGraph() const {
			return residualsGraph;
		}

		inline TH1F *getPullsHisto() const {
			return pullsHisto;
		}

		inline TPaveText *getLinearStats() const {
			return linearStatistics;
		}


	protected:
		const double *x;
		const double *y;
		const double *xErrors;
		const double *yErrors;
		const unsigned int length;

		TCanvas *canvas;
		TPad *linearPad;
		TPad *residualsPad;
		TPad *pullsPad;

		TF1 *linearFunction;
		TGraph *linearGraph;
		TPaveText *linearStatistics;
		TGraphErrors *residualsGraph;
		TH1F *pullsHisto;

		bool residualsVisible;
		bool pullsVisible;
};


