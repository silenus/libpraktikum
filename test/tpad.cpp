#include <iostream>
#include <TPad.h>
#include <TPaveText.h>

int main() {
	TPad *pad = new TPad("pad", "pad", 0, 0, 1, 1);
	pad->cd();
	TPaveText *text = new TPaveText(0.7, 0.7, 1, 1, "NDC" );
	text->AddText("foo");
	return 0;
}
