#include "gengraph.h"
extern cGenericGraphics* GenericGraphics;

void DrawAll()
{
	GenericGraphics->DrawLine(10, 10, 200, 50, kBlue, 10, false, true);
	GenericGraphics->DrawLine(10, 50, 200, 20, kRed, 20, false, false);
	GenericGraphics->DrawArc(100, 150, 200, 200, 40, 100, 200, kGold, 5, false, false);
	GenericGraphics->DrawArc(100, 150, 200, 200, -40, 100, 200, kMagenta, 5, false, true);

	GenericGraphics->DrawLine(10, 300, 200, 300, kBlack, 0, false, true);
	GenericGraphics->DrawLine(10, 310, 200, 310, kBlack, 1, false, true);
	GenericGraphics->DrawLine(10, 320, 200, 320, kBlack, 2, false, true);
	GenericGraphics->DrawLine(10, 330, 200, 330, kBlack, 3, false, true);

}
