#include <wx/wx.h>
#include <utility>
#include <vector>
#include <algorithm>
#include "shapes.h"
#include "gengraph.h"
extern cGenericGraphics* GenericGraphics;
extern void DrawAll();

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

	std::vector<ggShape*> shapes;
	class wxGenGraph* gg;
	wxClientDC* dc;
};

Canvas::Canvas(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180))
{
	extern class wxGenGraph* SetUpGenericGraphics(wxDC *draw, std::vector<ggShape*>* store);
	dc = new wxClientDC(this);
	dc->SetBackground(*wxWHITE_BRUSH);
	wxAffineMatrix2D m;
	m.Scale(1, -1);
	dc->SetTransformMatrix(m);
	gg = SetUpGenericGraphics(dc, &shapes);

	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Canvas::DrawPoint));
	Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(Canvas::StorePoint));
	Connect(wxEVT_PAINT, wxPaintEventHandler(Canvas::OnPaint));

	wxMenuBar* menubar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(1, wxT("&Draw All"));
	menubar->Append(file, wxT("&Draw"));
	SetMenuBar(menubar);

	Connect(1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::DrawAll));
}
Canvas::~Canvas()
{
	extern void UnsetGenericGraphics(class wxGenGraph* gg);
	UnsetGenericGraphics(gg);
	delete dc;
	for (auto s : shapes)
		s->Release();
}

void Canvas::DrawPoint(wxMouseEvent & ev)
{
	GenericGraphics->DrawCircle(ev.m_x, ev.m_y, 1, kBlue, true, true);
}
void Canvas::StorePoint(wxMouseEvent & ev)
{
	GenericGraphics->DrawCircle(ev.m_x, ev.m_y, 1, kRed, true, false);
}

void Canvas::DrawAll(wxCommandEvent&)
{
	::DrawAll();
}

void Canvas::OnPaint(wxPaintEvent&)
{
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
