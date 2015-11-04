#ifndef SHAPES_H
#define SHAPES_H

class ggShape
{
public:
	virtual void Draw(class wxDC &) = 0;
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
	virtual void Draw(class wxDC &);
	ggPoint(int xx, int yy, int c) : ggShape(c), x(xx), y(yy) {
	}
private:
	int x, y;
};

class ggCircle : public ggShape
{
public:
	virtual void Draw(class wxDC &);
	ggCircle(int xx, int yy, int rr, int c) : ggShape(c), x(xx), y(yy), r(rr) {
	}
private:
	int x, y, r;
};

class ggSegm : public ggShape
{
public:
	virtual void Draw(class wxDC &);
	ggSegm(int xx1, int yy1, int xx2, int yy2, int ww, int c) : ggShape(c), x1(xx1), y1(yy1), x2(xx2), y2(yy2), w(ww) {
	}
private:
	int x1, y1, x2, y2, w;
};

class ggArc : public ggShape
{
public:
	virtual void Draw(class wxDC &);
	ggArc(int xx1, int yy1, int xx2, int yy2, int cxx, int cyy, int ww, int c) : ggShape(c), x1(xx1), y1(yy1), x2(xx2), y2(yy2), cx(cxx), cy(cyy), w(ww) {
	}
private:
	int x1, y1, x2, y2, cx, cy, w;
};


#endif
