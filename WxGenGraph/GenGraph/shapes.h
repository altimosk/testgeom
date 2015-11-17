#ifndef SHAPES_H
#define SHAPES_H

class wxDC;

class ggShape
{
public:
	virtual void Draw(wxDC &) = 0;
	virtual void Release() {
		delete this; }
	ggShape(int c) : color(c) {
	}
	int Color() const {
		return color; }
private:
	int color;
};

class ggPoint : public ggShape
{
public:
	virtual void Draw(wxDC &);
	ggPoint(int xx, int yy, int c) : ggShape(c), x(xx), y(yy) {
	}
private:
	int x, y;
};

class ggCircle : public ggShape
{
public:
	virtual void Draw(wxDC &);
	ggCircle(int xx, int yy, int rr, bool f, int c) : ggShape(c), x(xx), y(yy), r(rr), fill(f) {
	}
private:
	int x, y, r;
	bool fill;
};

class ggSegm : public ggShape
{
public:
	virtual void Draw(wxDC &);
	ggSegm(int xx1, int yy1, int xx2, int yy2, int ww, int c) : ggShape(c), x1(xx1), y1(yy1), x2(xx2), y2(yy2), w(ww) {
	}
private:
	int x1, y1, x2, y2, w;
};

class ggRect : public ggShape
{
public:
	virtual void Draw(wxDC &);
	ggRect(int xx1, int yy1, int xx2, int yy2, bool f, int c) : ggShape(c), x1(xx1), y1(yy1), x2(xx2), y2(yy2), fill(f) {
	}
private:
	int x1, y1, x2, y2;
	bool fill;
};

class ggArc : public ggShape
{
public:
	virtual void Draw(wxDC &);
	ggArc(int xx1, int yy1, int xx2, int yy2, int cxx, int cyy, int ww, bool f, int c) : ggShape(c), x1(xx1), y1(yy1), x2(xx2), y2(yy2), cx(cxx), cy(cyy), w(ww), fill(f) {
	}
private:
	int x1, y1, x2, y2, cx, cy, w;
	bool fill;
};


#endif
