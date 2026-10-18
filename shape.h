#pragma once

class Shape
{
protected:
	long x1 = 0, y1 = 0;
	long x2 = 0, y2 = 0;
	bool rubber = false;

	HPEN CreateShapePen() const;
public:
	virtual ~Shape() {};

	void Set(long x1, long y1, long x2, long y2);
	void SetRubber(bool r) { rubber = r; }

	virtual void Show(HDC) = 0;
	virtual Shape* Clone() const = 0;
	virtual const WCHAR* Get() const = 0;
};

class Point : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Point(*this); }
	const WCHAR* Get() const override { return L"Point"; }
};

class Line : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Line(*this); }
	const WCHAR* Get() const override { return L"Line\t"; }
};

class Rect : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Rect(*this); }
	const WCHAR* Get() const override { return L"Rectangle"; }
};

class Elips : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Elips(*this); }
	const WCHAR* Get() const override { return L"Ellipse"; }
};

class LineOO : public Line, public Elips 
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new LineOO(*this); }
	const WCHAR* Get() const override { return L"LineOO"; }
};


class Cube : public Line, public Rect
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Cube(*this); }
	const WCHAR* Get() const override { return L"Cube\t"; }
};