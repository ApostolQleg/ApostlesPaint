#pragma once

class Shape
{
protected:
	long xstart = 0, ystart = 0;
	long xend = 0, yend = 0;
public:
	virtual ~Shape() {};

	void Set(long x1, long y1, long x2, long y2);
	virtual void Show(HDC) = 0;
};

class PointShape : public Shape {
public:
	void Show(HDC) override;
};

class LineShape : public Shape {
public:
	void Show(HDC) override;
};

class RectShape : public Shape {
public:
	void Show(HDC) override;
};

class EllipseShape : public Shape {
public:
	void Show(HDC) override;
};