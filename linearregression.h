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
#include <TLatex.h>


class LinearRegression {
	public:

		LinearRegression(const double *_x, const double *_y, const double *_xErrors, const double *_yErrors, const unsigned int _length, string name = "LinearRegression");

//		LinearRegression(const double *_x, const double *_y, const unsigned int _length);

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

		inline TGraph *getResidualsGraph() const {
			return residualsGraph;
		}

		inline TH1F *getPullsHisto() const {
			return pullsHisto;
		}

		inline bool getHasErrors() const {
			return hasErrors;
		}

		inline TPaveText *getLinearStats() const {
			return linearStatistics;
		}

		inline void setXAxis(const string &title) {
			linearGraph->GetXaxis()->SetTitle(title.c_str());
			residualsGraph->GetXaxis()->SetTitle(title.c_str());
		}

		inline void setYAxis(const string &title) {
			linearGraph->GetYaxis()->SetTitle(title.c_str());
			residualsGraph->GetYaxis()->SetTitle(title.c_str());
		}

		/** Set the units in the statistics box
 		 * \param[in] n y-intercept
		 * \param[in] m slope
		 */
		void setUnits(const string &n, const string &m);



	protected:
		TPaveText *drawStats() const;

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
		TGraph *residualsGraph;
		TH1F *pullsHisto;

		bool hasErrors;
		bool residualsVisible;
		bool pullsVisible;
};


