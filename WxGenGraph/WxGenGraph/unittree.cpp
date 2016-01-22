#include "unittree.h"

// Boost.Test
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include <boost/test/tree/visitor.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test_parameters.hpp>
#include <boost/cstdlib.hpp>            // for exit codes

#ifndef BOOST_TEST_IMPL_TRY
#define BOOST_TEST_IMPL_TRY BOOST_TEST_I_TRY
#endif

#ifndef BOOST_TEST_IMPL_CATCH
#define BOOST_TEST_IMPL_CATCH BOOST_TEST_I_CATCH
#endif

#ifndef BOOST_TEST_IMPL_CATCH0
#define BOOST_TEST_IMPL_CATCH0 BOOST_TEST_I_CATCH0
#endif

#ifndef BOOST_TEST_IMPL_CATCHALL
#define BOOST_TEST_IMPL_CATCHALL BOOST_TEST_I_CATCHALL
#endif

#ifndef BOOST_TEST_IMPL_RETHROW
#define BOOST_TEST_IMPL_RETHROW BOOST_TEST_I_RETHROW
#endif

UnitTreeNode* MakeUnitTreeNode(boost::unit_test::test_unit const& u)
{
	UnitTreeNode* n = new UnitTreeNode(TreeNode::strdup(u.full_name()));
	n->id = u.p_id;
	return n;
}

UnitTreeNode* AddChild(UnitTreeNode* p, boost::unit_test::test_unit const& u)
{
	UnitTreeNode* n = p->AddChild(TreeNode::strdup(u.p_name));
	n->id = u.p_id;
	return n;
}

class UTbuilder : public boost::unit_test::test_tree_visitor
{
public:
	UTbuilder() : root(0), cur(0), last() {}

	// test tree visitor interface
	virtual bool    visit(boost::unit_test::test_unit const& u) 
	{ 
		if (cur)
			last = AddChild(cur, u);
		else
			last = root = MakeUnitTreeNode(u);
		return true;
	}

	virtual void    visit(boost::unit_test::test_case const& tc) { 
		visit((boost::unit_test::test_unit const&)tc); }
	virtual bool    test_suite_start(boost::unit_test::test_suite const& ts) 
	{ 
		visit((boost::unit_test::test_unit const&)ts); 
		cur = last;
		return true;
	}
	virtual void    test_suite_finish(boost::unit_test::test_suite const&) 
	{
		cur = cur->Parent();
	}
	UnitTreeNode* Root() const {
		return root; }


private:
	UnitTreeNode* root, *cur, *last;
};

UnitTreeNode* MakeCurrentUnitTree()
{
	UTbuilder v;
	traverse_test_tree(boost::unit_test::framework::master_test_suite(), v, true);

	return v.Root();
}

using namespace ::boost::unit_test;

test_suite* dummy(int, char*[]) { 
	return 0; }

bool dummyAlt() {
	return true;
}

int RunUnitTests(unsigned long id, std::ostream& str)
{
	boost::unit_test::unit_test_log.set_stream(str);
	return RunUnitTests(id);
}
int RunUnitTests(unsigned long id)
{
	char* name = "qq";
#if 0
	::boost::unit_test::unit_test_main(&dummy, 1, &name);
#else

	int result_code = 0;

	BOOST_TEST_IMPL_TRY{
		framework::init(&dummyAlt, 1, &name);
	framework::finalize_setup_phase();
	framework::run(id);

	results_reporter::make_report();

	result_code = 
        //runtime_config::no_result_code()	? boost::exit_success.		: 
        results_collector.results(framework::master_test_suite().p_id).result_code();
	}
		BOOST_TEST_IMPL_CATCH0(framework::nothing_to_test) {
		result_code = boost::exit_success;
	}
	BOOST_TEST_IMPL_CATCH(framework::internal_error, ex) {
		results_reporter::get_stream() << "Boost.Test framework internal error: " << ex.what() << std::endl;

		result_code = boost::exit_exception_failure;
	}
	BOOST_TEST_IMPL_CATCH(framework::setup_error, ex) {
		results_reporter::get_stream() << "Test setup error: " << ex.what() << std::endl;

		result_code = boost::exit_exception_failure;
	}
	BOOST_TEST_IMPL_CATCHALL() {
		results_reporter::get_stream() << "Boost.Test framework internal error: unknown reason" << std::endl;

		result_code = boost::exit_exception_failure;
	}

	framework::shutdown();
#endif
	return result_code;
}
