#pragma once

#include <iostream>
#include <vector>  // подключили библиотеку
using namespace std;

class painter : public CStatic
{
	DECLARE_DYNAMIC(painter)

public:
	painter();
	virtual ~painter();
	void updateGraph(vector<double> &y, vector<double> &x, double inDelW);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


