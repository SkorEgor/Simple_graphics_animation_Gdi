#include "stdafx.h"
#include "drawing.h"
#include "drawingDlg.h"
#include "afxdialogex.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int countPoint = 600;
const double samplingFrequency = 500;
const double w1 = 100;
const double w2 = 110;

double graphFunction(double w1, double w2, double fi, double x) {
	return (cos(w1 * x + fi) + cos(w2 * x));
}

CdrawingDlg::CdrawingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdrawingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdrawingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, pictureControlID, pictureControl);
}

BEGIN_MESSAGE_MAP(CdrawingDlg, CDialogEx)
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CdrawingDlg::OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CdrawingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdrawingDlg::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CdrawingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	return TRUE;
}

void CdrawingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CdrawingDlg::OnStnClickedStatic2() { UpdateData(FALSE); }

void CdrawingDlg::OnBnClickedOk() { CDialogEx::OnOK(); }

int countIter = 0;
void CdrawingDlg::OnBnClickedButton1()
{
	vector<double> x, y;
	for (int i = 0; i < countPoint; i++) {
		x.push_back((i) / samplingFrequency);
		y.push_back(graphFunction(w1,w2, countIter / samplingFrequency,x[i]));
	}
	countIter++;

	pictureControl.updateGraph(y, x, abs(w1 - w2));
}


bool status_th = false;
/*
DWORD WINAPI animationThread(LPVOID t)
{
	CdrawingDlg* thisWind = (CdrawingDlg*)t;
	vector<double> x, y;
	for (int i = 0; i < countPoint; i++) {
		x.push_back((i) / samplingFrequency);
		y.push_back(graphFunction(w1, w2, countIter / samplingFrequency, x[i]));
		
	}
	countIter++;
	thisWind->pictureControl.updateGraph(y, x, abs(w1 - w2));
	return 0;
}*/

bool startAnimation = false;
UINT_PTR m_nTimer;
void CdrawingDlg::OnBnClickedButton2()
{
	if (startAnimation) {
		KillTimer(m_nTimer);
		startAnimation = false;
	}
	else {
		m_nTimer = SetTimer(1, 100, NULL);
		startAnimation = true;
	}
}



void CdrawingDlg::OnTimer(UINT_PTR nIDEvent)
{
	vector<double> x, y;
	for (int i = 0; i < countPoint; i++) {
		x.push_back((i) / samplingFrequency);
		y.push_back(graphFunction(w1, w2, (countIter*5) / samplingFrequency, x[i]));
	}
	countIter++;

	pictureControl.updateGraph(y, x, abs(w1-w2));
	CDialogEx::OnTimer(nIDEvent);
}
