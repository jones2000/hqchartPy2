
// DemoDlg.h : header file
//

#pragma once
#include "HQChartControl.h"


// CDemoDlg dialog
class CDemoDlg : public CDialogEx
{
// Construction
public:
	CDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HQCHARTCSHARPCPLUSDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnRun();

private:
	HQChartControl m_HQChartCtrl;

	CEdit m_strCode;
	CEdit m_EditResult;
	CString m_strVersion;
	long m_lID = 0;
	
};
