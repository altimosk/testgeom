
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "gengraph.h"
static GenericGraphics* GG = 0;
extern "C"
__declspec(dllexport) void SetGenericGraphics(GenericGraphics *gg) {
	GG = gg;
}


// initialization function:
extern "C"
__declspec(dllexport) bool init_unit_test()
{
	return true;
}

// entry point:
int main(int argc, char* argv[])
{
	return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}

BOOST_AUTO_TEST_CASE(free_test_function)
/* Compare with void free_test_function() */
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

	BOOST_TEST(false /* test assertion */);
}

