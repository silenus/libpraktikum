#include "../data.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TApplication.h>

using namespace std;
int main(int argc, char* argv[]) {
	// Parse for options
	unsigned int xCol = 1;
	unsigned int yCol = 2;
	int c;

	opterr = 0;

	while ((c = getopt (argc, argv, "hx:y:")) != -1)
		switch (c) {
			case 'h':
				puts("Usage: plot [-x <column> -y <column>] FILES");
				return 0;
			case 'x':
				xCol = atoi(optarg);
				break;
			case 'y':
				yCol = atoi(optarg);
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);
				return 1;
			default:
				abort();
		}
	if (xCol < 1 || yCol < 1) {
		cerr << "Error: Columns must be greater than 1" << endl;
		return 1;
	}

	unsigned int length = argc - optind;
	TApplication app(argv[0], NULL, NULL);
	Data **data = new Data*[length];

	for (unsigned int i = 0; i < length; i++) {
		data[i] = new Data(argv[optind + i]);
		double *x = data[i]->getCol(xCol);
		double *y = data[i]->getCol(yCol);

		TCanvas *c1 = new TCanvas();
		c1->SetGrid();

		TGraph *graph = new TGraph(data[i]->length(), x, y);

		string title = "";
		if (data[i]->hasHeader()) {
			title = data[i]->getName(xCol) + " [" + data[i]->getUnit(xCol) + "]";
			graph->GetXaxis()->SetTitle(title.c_str());
			title = data[i]->getName(yCol) + " [" + data[i]->getUnit(yCol) + "]";
			graph->GetYaxis()->SetTitle(title.c_str());
		}
		graph->SetMarkerStyle(21);
		graph->SetMarkerSize(0.5);
		if (title != "")
			graph->SetTitle(title.c_str());
		graph->Draw("APL");
	}

	app.Run();
	return 0;
}

