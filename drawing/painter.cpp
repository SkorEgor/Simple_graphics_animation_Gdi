

#include "stdafx.h"
#include "drawing.h"
#include "painter.h"
#include <gdiplus.h>
using namespace Gdiplus;
using namespace std;

ULONG_PTR token;
bool notInit = true;

/*Параметры построение графика*/
int padding = 20;
int left_keys_padding = 20;
int bottom_keys_padding = 10;
unsigned int grid_size = 10;

int actual_width, actual_height, actual_top,
actual_bottom, actual_left, actual_right;
/******************************/

vector<double> y, x;

IMPLEMENT_DYNAMIC(painter, CStatic)

painter::painter()
{
	GdiplusStartupInput input;
	Status a;
	a = GdiplusStartup(&token, &input, NULL);
	if (a != Ok) MessageBox(L"Ошибка GDI", L"Error");
}

painter::~painter() { GdiplusShutdown(token); }

double delW;
void painter::updateGraph(vector<double> &inputY, vector<double> &inputX, double inDelW) {
	y = inputY;
	x = inputX;
	delW = inDelW;
	Invalidate(FALSE);
}

BEGIN_MESSAGE_MAP(painter, CStatic)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}

void painter::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (y.empty() && x.empty()) return;
	//Цвет подсетки
	Pen subgrid_pen (Color(105, 105, 105), 1);
	subgrid_pen.SetDashStyle(DashStyleDash);
	//Цвет сетки
	Pen grid_pen	(Color(0, 0, 0), 1);

	//Графика для отрисовки
	Graphics gr(lpDrawItemStruct->hDC);
	Rect rect;
	gr.GetClipBounds(&rect);
	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	//Создание буфера
	Gdiplus::Bitmap backBuffer(lpDrawItemStruct->rcItem.right+1, lpDrawItemStruct->rcItem.bottom+1, &gr);
	Graphics temp(&backBuffer);

	// Белый фон.
	temp.Clear(Color::White);
	//Расчет границ графика
	int width = lpDrawItemStruct->rcItem.right+1;
	int height = lpDrawItemStruct->rcItem.bottom+1;
	
	actual_width = width - 2 * padding - left_keys_padding;
	actual_height = height - 2 * padding - bottom_keys_padding;

	actual_top = padding;
	actual_bottom = actual_top + actual_height;
	actual_left = padding + left_keys_padding;
	actual_right = actual_left + actual_width;

	//Отрисовка подсетки
	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		//Вертикальные, линия подсетки
		temp.DrawLine(
			&subgrid_pen,								//const Pen * pen,
			actual_left + i * actual_width / grid_size,	//REAL      x1,
			actual_top,									//REAL      y1,
			actual_left + i * actual_width / grid_size,	//REAL      x2,
			actual_bottom								//REAL      y2
		);
		//Горизонтальные, линия подсетки
		temp.DrawLine(
			&subgrid_pen,								//const Pen * pen,
			actual_left,								//REAL      x1,
			actual_top + i * actual_height / grid_size,	//REAL      y1,
			actual_right,								//REAL      x2,
			actual_top + i * actual_height / grid_size	//REAL      y2
		);
	}

	//Отрисовка сетки
	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		//Вертикальные, линия сетки
		temp.DrawLine(
			&grid_pen,								//const Pen * pen,
			actual_left + i * actual_width / grid_size,	//REAL      x1,
			actual_top,									//REAL      y1,
			actual_left + i * actual_width / grid_size,	//REAL      x2,
			actual_bottom								//REAL      y2
		);
		//Горизонтальные, линия сетки
		temp.DrawLine(
			&grid_pen,								//const Pen * pen,
			actual_left,								//REAL      x1,
			actual_top + i * actual_height / grid_size,	//REAL      y1,
			actual_right,								//REAL      x2,
			actual_top + i * actual_height / grid_size	//REAL      y2
		);
	}

	//Отрисовка графика
	double data_x_min(x[0]), data_x_max(x[x.size()-1]);
	double data_y_min(y[0]), data_y_max(y[0]);
	for (int i = 0; i < y.size(); i++) {
		if (y[i] > data_y_max) data_y_max = y[i];
		if (y[i] < data_y_min) data_y_min = y[i];
	}

	/*
	y = convert_range(y, actual_top, actual_bottom, data_y_max, data_y_min);
	x = convert_range(x, actual_right, actual_left, data_x_max, data_x_min);
	
	for (unsigned int i = 0; i < y.size() - 1; i++)
	{
		temp.DrawLine(
			&data_pen,	//const Pen * pen,
			(REAL)x[i],	//REAL      x1,
			y[i],		//REAL      y1,
			x[i + 1],	//REAL      x2,
			y[i + 1]	//REAL      y2
		);
	}
	*/
	Gdiplus::Font myFont(L"Arial", 10);
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	
	for (int i = 0; i < grid_size / 2 +1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		temp.DrawString(
			str,
			str.GetLength(),
			&myFont,
			PointF(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2),
			&blackBrush);
		
		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		temp.DrawString(
			str,
			str.GetLength(),
			&myFont,
			PointF(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding),
			&blackBrush);
	}
	GraphicsContainer state = 0;
	//gr.DrawImage(&backBuffer, 0, 0, 0, 0, width, height, UnitPixel);

	Matrix matr;
	matr.Translate(actual_left, actual_top);
	double xScale = ((double)actual_width) / (x[x.size()-1]);
	matr.Scale(xScale, -actual_height / 4.);
	matr.Translate( 0, -2);
	temp.SetTransform(&matr);

	Pen data_pen(Color(255, 0, 0), 0.01);
	//рисуем график 
	for (unsigned int i = 0; i < y.size() - 1; i++)
	{
		temp.DrawLine(
			&data_pen,	//const Pen * pen,
			(REAL)x[i],	//REAL      x1,
			y[i],		//REAL      y1,
			x[i + 1],	//REAL      x2,
			y[i + 1]	//REAL      y2
		);
	}

	gr.DrawImage(&backBuffer, 0, 0, 0, 0, width, height, UnitPixel);

}