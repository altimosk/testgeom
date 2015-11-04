#include <wx/wx.h>
#include "gengraph.h"
#include "shapes.h"
#include <vector>
#include <algorithm>
#include <cmath>

wxColor MakeColor(int color)
{
	unsigned char red, green, blue;

	switch (color)
	{
	case kBlack:
		red = 0; green = 0; blue = 0;
		break;

	case kBlue:
		red = 100; green = 149; blue = 237;
		break;

	case kLtGray:
		red = 180; green = 180; blue = 180;
		break;

	case kOrange:
		red = 255; green = 165; blue = 0;
		break;

	case kRed:
		red = 255; green = 0; blue = 0;
		break;

	case kMagenta:
		red = 255; green = 0; blue = 255;
		break;

	case kMaroon:
		red = 176; green = 48; blue = 96;
		break;

	case kForest:
		red = 34; green = 139; blue = 34;
		break;

	case kSkyBlue:
		red = 135; green = 206; blue = 235;
		break;

	case kGold:
		red = 255; green = 215; blue = 0;
		break;

	case kBrown:
		red = 165; green = 42; blue = 42;
		break;

	case kPlum:
		red = 221; green = 160; blue = 221;
		break;

	case kGreen:
		red = 0; green = 255; blue = 0;
		break;

	case kYellow:
		red = 255; green = 255; blue = 0;
		break;

	case kPink:
		red = 255; green = 192; blue = 203;
		break;

	case kWhite: 
	case kInvalidColor:
		red = 255; green = 255; blue = 255;
	default:
		return wxColor(color);
		break;
	}

	return wxColor(red, green, blue);
}

void ggPoint::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), 0, wxSOLID));
	dc.DrawCircle(x, y, 1);
}
void ggCircle::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), 1, wxSOLID));
	dc.DrawCircle(x, y, r);
}

void ggSegm::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), w, wxSOLID));
	dc.DrawLine(x1, y1, x2, y2 );
}

void ggArc::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), w, wxSOLID));
	dc.DrawArc(x1, y1, x2, y2, cx, cy);
}

class wxGenGraph : public cGenericGraphics
{
public:
	wxGenGraph(wxDC *draw, std::vector<ggShape*>* store) : shapes(store), dc(draw) {
	}

	void DeleteSavedGraphics(int segment)
	{
		if (shapes)
		{
			int shift = 0;
			for (size_t i = 0; i < shapes->size(); ++i)
			{
				ggShape* s = (*shapes)[i];
				if (s->Color() == segment)
				{
					s->Release();
					++shift;
				}
				else if (shift)
				{
					(*shapes)[i - shift] = s;
				}
			}
			shapes->resize(shapes->size() - shift);
		}
	}

	void DeleteSavedGraphics() 
	{
		if (shapes)
		{
			for (auto s : *shapes)
				s->Release();
			shapes->clear();
		}
	}


	void DrawLine(int x1, int y1, int x2, int y2, int segment,
			int w, bool f, bool t)
	{
		ggSegm s(x1, y1, x2, y2, w, segment);
		if( dc )
			s.Draw(*dc);
		if (!t && shapes)
			shapes->push_back(new ggSegm(s));
	}

	void DrawArc(int x1, int y1, int x2, int y2, int radius, // > 0 clockwise
		int cx, int cy, int segment,
		int width, bool, bool throw_away)
	{
		if (radius > 0)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		else
			radius = -radius;

		double d = hypot(x1 - cx, y1 - cy) / radius;
		x1 = (int)(cx + (x1 - cx) / d);
		y1 = (int)(cy + (y1 - cy) / d);
		ggArc a(x1, y1, x2, y2, cx, cy, width, segment);

		if (dc)
			a.Draw(*dc);
		if (!throw_away && shapes)
			shapes->push_back(new ggArc(a));


	}
	void DrawCircle(int x, int y, int diameter, int segment,
		bool f = fill, bool t = throwAway)
	{
		ggCircle c(x, y, (1+diameter)/2, segment);
		if (dc)
			c.Draw(*dc);
		if (!t && shapes)
			shapes->push_back(new ggCircle(c));
	}


	/*----------------------------------------------------------------------+
	|                                                                       |
	|   Redraw -- redraw the entire contents of the window.  This does      |
	|             not redraw any unsaved graphics that were previously      |
	|             drawn.  This routine is provided for your convenience     |
	|             while inside a debugger.  This routine will discard any   |
	|             pending keyin and mouse events.                           |
	|                                                                       |
	+----------------------------------------------------------------------*/
	void Redraw()
	{
		if (dc && shapes )
		{
			dc->Clear();
			for (auto p : *shapes)
				p->Draw(*dc);
		}
	}
	// changing the defaults for drawing calls like DrawLine (return previous value)
	virtual bool SetThrowAway(bool t) 
	{
		std::swap(t, throwAway);
		return t;
	}
	virtual int SetWidth(int w)
	{
		std::swap(w, width);
		return w;
	}
	virtual bool SetFill(bool f)
	{
		std::swap(f, fill);
		return f;
	}
private:
	std::vector<ggShape*>* shapes;
	wxDC *dc;
	static bool throwAway;
	static int width;
	static bool fill;
};

bool wxGenGraph::throwAway = true;
int wxGenGraph::width = 0;
bool wxGenGraph::fill = false;

class wxGenGraph* SetUpGenericGraphics(wxDC *draw, std::vector<ggShape*>* store)
{
	wxGenGraph* gg = new wxGenGraph(draw, store);
	SetGenericGraphics(gg);
	return gg;
}

void UnsetGenericGraphics(class wxGenGraph* gg = 0)
{
	SetGenericGraphics(0);
	delete gg;
}
cGenericGraphics* GenericGraphics = 0;
extern "C"
__declspec(dllexport) void SetGenericGraphics(cGenericGraphics *gg) {
	GenericGraphics = gg; }

