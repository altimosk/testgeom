#include "..\gengraph.h"
static GenericGraphics* GG = 0;
extern "C"
__declspec(dllexport) void SetGenericGraphics(GenericGraphics *gg) {
	GG = gg;
}


extern "C"
__declspec(dllexport) void DrawAll()
{
	if (!GG)
		return;

	GG->Center(0, 0);
	GG->DrawLine(10, 10, 200, 50, kBlue, 10, false, true);
	GG->DrawLine(10, 50, 200, 20, kRed, 20, false, false);
	GG->DrawArc(100, 150, 200, 200, 40, 100, 200, kGold, 5, true, false);
	GG->DrawArc(100, 150, 200, 200, -40, 100, 200, kMagenta, 5, false, true);

	GG->DrawRect(10, 300, 100, 400, kGreen, false, true);
	GG->DrawRect(110, 300, 210, 400, kOrange, true, false);
	
	GG->DrawCircle(100, 0, 40, kOrange, false, false);
}
