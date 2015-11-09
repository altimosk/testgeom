#include <wx/wx.h>
#include <utility>
#include <vector>
#include <algorithm>
#include "shapes.h"
#include "gengraph.h"
#include "cliwrap.h"

// Boost.Test
#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>


extern GenericGraphics* SetUpGenericGraphics(wxDC *draw, std::vector<ggShape*>* store);
extern void UnsetGenericGraphics(GenericGraphics* gg);

class Canvas : public wxFrame
{
public:
	Canvas(const wxString& title);
	~Canvas();
private:
	void DrawPoint(wxMouseEvent &);
	void StorePoint(wxMouseEvent &);
	void OnPaint(wxPaintEvent&);
	void DrawAll(wxCommandEvent&);
	void InitClient(wxCommandEvent&);
	void RunUnitTests(wxCommandEvent&);
	void DeInitClient();

	std::vector<ggShape*> shapes;
	GenericGraphics* gg;
	wxClientDC* dc;
	ClientWrap cw;
};

Canvas::Canvas(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180)), gg(0), dc(0)
{
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Canvas::DrawPoint));
	Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(Canvas::StorePoint));
	Connect(wxEVT_PAINT, wxPaintEventHandler(Canvas::OnPaint));

	wxMenuBar* menubar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(1, wxT("&Init Client"));
	file->Append(2, wxT("&Draw All"));
	file->Append(3, wxT("&Run Tests"));
	menubar->Append(file, wxT("&Draw"));
	SetMenuBar(menubar);

	Connect(1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::InitClient));
	Connect(2, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::DrawAll));
	Connect(3, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::RunUnitTests));
}
Canvas::~Canvas()
{
	DeInitClient();
	for (auto s : shapes)
		s->Release();
}

void Canvas::InitClient(wxCommandEvent&)
{
	DeInitClient();
	dc = new wxClientDC(this);
	dc->SetBackground(*wxWHITE_BRUSH);
	wxAffineMatrix2D m;
	m.Scale(1, -1);
	dc->SetTransformMatrix(m);
	gg = SetUpGenericGraphics(dc, &shapes);
	if (!gg)
	{
		delete dc;
		dc = 0;
	}
	else
	{
		cw.ReInit(gg);
	}

	Refresh();
}

void  Canvas::DeInitClient()
{
	UnsetGenericGraphics(gg);
	delete dc;
	gg = 0;
	dc = 0;
	cw.Unwrap();
}

void Canvas::DrawPoint(wxMouseEvent & ev)
{
	if (!gg)
		return;

	wxAffineMatrix2D m = dc->GetTransformMatrix();
	m.Invert();
	wxPoint2DDouble p = m.TransformPoint(wxPoint2DDouble(ev.m_x, ev.m_y));
	gg->DrawCircle(p.m_x, p.m_y, 1, kBlue, true, true);
}
void Canvas::StorePoint(wxMouseEvent & ev)
{
	if (!gg)
		return;
	wxAffineMatrix2D m = dc->GetTransformMatrix();
	m.Invert();
	wxPoint2DDouble p = m.TransformPoint(wxPoint2DDouble(ev.m_x, ev.m_y));
	gg->DrawCircle(p.m_x, p.m_y, 1, kRed, true, false);
}

void Canvas::DrawAll(wxCommandEvent&)
{
	cw.DrawAll();
}

bool dummy() 
{
	return true;
}
void Canvas::RunUnitTests(wxCommandEvent&)
{
	char* name = "qq";

	::boost::unit_test::unit_test_main(&dummy, 1, &name);
//	::boost::unit_test::framework::clear();
}
void Canvas::OnPaint(wxPaintEvent&)
{
	if (!gg)
		return;

	wxPaintDC pdc(this);
	pdc.SetBackground(*wxWHITE_BRUSH);
	pdc.Clear();
	
	pdc.SetTransformMatrix(dc->GetTransformMatrix());
	for (auto s : shapes)
		s->Draw(pdc);
}

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	(new Canvas(wxT("Canvas")))->Show(true);
	return true;
}

