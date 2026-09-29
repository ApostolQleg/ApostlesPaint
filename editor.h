#pragma once

#include "Resource.h"

class Editor {
public:
	virtual ~Editor() {};

	virtual void OnLBdown(HWND) = 0;
	virtual void OnLBup(HWND) = 0;
	virtual void OnMouseMove(HWND) = 0;
	virtual void OnPaint(HWND) = 0;
};

class ShapeEditor : public Editor
{
protected:
	bool isDrawing = false;
	long xstart = 0, ystart = 0;
	long xend = 0, yend = 0;
public:
	ShapeEditor() {}
	virtual ~ShapeEditor() {}

	void OnLBdown(HWND hWnd) override;
	void OnLBup(HWND hWnd) override;
	void OnMouseMove(HWND hWnd) override;
	void OnPaint(HWND hWnd) override;

	virtual void OnInitMenuPopup(HWND hWnd, WPARAM wParam);
};

class PointEditor : public ShapeEditor
{
public:
	void OnLBup(HWND hWnd) override;
	void OnInitMenuPopup(HWND hWnd, WPARAM wParam) override;
};

class LineEditor : public ShapeEditor
{
public:
	void OnMouseMove(HWND hWnd) override;
	void OnLBup(HWND hWnd) override;
	void OnInitMenuPopup(HWND hWnd, WPARAM wParam) override;
};

class RectEditor : public ShapeEditor
{
public:
	void OnMouseMove(HWND hWnd) override;
	void OnLBup(HWND hWnd) override;
	void OnInitMenuPopup(HWND hWnd, WPARAM wParam) override;
};

class EllipseEditor : public ShapeEditor
{
public:
	void OnMouseMove(HWND hWnd) override;
	void OnLBup(HWND hWnd) override;
	void OnInitMenuPopup(HWND hWnd, WPARAM wParam) override;
};