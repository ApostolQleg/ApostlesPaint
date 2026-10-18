#pragma once

class Shape
{
protected:
	long x1 = 0, y1 = 0;
	long x2 = 0, y2 = 0;
	bool rubber = false;
	bool highlight = false;

	HPEN CreateShapePen() const;
public:
	virtual ~Shape() {};

	void Set(long x1, long y1, long x2, long y2);
	void SetRubber(bool r) { rubber = r; }
	void SetHighlight(bool h) { highlight = h; }

	virtual void Show(HDC hdc) = 0;
	virtual Shape* Clone() const = 0;
	virtual const WCHAR* GetName() const = 0;
	virtual void GetInfo(WCHAR* buf, size_t size) const {
		swprintf_s(buf, size, L"%s\t%ld\t%ld\t%ld\t%ld", GetName(), x1, y1, x2, y2);
	}
};

class Point : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Point(*this); }
	const WCHAR* GetName() const override { return L"Point\t"; }
};

class Line : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Line(*this); }
	const WCHAR* GetName() const override { return L"Line\t"; }
};

class Rect : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Rect(*this); }
	const WCHAR* GetName() const override { return L"Rectangle"; }
};

class Elips : virtual public Shape
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Elips(*this); }
	const WCHAR* GetName() const override { return L"Ellipse\t"; }
};

class LineOO : public Line, public Elips 
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new LineOO(*this); }
	const WCHAR* GetName() const override { return L"LineOO\t"; }
};


class Cube : public Line, public Rect
{
public:
	void Show(HDC) override;
	Shape* Clone() const override { return new Cube(*this); }
	const WCHAR* GetName() const override { return L"Cube\t"; }
};