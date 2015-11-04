#include <wx/wx.h>
#include "gengraph.h"
#include "shapes.h"
#include <vector>
#include <algorithm>
#include <cmath>

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
			int w, bool, bool t)
	{
		ggSegm s(x1, y1, x2, y2, w, segment);
		if( dc )
			s.Draw(*dc);
		if (!t && shapes)
			shapes->push_back(new ggSegm(s));
	}

	void DrawArc(int x1, int y1, int x2, int y2, int radius, // > 0 clockwise
		int cx, int cy, int segment,
		int w, bool f, bool t)
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
		ggArc a(x1, y1, x2, y2, cx, cy, w, f, segment);

		if (dc)
			a.Draw(*dc);
		if (!t && shapes)
			shapes->push_back(new ggArc(a));


	}
	void DrawCircle(int x, int y, int diameter, int segment,
		bool f, bool t)
	{
		ggCircle c(x, y, (1+diameter)/2, f, segment);
		if (dc)
			c.Draw(*dc);
		if (!t && shapes)
			shapes->push_back(new ggCircle(c));
	}

	void DrawRect(int xmin, int ymin, int xmax, int ymax, int segment,
		bool f, bool t)
	{
		ggRect r(xmin, ymin, xmax, ymax, f, segment);
		if (dc)
			r.Draw(*dc);
		if (!t && shapes)
			shapes->push_back(new ggRect(r));
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
	//virtual bool SetThrowAway(bool t) 
	//{
	//	std::swap(t, throwAway);
	//	return t;
	//}
	//virtual int SetWidth(int w)
	//{
	//	std::swap(w, width);
	//	return w;
	//}
	//virtual bool SetFill(bool f)
	//{
	//	std::swap(f, fill);
	//	return f;
	//}
private:
	std::vector<ggShape*>* shapes;
	wxDC *dc;
	//static bool throwAway;
	//static int width;
	//static bool fill;
};

//bool wxGenGraph::throwAway = true;
//int wxGenGraph::width = 0;
//bool wxGenGraph::fill = false;

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

