#include <wx/wx.h>
#include "gengraph.h"
#include "shapes.h"
#include <vector>
#include <algorithm>
#include <cmath>

// helper for Zoom and Center: change translation to place src at dest
void AdjustTranslate(wxAffineMatrix2D& m, wxPoint2DDouble src, wxPoint2DDouble dest)
{
	wxAffineMatrix2D inv(m);
	inv.Invert();
	wxPoint2DDouble t = inv.TransformPoint(dest) - src;
	m.Translate(t.m_x, t.m_y);
}

class wxGenGraph : public GenericGraphics
{
public:
	wxGenGraph(wxDC *draw) : dc(draw) {
		}
	~wxGenGraph()
	{
		for (auto s : shapes)
			s->Release();
	}

	void DeleteSavedGraphics(int segment)
	{
		int shift = 0;
		for (size_t i = 0; i < shapes.size(); ++i)
		{
			ggShape* s = shapes[i];
			if (s->Color() == segment)
			{
				s->Release();
				++shift;
			}
			else if (shift)
			{
				shapes[i - shift] = s;
			}
		}
		shapes.resize(shapes.size() - shift);
	}

	void DeleteSavedGraphics() 
	{
		for (auto s : shapes)
			s->Release();
		shapes.clear();
	}


	void DrawLine(int x1, int y1, int x2, int y2, int segment,
			int w, bool, bool t)
	{
		ggSegm s(x1, y1, x2, y2, w, segment);
		if( dc )
			s.Draw(*dc);
		if (!t)
			shapes.push_back(new ggSegm(s));
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
		if (!t)
			shapes.push_back(new ggArc(a));


	}
	void DrawCircle(int x, int y, int diameter, int segment,
		bool f, bool t)
	{
		ggCircle c(x, y, (1+diameter)/2, f, segment);
		if (dc)
			c.Draw(*dc);
		if (!t)
			shapes.push_back(new ggCircle(c));
	}

	void DrawRect(int xmin, int ymin, int xmax, int ymax, int segment,
		bool f, bool t)
	{
		ggRect r(xmin, ymin, xmax, ymax, f, segment);
		if (dc)
			r.Draw(*dc);
		if (!t)
			shapes.push_back(new ggRect(r));
	}

	// redraw non-throw-away shapes
	void Redraw()
	{
		if (dc)
		{
			wxAffineMatrix2D m = dc->GetTransformMatrix();
			dc->ResetTransformMatrix();
			dc->Clear();
			dc->SetTransformMatrix(m);

			for (auto p : shapes)
				p->Draw(*dc);
		}
	}

	// scale all saved and future drawings. x,y retains its position on screen
	virtual void Zoom(int x, int y, double k)
	{
		if (dc && k != 1)
		{
			wxAffineMatrix2D m = dc->GetTransformMatrix();
			wxPoint2DDouble src(x, y);
			wxPoint2DDouble dest = m.TransformPoint(src);
			m.Scale(k, k);
			AdjustTranslate(m, src, dest);
			dc->SetTransformMatrix(m);
		}
	}

	// make x,y appear in the center of the screen; scaling unchanged
	virtual void Center(int x, int y)
	{
		if (dc)
		{
			wxAffineMatrix2D m = dc->GetTransformMatrix();
			wxSize sz = dc->GetSize();
			AdjustTranslate(m, wxPoint2DDouble(x, y), wxPoint2DDouble(sz.GetWidth()/2, sz.GetHeight()/2));
			dc->SetTransformMatrix(m);
		}
	}

	wxDC *dc;

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
	std::vector<ggShape*> shapes;
	//static bool throwAway;
	//static int width;
	//static bool fill;
};

//bool wxGenGraph::throwAway = true;
//int wxGenGraph::width = 0;
//bool wxGenGraph::fill = false;


GenericGraphics* SetUpGenericGraphics(wxDC *draw)
{
	return new wxGenGraph(draw);
}

void UnsetGenericGraphics(GenericGraphics* gg = 0)
{
	delete gg;
}

wxDC* ReplaceDC(GenericGraphics* gg, wxDC *dc)
{
	wxGenGraph* wgg = static_cast<wxGenGraph*>(gg);
	std::swap(dc, wgg->dc);
	return dc;
}
