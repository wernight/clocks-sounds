// SystemTrayDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SystemTrayDemo.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemTrayDemoApp

BEGIN_MESSAGE_MAP(CSystemTrayDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTrayDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTrayDemoApp construction

CSystemTrayDemoApp::CSystemTrayDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTrayDemoApp object

CSystemTrayDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSystemTrayDemoApp initialization
#define	WM_ICON_NOTIFY			WM_USER+10

BOOL CSystemTrayDemoApp::InitInstance()
{
	// Standard initialization
	LoadConfig();

	m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, "Stop Sound");

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_POPUP_MENU))
		return FALSE;
	m_pMainWnd = pMainFrame;

	return TRUE;
}

void CSystemTrayDemoApp::LoadConfig()
{
	CString strSection;
	CString strBellTime;

	// Options
	strSection = _T("Options");
	m_nRingWhen = GetProfileInt(strSection, "RingWhen", 4);
	m_nHorloge = GetProfileInt(strSection, "BellType", 0);
	m_bScreenSaver = GetProfileInt(strSection, "ScreenSaver", TRUE) != 0;

	// Sonnerie
	strSection = _T("Bell");
	m_nBellActivate = GetProfileInt(strSection, "Activate", 0);
	m_nBellType = GetProfileInt(strSection, "BellType", 0);
	m_nBellTimeHours = GetProfileInt(strSection,  "Time_Hr", 0);
	m_nBellTimeMins = GetProfileInt(strSection,  "Time_Mn", 0);
	m_nDateDay = GetProfileInt(strSection,"Date_Day", 01);
	m_nDateMonth = GetProfileInt(strSection, "Date_Month", 01);
	m_nDateYear = GetProfileInt(strSection, "Date_Year", 2000);
}

void CSystemTrayDemoApp::PlayClock(int nMinutes, int nHorloge, int nSonneries)
{
	// Stop current sound
	StopClock();
	WaitForSingleObject(m_hThread, INFINITE);
	ResetEvent(m_hStopEvent);

	// Play new sound
	m_dwParam.nMinutes = nMinutes;
	m_dwParam.nHorloge = nHorloge;
	m_dwParam.nSonneries = nSonneries;
	m_dwParam.hStopEvent = m_hStopEvent;
	m_hThread = (HANDLE) _beginthread(ThreadPlayClock, 0, &m_dwParam);
}

