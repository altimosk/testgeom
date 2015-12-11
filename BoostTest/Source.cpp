
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "..\gengraph.h"
GenericGraphics* GG = 0;
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
BOOST_AUTO_TEST_SUITE(Drawing)

BOOST_AUTO_TEST_CASE(DrawAll)
/* Compare with void free_test_function() */
{
	std::cout << "here comes DrawAll" << std::endl;
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

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(test_suite1)

BOOST_AUTO_TEST_CASE(test_case1)
{
	std::cout << "here comes test_case1" << std::endl;
	BOOST_TEST_WARN(sizeof(int) < 4U);
}

BOOST_AUTO_TEST_CASE(test_case2)
{
	std::cout << "here comes test_case2" << std::endl;
	BOOST_TEST_REQUIRE(1 == 2);
	BOOST_FAIL("Should never reach this line");
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE(test_suite2)

BOOST_AUTO_TEST_CASE(test_case3)
{
	BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(test_case4)
{
	BOOST_TEST(false);
}

BOOST_AUTO_TEST_SUITE_END()

extern class TriGraph* CreateTriGraph(int nx, int ny);
void DrawTriGraph(TriGraph* t);
extern void TriGraphMaxFlowEdmondsKarp(TriGraph* t, int sx, int sy, int tx, int ty, int w);
extern void TriGraphMaxFlowPushRelabel(TriGraph* t, int sx, int sy, int tx, int ty, int w);
extern void TriGraphMaxFlowBoykovKolmogorov(TriGraph* t, int sx, int sy, int tx, int ty, int w);
extern void TriGraphMinCostMaxFlow(TriGraph* t, int sx, int sy, int tx, int ty, int w);
extern void ReleaseTriGraph(TriGraph* t);
extern int dimacs();

BOOST_AUTO_TEST_SUITE(Graph)
BOOST_AUTO_TEST_CASE(CreateDraw)
{
	TriGraph* g = ::CreateTriGraph(20, 20);
	::DrawTriGraph(g);
	::ReleaseTriGraph(g);
}

BOOST_AUTO_TEST_CASE(EdmondsKarp)
{
	TriGraph* g = ::CreateTriGraph(240, 240);
	::TriGraphMaxFlowEdmondsKarp(g, 34, 35, 203, 206, 8);
	::ReleaseTriGraph(g);
}

BOOST_AUTO_TEST_CASE(PushRelabel)
{
	TriGraph* g = ::CreateTriGraph(240, 240);
	::TriGraphMaxFlowPushRelabel(g, 34, 35, 203, 206, 8);
	::ReleaseTriGraph(g);
}

BOOST_AUTO_TEST_CASE(BoykovKolmogorov)
{
	TriGraph* g = ::CreateTriGraph(240, 240);
	::TriGraphMaxFlowBoykovKolmogorov(g, 34, 35, 203, 206, 8);
	::ReleaseTriGraph(g);
}

BOOST_AUTO_TEST_CASE(MinCostMaxFlow)
{
	TriGraph* g = ::CreateTriGraph(240, 240);
	::TriGraphMinCostMaxFlow(g, 34, 35, 203, 206, 8);
	::ReleaseTriGraph(g);
}
BOOST_AUTO_TEST_CASE(Dimacs)
{
	dimacs();
}

BOOST_AUTO_TEST_SUITE_END()
