#include "dataanalysis.h"
#include "options.h"
#include "utils.h"

#include <iostream>
#include <stdio.h>
#include <TStyle.h>
#include <TPad.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1F.h>
#include <TPaveText.h>
#include <TList.h>
#include <TVirtualFFT.h>


class Oscillation : DataAnalysis {
	public:

		inline Oscillation(const double *_x, const double *_y, const double *_xErrors, const double *_yErrors, const unsigned int _length)
			: DataAnalysis(_x, _y, _xErrors, _yErrors, _length)
		{
			init();
		}

		inline Oscillation(const double *_x, const double *_y, const unsigned int _length)
			: DataAnalysis(_x, _y, _length)
		{
			init();
		}

		void runFFT();

		void draw();

		void drawOscillation();

		void drawFrequency();

		inline TPad *getOscillationPad() const {
			return oscillationPad;
		}

		inline TPad *getFrequencyPad() const {
			return frequencyPad;
		}


		inline TF1 *getExpFunction() const {
			return expFunction;
		}

		inline TGraph *getOscillationGraph() const {
			return oscillationGraph;
		}

		inline TGraph *getFrequencyGraph() const {
			return frequencyGraph;
		}

		inline TPaveText *getExpStats() const {
			return expStatistics;
		}

		inline void enableFitting() {
			doFit = true;
		}


	protected:
		TPad *oscillationPad;
		TPad *frequencyPad;

		TF1 *oscillationFunction;
		TF1 *expFunction;
		TGraph *oscillationGraph;
		TPaveText *expStatistics;
		TGraph *frequencyGraph;

		bool oscillationVisible;
		bool frequencyVisible;
		bool doFit;

		/// the x values of the frequency spectrum
		double *xFreq;
		/// the y values of the frequency spectrum, i.e. the amplitude
		double *yFreq;
		unsigned int iterations;

	private:
		// This is called by the constructors
		void init();

		/**
		 * \brief simple discrete fourier transformation
		 * 
		 * Fourier transformation as in the maple library of Anfängerpraktikum at RWTH \n
		 * \a f_out and \a amp_out have to be allocated with size>=\a n_f 
		 * 
		 * \param[in] n_datasets numbers of values in \a data_t and \a data_a
		 * \param[in] data_t time of each datapoint
		 * \param[in] data_a input value of each datapoint
		 * \param[in] n_f number of discrete frequencies to calculate amplitude for
		 * \param[in] f_min minimum frequency
		 * \param[in] f_max maximum frequency
		 * \param[out] f_out pointer to double, frequency value of each generated datapoint (array of \a n_f double values has to be allocated)
		 * \param[out] amp_out pointer to double, amplitude of each generated datapoint (array of \a n_f double values has to be allocated)
		 * \param[in] progress (optional) weather to show generated values or not (can be useful to monitor progress which can take a while) (default: true)
		 * \return allways 0
		 */
		int fourier(int n_datasets, const double* data_t, const double* data_a, int n_f, double f_min, double f_max, double* f_out, double* amp_out, bool progress);
};


