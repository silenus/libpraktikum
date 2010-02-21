#include "../data.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TApplication.h>

using namespace std;
int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "Usage: plot path [column1 column2]" << endl;
		return 0;
	}
	string filename(argv[1]);
	Data data(filename);

	int col1 = 1;
	int col2 = 2;
	if (argc > 2) {
		col1 = atoi(argv[2]);
		col2 = atoi(argv[3]);
	}
	if (col1 < 1 || col2 < 1) {
		cerr << "Error: Columns must be greater than 1" << endl;
		return 1;
	}

	double *x = data.getCol(col1);
	double *y = data.getCol(col2);

	TApplication app(argv[0], &argc, argv);
	TCanvas *c1 = new TCanvas();
	c1->SetGrid();

	TGraph *graph = new TGraph(data.length(), x, y);

	string title = "";
	if (data.hasHeader()) {
		title = data.getName(col1) + " [" + data.getUnit(col1) + "]";
		graph->GetXaxis()->SetTitle(title.c_str());
		title = data.getName(col2) + " [" + data.getUnit(col2) + "]";
		graph->GetYaxis()->SetTitle(title.c_str());
	}
	graph->SetMarkerStyle(21);
	graph->SetMarkerSize(0.5);
	if (title != "")
		graph->SetTitle(title.c_str());
	graph->Draw("APL");

	app.Run();
	return 0;
}

