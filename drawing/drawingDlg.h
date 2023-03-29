#pragma once

#include "painter.h"
#include "afxcmn.h"
#include <vector>

class CdrawingDlg : public CDialogEx
{
public:
	CdrawingDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_DRAWING_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	painter pictureControl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedStatic2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
