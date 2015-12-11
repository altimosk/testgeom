#include <wx/wx.h>
#include "..\gengraph.h"
#include "cliwrap.h"
#include "treedlg.h"
#include "unittree.h"


extern GenericGraphics* SetUpGenericGraphics(wxDC *dc);
extern void UnsetGenericGraphics(GenericGraphics* gg);
extern wxDC* ReplaceDC(GenericGraphics* gg, wxDC *dc);

class Canvas : public wxFrame
{
public:
	Canvas(const wxString& title);
	~Canvas();
private:
	void DrawPoint(wxMouseEvent &);
	void StorePoint(wxMouseEvent &);
	void PanZoom(wxMouseEvent &);
	void OnMouseMove(wxMouseEvent &);
	void OnPaint(wxPaintEvent&);

	void ProcessMiddleDrag(wxMouseEvent &, int& oldX, int& oldY);
	void ProcessMouseWheel(wxMouseEvent &, int& oldX, int& oldY);

	void InitClient(wxCommandEvent&);
	void RunUnitTests(wxCommandEvent&);
	void DeInitClient();
	void SelectTest(wxCommandEvent&);

	GenericGraphics* gg;
	wxClientDC* dc;
	ClientWrap cw;
	unsigned long testId;

	// a window to redirect std::cout to
	wxDialog coutDlg;
	wxTextCtrl coutTxt;
	wxStreamToTextRedirector redirect;

};

Canvas::Canvas(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(280, 180)), gg(0), dc(0), testId(0xffffffff),
coutDlg(0, -1, "Output", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
coutTxt(&coutDlg, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE), redirect(&coutTxt)
{
	coutDlg.Show();

	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Canvas::DrawPoint));
	Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(Canvas::StorePoint));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Canvas::PanZoom));
	Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Canvas::PanZoom));
	Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(Canvas::PanZoom));
	Connect(wxEVT_MOTION, wxMouseEventHandler(Canvas::OnMouseMove));
	Connect(wxEVT_PAINT, wxPaintEventHandler(Canvas::OnPaint));

	wxMenuBar* menubar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(1, wxT("&Init Client"));
	file->Append(2, wxT("&Select Test"));
	file->Append(3, wxT("&Run Test(s)"));
	menubar->Append(file, wxT("&Tests"));
	SetMenuBar(menubar);

	Connect(1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::InitClient));
	Connect(2, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::SelectTest));
	Connect(3, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Canvas::RunUnitTests));

	CreateStatusBar();
}

Canvas::~Canvas()
{
	DeInitClient();
}

