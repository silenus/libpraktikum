#include "oscillation.h"

void Oscillation::init() {
		oscillationPad = NULL;
		frequencyPad = NULL;

		oscillationFunction = NULL;
		expFunction = NULL;
		oscillationGraph = NULL;
		expStatistics = NULL;
		frequencyGraph = NULL;

		oscillationVisible = false;
		frequencyVisible = false;
		doFit = false;

		xFreq = NULL;
		yFreq = NULL;
		iterations = 10000;
}

void Oscillation::draw() {
	oscillationVisible = true;
	frequencyVisible = true;
	drawOscillation();
	drawFrequency();
}

void Oscillation::drawOscillation() {
	canvas->cd();
	if (frequencyVisible)
		oscillationPad = new TPad("oscillationPad", "Oscillation", 0, 0.5, 1, 1);
	else
		oscillationPad = new TPad("oscillationPad", "Oscillation", 0, 0, 1, 1);
	oscillationPad->SetGrid();
	oscillationPad->Draw();
	oscillationPad->cd();

	// Create a graph of the oscillation
	if (hasErrors)
		oscillationGraph = new TGraphErrors(length, x, y, xErrors, yErrors);
	else
		oscillationGraph = new TGraph(length, x, y);
	oscillationGraph->SetName("oscillationGraph");
	oscillationGraph->SetTitle("Oscillation");
	if (doFit) {
		//TODO do fit to a damped harmonic oscillator here
	}
	oscillationGraph->Draw("APC");
}

void Oscillation::drawFrequency() {
	canvas->cd();
	if (oscillationVisible)
		frequencyPad = new TPad("frequencyPad", "Frequency", 0, 0, 1, 0.5);
	else
		frequencyPad = new TPad("frequencyPad", "Frequency", 0, 0, 1, 1);
	frequencyPad->SetGrid();
	frequencyPad->Draw();
	frequencyPad->cd();
	runFFT();

	frequencyGraph = new TGraph(iterations, xFreq, yFreq);
	frequencyGraph->SetName("frequencyGraph");
	frequencyGraph->SetTitle("Frequency");
	frequencyGraph->Draw("APL");
}

void Oscillation::runFFT() {
	xFreq = new double[iterations];
	yFreq = new double[iterations];
	fourier(length, x, y, iterations, 800, 1600, xFreq, yFreq, false);
	/*
	TVirtualFFT *fft = TVirtualFFT::FFT(1, (Int_t*)(&length), "R2C M K");
	if (!fft) return;
	fft->SetPoints(y);
	fft->Transform();
	TH1 *hr = NULL;
	hr = TH1::TransformHisto(fft, hr, "MAG");
	hr->Draw();*/
	/*
	// Code copied from praktlib.h
	TH1::AddDirectory(kFALSE);
	Double_t xStart = x[0];
	Double_t xStop = x[length-1];
	TH1D *hfft = new TH1D("hfft", "hfft", length, xStart, xStop);
	for (unsigned int i=0; i<length; i++){
		hfft->SetBinContent(i+1, y[i]);
	}
	TH1 *hm =0;
	TVirtualFFT::SetTransform(0);
	hm = hfft->FFT(hm, "MAG");
	hm->SetStats(kFALSE);
	TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
	Double_t *im_full = new Double_t[length];
	fft->GetPointsComplex(yFreq, im_full);
	int maxPoint=0;
	for (unsigned int i=0;i<length/2;i++)
	{
		if (yFreq[i]>yFreq[maxPoint])
		{
			maxPoint=i;
		}
	}
	// Scale x
	for (unsigned int i=0;i<length;i++)
	{
		xFreq[i] = i/TMath::Abs((x[length-1]-x[0]));
	}
	*/
}

int Oscillation::fourier(int n_datasets, const double* data_t, const double* data_a, int n_f, double f_min, double f_max, double* f_out, double* amp_out, bool progress){

			//calculate distance between two values of frequency
			double df=((double)(f_max-f_min))/((double)(n_f-1));

			double sumsin=0;
			double sumcos=0;
			double omega=0;

			//calculate amplitudes for the requested values of frequency
			for(int i=0; i < n_f; i++){
				sumsin=0;
				sumcos=0;

				//calculate the currently used frequency
				f_out[i]=f_min+(i*df);

				//calculate omega once
				omega=f_out[i]*2*TMath::Pi();

				//Do the magic ...
				for(int j=0; j< n_datasets; j++)
				{
					sumcos +=data_a[j]*cos(omega*data_t[j]);
				}
				for(int j=0; j< n_datasets; j++)
				{
					sumsin +=data_a[j]*sin(omega*data_t[j]);
				}
				amp_out[i]= sqrt(pow(sumcos,2)+pow(sumsin,2));
				//... magic done

				//Debugoutput, calculations may take a while, so you can see the progress
				if((progress==true) && (i % 32 ==0)) 
					cout <<f_out[i] << " " << amp_out[i]<< endl;
			}

			return 0;
		};