// Param: nTime(minutes) : nHorloge(id) : nSonneries(number)
void CSystemTrayDemoApp::ThreadPlayClock(void *lpParam)
{
	int		nTime = ((PLAY_SOUND*)lpParam)->nMinutes,
			nHorloge = ((PLAY_SOUND*)lpParam)->nHorloge,
			nSonneries = ((PLAY_SOUND*)lpParam)->nSonneries,
			i;
	HANDLE	hStopEvent = ((PLAY_SOUND*)lpParam)->hStopEvent;

	/* WARNING: nSonneries=0 is a special value. */

	// Sonne
	switch (nHorloge)
	{
	case 0:	// Carillon Westminster
		switch (nTime)
		{
		case 0:
			PlaySound(IDW_WMINSTER_H, GetModuleHandle(NULL), SND_FILENAME | SND_SYNC);
			for (i=0; i<nSonneries-1 && WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT; i++)
				PlaySound(IDW_WMINSTER_HPLUS, GetModuleHandle(NULL), SND_FILENAME | SND_SYNC);
			if (i<nSonneries && WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT)
				PlaySound(IDW_WMINSTER_HPLUS, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		case 15:
			PlaySound(IDW_WMINSTER_15M, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		case 30:
			PlaySound(IDW_WMINSTER_30M, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		case 45:
			PlaySound(IDW_WMINSTER_45M, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		}
		break;
	case 3:	// Cuckoo
		switch (nTime)
		{
		case 0:
			PlaySound(IDW_CUCKOO_START, GetModuleHandle(NULL), SND_FILENAME | SND_SYNC);
			for (i=0; i<nSonneries-1 && WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT; i++)
				PlaySound(IDW_CUCKOO_CL, GetModuleHandle(NULL), SND_FILENAME | SND_SYNC);
			if (WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT)
				PlaySound(IDW_CUCKOO_END, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		case 30:
			PlaySound(IDW_CUCKOO_START, GetModuleHandle(NULL), SND_FILENAME | SND_SYNC);
			if (WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT)
				PlaySound(IDW_CUCKOO_END, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		}
		break;
	default:	// Autres
		switch (nTime)
		{
		case 0:
			for (i=0; i<nSonneries-1 && WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT; i++)
				PlaySound(IDW_OTHER(nHorloge), GetModuleHandle(NULL), SND_FILENAME | SND_SYNC);
			if (WaitForSingleObject(hStopEvent, 0)==WAIT_TIMEOUT)
				PlaySound(IDW_OTHER(nHorloge), GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		case 30:
			PlaySound(IDW_OTHER(nHorloge), GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
			break;
		}
	}
}

void CSystemTrayDemoApp::StopClock()
{
	SetEvent(m_hStopEvent);
	PlaySound(NULL, GetModuleHandle(NULL), SND_PURGE);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strWeb;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strWeb = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_WEB_EDIT, m_strWeb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

// App command to run the dialog
void CSystemTrayDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTrayDemoApp commands

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg) 
{
	// Site Web
	if (pMsg->hwnd == GetDlgItem(IDC_WEB_EDIT)->m_hWnd &&
		pMsg->message == WM_LBUTTONDOWN)
	{
		CString strWeb;
		strWeb.LoadString(IDC_WEB_EDIT);
		ShellExecute(GetActiveWindow()->m_hWnd,
					 "open",
					 strWeb,
					 NULL,
					 NULL,
					 SW_SHOWNORMAL);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Site Web
	CString strWeb;
	strWeb.LoadString(IDC_WEB_EDIT);
	m_strWeb = strWeb;
	UpdateData(FALSE);

	// Lien
	CFont *pFontLien = new CFont;
	pFontLien->CreateFont(-8, NULL,
						  NULL,
						  NULL,
						  FW_NORMAL, FALSE, TRUE, FALSE,
						  DEFAULT_CHARSET,
						  OUT_DEFAULT_PRECIS,
						  CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY,
						  DEFAULT_PITCH | FF_SWISS,
						  "MS Sans Serif");
	GetDlgItem(IDC_WEB_EDIT)->SetFont(pFontLien, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// Change the cursor
	int nWndID = pWnd->GetDlgCtrlID();
	if (nWndID == IDC_WEB_EDIT)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_LIEN_CURSOR));
		return TRUE;
	}
	else
		return CDialog::OnSetCursor(pWnd, nHitTest, message);	
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// Change the color
	int nWndID = pWnd->GetDlgCtrlID();
	if (nWndID == IDC_WEB_EDIT)
		pDC->SetTextColor(RGB(0,0,255));

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC		dcBmp;
	CBitmap	bmp;
	CRect	rectWnd;

	// Get client paint zone
	GetDlgItem(IDC_LOGO_PICTURE)->GetWindowRect(rectWnd);
	ScreenToClient(rectWnd);

	// Put the alpha
	bmp.LoadBitmap(IDB_LIBERTYS_A);
	dcBmp.CreateCompatibleDC(&dc);
	dcBmp.SelectObject(bmp);
	dc.BitBlt(rectWnd.left, rectWnd.top, rectWnd.Width(), rectWnd.Height(), &dcBmp, 0, 0, SRCAND);

	// Put the logo (over the alpha)
	bmp.LoadBitmap(IDB_LIBERTYS);
	dcBmp.SelectObject(bmp);
	dc.BitBlt(rectWnd.left, rectWnd.top, rectWnd.Width(), rectWnd.Height(), &dcBmp, 0, 0, SRCPAINT);

	// Clear memory
	dcBmp.DeleteDC();
	bmp.DeleteObject();
}
