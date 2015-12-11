#include <wx/wx.h>
#include "..\gengraph.h"
#include "shapes.h"

wxColor MakeColor(int color)
{
	unsigned char red, green, blue;

	switch (color)
	{
	case kBlack:
		return *wxBLACK;

	case kBlue:
		return *wxBLUE;

	case kLtGray:
		return *wxLIGHT_GREY;

	case kOrange:
		red = 255; green = 165; blue = 0;
		break;

	case kRed:
		return *wxRED;

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
		return *wxGREEN;

	case kYellow:
		return *wxYELLOW;

	case kPink:
		red = 255; green = 192; blue = 203;
		break;

	case kWhite:
	case kInvalidColor:
		return *wxWHITE;
	default:
		return wxColor(color);
	}

	return wxColor(red, green, blue);
}

void ggPoint::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), 0, wxSOLID));
	dc.SetBrush(wxBrush(MakeColor(Color()), wxBRUSHSTYLE_TRANSPARENT));
	dc.DrawCircle(x, y, 1);
}
void ggCircle::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), 1, wxSOLID));
	dc.SetBrush(wxBrush(MakeColor(Color()), fill ? wxBRUSHSTYLE_SOLID : wxBRUSHSTYLE_TRANSPARENT));
	dc.DrawCircle(x, y, r);
}

void ggSegm::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), w, wxSOLID));
	dc.DrawLine(x1, y1, x2, y2);
}

void ggRect::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), 0, wxSOLID));
	dc.SetBrush(wxBrush(MakeColor(Color()), fill ? wxBRUSHSTYLE_SOLID : wxBRUSHSTYLE_TRANSPARENT));
	dc.DrawRectangle(x1, y1, x2-x1, y2-y1);
}

void ggArc::Draw(wxDC &dc)
{
	dc.SetPen(wxPen(MakeColor(Color()), w, wxSOLID));
	dc.SetBrush(wxBrush(MakeColor(Color()), fill ? wxBRUSHSTYLE_SOLID : wxBRUSHSTYLE_TRANSPARENT));
	dc.DrawArc(x1, y1, x2, y2, cx, cy);
}

