// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "systemtraydemo.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSystemTrayDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
	, m_bWinStart(FALSE)
	, m_b12heures(FALSE)
	, m_b14heures(FALSE)
	, m_bHeures(FALSE)
	, m_nHorloge(0)
	, m_bScreenSaver(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_CLOCKSND);
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HORLOGE_COMBO, m_cboHorloge);
	DDX_Check(pDX, IDC_12HEURES_CHECK, m_b12heures);
	DDX_Check(pDX, IDC_14HEURES_CHECK, m_b14heures);
	DDX_Check(pDX, IDC_HEURES_CHECK, m_bHeures);
	DDX_CBIndex(pDX, IDC_HORLOGE_COMBO, m_nHorloge);
	DDX_Check(pDX, IDC_WINSTART_CHECK, m_bWinStart);
	DDX_Check(pDX, IDC_SCREENSAVER_CHECK, m_bScreenSaver);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_CBN_SELCHANGE(IDC_HORLOGE_COMBO, OnSelchangeHorlogeCombo)
	ON_BN_CLICKED(IDC_TESTER15H00_BUTTON, OnTester15h00Button)
	ON_BN_CLICKED(IDC_TESTER00H30_BUTTON, OnTester00h30Button)
	ON_BN_CLICKED(IDC_TESTER00H15_BUTTON, OnTester00h15Button)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	HKEY	hKey;

	CDialog::OnInitDialog();

	// Définit l'icone
	SetIcon(m_hIcon, TRUE);	// TRUE = Petit

	// Load at Windows startup ?
	m_bWinStart = FALSE;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, "Clocks-Sounds", NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
			m_bWinStart = TRUE;
		RegCloseKey(hKey);
	}
	
	// Charge les options
	m_strEntree = "Options";
	int nSonne = theApp.m_nRingWhen;
	m_bHeures   = (nSonne & 4) == 4;
	m_b14heures = (nSonne & 2) == 2;
	m_b12heures = (nSonne & 1) == 1;
	m_nHorloge = theApp.m_nHorloge;
	m_bScreenSaver = theApp.m_bScreenSaver;
	UpdateData(FALSE);
	OnSelchangeHorlogeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDlg::OnOK() 
{
	CString			strThisExe;
	HKEY			hKey;
	DWORD			dwDisposition;

	// enregistre les options
	UpdateData(TRUE);
	AfxGetApp()->WriteProfileInt(m_strEntree, "RingWhen", 4*m_bHeures + 2*m_b14heures + 1*m_b12heures);
	AfxGetApp()->WriteProfileInt(m_strEntree, "BellType", m_nHorloge);
	AfxGetApp()->WriteProfileInt(m_strEntree, "ScreenSaver", m_bScreenSaver);

	// Reload config to set variables
	theApp.LoadConfig();

	// Create/Delete Reg Key
	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisposition);
	if (m_bWinStart)
	{
		// Find path to this exe
		strThisExe = GetCommandLine()+1;
		strThisExe = strThisExe.Left(strThisExe.Find('\"'));
        RegSetValueEx(hKey, "Clocks-Sounds", 0, REG_SZ, (BYTE*) (LPCSTR) strThisExe, strThisExe.GetLength()+1);
	}
	else
		RegDeleteValue(hKey, "Clocks-Sounds");
	RegCloseKey(hKey);

	CDialog::OnOK();
}

void COptionsDlg::OnSelchangeHorlogeCombo() 
{
	// enlève ou met 1/4h et 1/2h
	UpdateData(TRUE);
	GetDlgItem(IDC_14HEURES_CHECK)->EnableWindow(m_nHorloge == 0);
	GetDlgItem(IDC_TESTER00H15_BUTTON)->EnableWindow(m_nHorloge == 0);
	UpdateData(FALSE);
}

void COptionsDlg::OnTester15h00Button() 
{
	UpdateData(TRUE);
	theApp.PlayClock(0, m_nHorloge, 3);
}

void COptionsDlg::OnTester00h15Button() 
{
	UpdateData(TRUE);
	theApp.PlayClock(15, m_nHorloge, 3);
}

void COptionsDlg::OnTester00h30Button() 
{
	UpdateData(TRUE);
	theApp.PlayClock(30, m_nHorloge, 3);
}

void COptionsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
}

void COptionsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// Stop current sound
	theApp.StopClock();
}
