#pragma once

class Shape
{
protected:
	long xstart = 0, ystart = 0;
	long xend = 0, yend = 0;
	bool rubber = false;

	HPEN CreateShapePen() const;
public:
	virtual ~Shape() {};

	void Set(long x1, long y1, long x2, long y2);
	void SetRubber(bool r) { rubber = r; }

	virtual void Show(HDC) = 0;
	virtual Shape* Clone() const = 0;
};

class Point : public Shape {
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Point(*this); }
};

class Line : public Shape {
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Line(*this); }
};

class Rect : public Shape {
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Rect(*this); }
};

class Elips : public Shape {
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Elips(*this); }
};