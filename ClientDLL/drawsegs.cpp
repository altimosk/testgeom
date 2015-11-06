#include "gengraph.h"
static cGenericGraphics* GenericGraphics = 0;
extern "C"
__declspec(dllexport) void SetGenericGraphics(cGenericGraphics *gg) {
	GenericGraphics = gg;
}


extern "C"
__declspec(dllexport) void DrawAll()
{
	if (!GenericGraphics)
		return;

	GenericGraphics->Center(0, 0);
	GenericGraphics->DrawLine(10, 10, 200, 50, kBlue, 10, false, true);
	GenericGraphics->DrawLine(10, 50, 200, 20, kRed, 20, false, false);
	GenericGraphics->DrawArc(100, 150, 200, 200, 40, 100, 200, kGold, 5, true, false);
	GenericGraphics->DrawArc(100, 150, 200, 200, -40, 100, 200, kMagenta, 5, false, true);

	GenericGraphics->DrawRect( 10, 300, 100, 400, kGreen, false, true);
	GenericGraphics->DrawRect(110, 300, 210, 400, kOrange, true, false);
	
	GenericGraphics->DrawCircle(100, 0, 40, kOrange, false, false);
}