void Canvas::InitClient(wxCommandEvent&)
{
	DeInitClient();
	dc = new wxClientDC(this);
	dc->SetBackground(*wxWHITE_BRUSH);
	wxAffineMatrix2D m;
	m.Scale(1, -1);
	dc->SetTransformMatrix(m);
	gg = SetUpGenericGraphics(dc);
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

static char const *EventType(wxMouseEvent &ev)
{
	if( ev.GetEventType() == wxEVT_LEFT_DOWN ) return "EVT_LEFT_DOWN";
	if( ev.GetEventType() == wxEVT_LEFT_UP ) return "EVT_LEFT_UP";
	if( ev.GetEventType() == wxEVT_MIDDLE_DOWN ) return "EVT_MIDDLE_DOWN";
	if( ev.GetEventType() == wxEVT_MIDDLE_UP ) return "EVT_MIDDLE_UP";
	if( ev.GetEventType() == wxEVT_RIGHT_DOWN ) return "EVT_RIGHT_DOWN";
	if( ev.GetEventType() == wxEVT_RIGHT_UP ) return "EVT_RIGHT_UP";
	if( ev.GetEventType() == wxEVT_MOTION ) return "EVT_MOTION";
	if( ev.GetEventType() == wxEVT_ENTER_WINDOW ) return "EVT_ENTER_WINDOW";
	if( ev.GetEventType() == wxEVT_LEAVE_WINDOW ) return "EVT_LEAVE_WINDOW";
	if( ev.GetEventType() == wxEVT_LEFT_DCLICK ) return "EVT_LEFT_DCLICK";
	if( ev.GetEventType() == wxEVT_MIDDLE_DCLICK ) return "EVT_MIDDLE_DCLICK";
	if( ev.GetEventType() == wxEVT_RIGHT_DCLICK ) return "EVT_RIGHT_DCLICK";
	if( ev.GetEventType() == wxEVT_SET_FOCUS ) return "EVT_SET_FOCUS";
	if( ev.GetEventType() == wxEVT_KILL_FOCUS ) return "EVT_KILL_FOCUS";
	if( ev.GetEventType() == wxEVT_CHILD_FOCUS ) return "EVT_CHILD_FOCUS";
	if( ev.GetEventType() == wxEVT_MOUSEWHEEL ) return "EVT_MOUSEWHEEL";
	if( ev.GetEventType() == wxEVT_AUX1_DOWN ) return "EVT_AUX1_DOWN";
	if( ev.GetEventType() == wxEVT_AUX1_UP ) return "EVT_AUX1_UP";
	if( ev.GetEventType() == wxEVT_AUX1_DCLICK ) return "EVT_AUX1_DCLICK";
	if( ev.GetEventType() == wxEVT_AUX2_DOWN ) return "EVT_AUX2_DOWN";
	if( ev.GetEventType() == wxEVT_AUX2_UP ) return "EVT_AUX2_UP";
	if( ev.GetEventType() == wxEVT_AUX2_DCLICK ) return "EVT_AUX2_DCLICK";

	return 0;
}

static char const *MouseState(wxMouseEvent &ev)
{
	static char buf[8];
	buf[0] = ev.m_leftDown ? 'L' : 'l';
	buf[1] = ev.m_middleDown ? 'M' : 'm';
	buf[2] = ev.m_rightDown ? 'R' : 'r';
	buf[3] = ev.m_aux1Down ? 'A' : 'a';
	buf[4] = ev.m_aux2Down ? 'A' : 'a';
	buf[5] = 0;
	return buf;
}

void TrackMouse(wxMouseEvent &ev)
{
	std::cout << "Type: " << EventType(ev) << " State: " << MouseState(ev) << "  " << ev.m_x << ", " << ev.m_y << std::endl;
	std::cout << "Mouse Event: " << 
		" clicks: " << ev.m_clickCount <<
		" axis: " << ev.m_wheelAxis <<
		" rotation: " << ev.m_wheelRotation <<
		" delta: " << ev.m_wheelDelta <<
		" lines: " << ev.m_linesPerAction <<
		" culumns: " << ev.m_columnsPerAction << std::endl;
}

void Canvas::OnMouseMove(wxMouseEvent &ev)
{
	if (ev.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		PanZoom(ev);
}

void Canvas::PanZoom(wxMouseEvent &ev)
{
//	TrackMouse(ev);

	if (!gg)
		return;

	static int oldX, oldY;
	if (ev.GetEventType() == wxEVT_MIDDLE_DOWN)
	{
		oldX = ev.GetX();
		oldY = ev.GetY();
		return;
	}
	if (ev.GetEventType() == wxEVT_MIDDLE_UP)
	{
		gg->Redraw();
		return;
	}
	if (ev.GetEventType() == wxEVT_MOTION)
	{
		ProcessMiddleDrag(ev, oldX, oldY);
		return;
	}
	if (ev.GetEventType() == wxEVT_MOUSEWHEEL )
	{
		ProcessMouseWheel(ev, oldX, oldY);
		return;
	}
	
}

void Canvas::ProcessMiddleDrag(wxMouseEvent &ev, int& oldX, int& oldY)
{
	int newX = ev.GetX();
	int newY = ev.GetY();
	oldX -= newX;
	oldY -= newY;
	gg->Pan(-oldX, -oldY);
	oldX = newX;
	oldY = newY;
}

void Canvas::ProcessMouseWheel(wxMouseEvent &ev, int& oldX, int& oldY)
{
	double k = pow(1.5, ev.m_wheelRotation /120.);
	oldX = ev.GetX();
	oldY = ev.GetY();
	gg->Zoom(oldX, oldY, k);
	gg->Redraw();
}


void Canvas::RunUnitTests(wxCommandEvent&)
{
	std::ostream str(&coutTxt);
	::RunUnitTests(testId, str);
	std::getchar();
}

void Canvas::OnPaint(wxPaintEvent&)
{
	if (!gg)
		return;

	wxPaintDC pdc(this);
	pdc.SetBackground(*wxWHITE_BRUSH);
	pdc.Clear();
	
	pdc.SetTransformMatrix(dc->GetTransformMatrix());
	wxDC* oldc = ReplaceDC(gg, &pdc);
	gg->Redraw();
	ReplaceDC(gg, oldc);
}

void Canvas::SelectTest(wxCommandEvent&)
{
	UnitTreeNode* root = MakeCurrentUnitTree();
	UnitTreeNode* sel = static_cast<UnitTreeNode*>(SelectNode(root));
	if (sel)
	{
		testId = sel->id;
		std::string path;
		sel->Path(path);
		SetStatusText(path);
	}
	delete root;
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

