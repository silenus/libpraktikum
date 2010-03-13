#include <iostream>
#include <TCanvas.h>


class DataAnalysis {
	public:
		inline DataAnalysis(const double *_x, const double *_y, const double *_xErrors, const double *_yErrors, const unsigned int _length)
			: x(_x), y(_y), xErrors(_xErrors), yErrors(_yErrors), length(_length), hasErrors(true)
		{
			if (x == NULL || y == NULL) {
				std::cerr << "Error: x and y must not be NULL" << std::endl;
				return;
			}
			canvas = new TCanvas("canvas");
		}

		inline DataAnalysis(const double *_x, const double *_y, const unsigned int _length)
			: x(_x), y(_y), xErrors(NULL), yErrors(NULL), length(_length), hasErrors(false)
		{
			if (x == NULL || y == NULL) {
				std::cerr << "Error: x and y must not be NULL" << std::endl;
				return;
			}
			canvas = new TCanvas("canvas");
		}

		inline void draw() const {
			canvas->Draw();
		}

		inline TCanvas *getCanvas() const {
			return canvas;
		}

		inline bool getHasErrors() const {
			return hasErrors;
		}


	protected:
		const double *x;
		const double *y;
		const double *xErrors;
		const double *yErrors;
		const unsigned int length;
		const bool hasErrors;

		TCanvas *canvas;

};


