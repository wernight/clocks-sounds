// Sonnerie.cpp : implementation file
//

#include "stdafx.h"
#include "systemtraydemo.h"
#include "Sonnerie.h"

#define REG_SECTION _T("Bell")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSystemTrayDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSonnerieDlg dialog


CSonnerieDlg::CSonnerieDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSonnerieDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSonnerieDlg)
	m_nBellType = theApp.m_nBellType;
	m_strDateNow = _T("");
	m_nAlarm = theApp.m_nBellActivate;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_CLOCKSND);
}


void CSonnerieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSonnerieDlg)
	DDX_CBIndex(pDX, IDC_TYPE_COMBO, m_nBellType);
	DDX_Text(pDX, IDC_DATENOW_STATIC, m_strDateNow);
	DDX_Radio(pDX, IDC_ALARM_RADIO, m_nAlarm);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DATEPICKER, m_ctlDate);
	DDX_Control(pDX, IDC_TIMEPICKER, m_ctlTime);
}


BEGIN_MESSAGE_MAP(CSonnerieDlg, CDialog)
	//{{AFX_MSG_MAP(CSonnerieDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TESTER_BUTTON, OnTesterButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ALARM_RADIO, OnAlarmRadio)
	ON_BN_CLICKED(IDC_ALARM2_RADIO, OnAlarmRadio)
	ON_BN_CLICKED(IDC_ALARM3_RADIO, OnAlarmRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSonnerieDlg message handlers

BOOL CSonnerieDlg::OnInitDialog() 
{
	SYSTEMTIME	st;
	CString		m_strDate;

	CDialog::OnInitDialog();

	// Définit l'icone
	SetIcon(m_hIcon, TRUE);	// TRUE = Petit

	// Affiche la date & l'heure en cours
	m_strDateNow = CTime::GetCurrentTime().Format("%c");
	SetTimer(1, 30000, NULL);

	// Charge les paramètres
	ZeroMemory(&st, sizeof(SYSTEMTIME));
	st.wHour = theApp.m_nBellTimeHours;
	st.wMinute = theApp.m_nBellTimeMins;
	st.wDay = theApp.m_nDateDay;
	st.wMonth = theApp.m_nDateMonth;
	st.wYear = theApp.m_nDateYear;
	VERIFY( m_ctlTime.SetFormat(_T("hh:mm")) );
	VERIFY( m_ctlTime.SetTime(&st) );
	VERIFY( m_ctlDate.SetTime(&st) );

	UpdateData(FALSE);
	OnAlarmRadio();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSonnerieDlg::OnPaint() 
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

void CSonnerieDlg::OnOK() 
{
	SYSTEMTIME		time,
					date;

	// enregistre les options
	if (!UpdateData(TRUE))
		return;
	m_ctlTime.GetTime(&time);
	m_ctlDate.GetTime(&date);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "Activate", m_nAlarm);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "BellType", m_nBellType);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "Time_Hr", time.wHour);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "Time_Mn", time.wMinute);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "Date_Day", date.wDay);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "Date_Month", date.wMonth);
	AfxGetApp()->WriteProfileInt(REG_SECTION, "Date_Year", date.wYear);

	// Reload config
	theApp.LoadConfig();
	
	CDialog::OnOK();
}

void CSonnerieDlg::OnTesterButton() 
{
	UpdateData(TRUE);
	switch (m_nBellType)
	{
	case 0:
		sndPlaySound(IDW_ALARM_WESTMINSTER, SND_FILENAME | SND_ASYNC);
		break;
	case 1:
		sndPlaySound(IDW_ALARM_CUCKOO, SND_FILENAME | SND_ASYNC);
		break;
	case 2:
		sndPlaySound(IDW_ALARM_COQ, SND_FILENAME | SND_ASYNC);
		break;
	}
}

void CSonnerieDlg::OnTimer(UINT nIDEvent) 
{
	// Affiche la date & l'heure en cours
	m_strDateNow = CTime::GetCurrentTime().Format("%c");
	UpdateData(FALSE);
	
	CDialog::OnTimer(nIDEvent);
}

void CSonnerieDlg::OnAlarmRadio() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_TYPE_COMBO)->EnableWindow(m_nAlarm > 0);
	GetDlgItem(IDC_TESTER_BUTTON)->EnableWindow(m_nAlarm > 0);
	GetDlgItem(IDC_TIMEPICKER)->EnableWindow(m_nAlarm > 0);

	GetDlgItem(IDC_DATEPICKER)->EnableWindow(m_nAlarm > 1);
}
