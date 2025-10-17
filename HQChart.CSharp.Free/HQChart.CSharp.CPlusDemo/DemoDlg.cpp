
// DemoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "HQChart.CSharp.CPlusDemo.h"
#include "DemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemoDlg dialog



CDemoDlg::CDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HQCHARTCSHARPCPLUSDEMO_DIALOG, pParent)
	, m_strVersion(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CODE, m_strCode);
	DDX_Text(pDX, IDC_HQCHART_INFO, m_strVersion);
	DDX_Control(pDX, IDC_EDIT_RESUTL, m_EditResult);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDemoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_RUN, &CDemoDlg::OnBnClickedBtnRun)
END_MESSAGE_MAP()


// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_HQChartCtrl.Inital();

	m_strVersion.Format(L"Ver:%d:%d %s", m_HQChartCtrl.GetMainVersion(), m_HQChartCtrl.GetMinVersion(), m_HQChartCtrl.GetAuthorizeInfo().c_str());

	CString strCode = L"T:MA(C, 10);\r\nT2:USER_VAR1+USER_VAR2;\r\nT3:USER_FUNC1(C);";
	m_strCode.SetWindowText(strCode.GetString());

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDemoDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CDemoDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CDemoDlg::OnBnClickedBtnRun()
{
	++m_lID;
	CString strCode;
	m_strCode.GetWindowText(strCode);

	if (strCode.IsEmpty())
	{
		MessageBox(L"脚本不能为空", L"执行脚本错误");
		return;
	}

	CString strSymbol = L"600000.sh";
	CString strJobID;
	strJobID.Format(L"JobID-%s-%d", strSymbol.GetString(), m_lID);

	StringBuffer strBuffer;
	Writer<StringBuffer> jsonWriter(strBuffer);
	jsonWriter.StartObject();               // Between StartObject()/EndObject(), 

	jsonWriter.Key("Symbol");		//股票代码
	jsonWriter.String(WStringToUTF8(strSymbol.GetString()).c_str());

	jsonWriter.Key("Period");	//周期
	jsonWriter.Int(0);

	jsonWriter.Key("Right");	//复权
	jsonWriter.Int(0);

	jsonWriter.Key("Script");
	jsonWriter.String(WStringToUTF8(strCode.GetString()).c_str());

	jsonWriter.Key("JobID");
	jsonWriter.String(WStringToUTF8(strJobID.GetString()).c_str());

	jsonWriter.EndObject();

	std::wstring strConfig = UTF8ToWString(strBuffer.GetString());

	HQChartCacheManage::GetInstance().Create(strJobID.GetString());
	auto pRefCache = HQChartCacheManage::GetInstance().Get(strJobID.GetString());
	pRefCache->m_strSymbol = strSymbol.GetString();
	m_HQChartCtrl.Run(strConfig);

	m_EditResult.SetWindowText(pRefCache->m_strResult.c_str());

}
