// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SystemTrayDemo.h"
#include "MainFrm.h"
#include "OptionsDlg.h"
#include "Sonnerie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WM_ICON_NOTIFY			WM_USER+10

extern CSystemTrayDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_OPTIONS_MENU, OnMenuOptions)
	ON_COMMAND(ID_SONNERIE, OnMenuSonnerie)
	ON_COMMAND(ID_FINSON, OnMenuStop)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the tray icon
	if (!m_TrayIcon.Create(NULL,				// Parent window
                           WM_ICON_NOTIFY,		// Icon notify message to use
                           _T("Clocks-Sounds"),	// tooltip
                           AfxGetApp()->LoadIcon(IDI_CLOCKSND16),// Icon to use
                           IDR_POPUP_MENU))		// ID of tray icon
		return -1;

	
	// Clignotte quand sonne
	m_TrayIcon.SetIconList(IDI_BELL1, IDI_ALARM);

	// Menu Options par défaut
	m_TrayIcon.SetMenuDefaultItem(5, TRUE);

	// Sonne
	SetTimer(1, 5000, NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame/CSystemTray menu message handlers

void CMainFrame::OnMenuOptions()
{
	COptionsDlg dlgOptions;
	dlgOptions.DoModal();
}

void CMainFrame::OnMenuSonnerie()
{
	CSonnerieDlg dlgSonnerie;
	dlgSonnerie.DoModal();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case 1:
		TestSonne();
		break;
	case 2:
		// Auto stop bell
		KillTimer(nIDEvent);
		PlaySound(NULL, GetModuleHandle(NULL), SND_PURGE);
		m_TrayIcon.StopAnimation();
		break;
	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMenuStop()
{
	m_TrayIcon.StopAnimation();
	theApp.StopClock();
}

void CMainFrame::TestSonne()
{
	static int nPrevMinutes = -1;
	bool bCanRingAtDate;
	CString strHorloge;	//pour avoir l'ID
	int	nMinutes;
	int nSonneries;
	CTime tHeure = CTime::GetCurrentTime();
	HICON hTrayIcon = m_TrayIcon.GetIcon();

	// Nombre de coups
	nSonneries = tHeure.GetHour()%12;
	if (nSonneries == 0)
		nSonneries = 12;
	
	// Affiche si la sonnerie est mise
	bCanRingAtDate = tHeure.GetDay() + 100*tHeure.GetMonth() + 1000*tHeure.GetYear() <= theApp.m_nDateDay + 100*theApp.m_nDateMonth + 1000*theApp.m_nDateYear;
	if (theApp.m_nBellActivate == 1 || (theApp.m_nBellActivate == 2 && bCanRingAtDate))
	{
		if  (hTrayIcon != AfxGetApp()->LoadIcon(IDI_ALARM))
			m_TrayIcon.SetIcon(IDI_ALARM);
	}
	else if (theApp.m_nBellActivate != 2 || !bCanRingAtDate)
	{
		if  (hTrayIcon != AfxGetApp()->LoadIcon(IDI_CLOCKSND16))
			m_TrayIcon.SetIcon(IDI_CLOCKSND16);
	}
	nMinutes = tHeure.GetMinute();

	// Only if new minute
	if (nMinutes == nPrevMinutes)
		return;
	nPrevMinutes = nMinutes;

	// Alarme (sonnerie)
	if (theApp.m_nBellActivate &&										// If there is an alarm
		tHeure.GetHour() == theApp.m_nBellTimeHours &&					// And time = alarm time
		nMinutes == theApp.m_nBellTimeMins &&							// If minutes = alarm minutes
		(theApp.m_nBellActivate == 1 ||									// And If asked for date
		(tHeure.GetDay() == theApp.m_nDateDay &&						// Check day
		tHeure.GetMonth() == theApp.m_nDateMonth &&						//       month
		tHeure.GetYear() == theApp.m_nDateYear)))						//       year
	{
		// Start
		switch (theApp.m_nBellType)
		{
		case 0:
			PlaySound(IDW_ALARM_WESTMINSTER, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
			break;
		case 1:
			PlaySound(IDW_ALARM_CUCKOO, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
			break;
		case 2:
			PlaySound(IDW_ALARM_COQ, GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
			break;
		}
		m_TrayIcon.Animate(500, 60);
		SetTimer(2, 60000, NULL);		// Stop after 1 min auto
	}
	else
	{
		// Disable during screen saver
		BOOL bScreenSaverRunning;
		if (theApp.m_bScreenSaver && SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &bScreenSaverRunning, 0) && bScreenSaverRunning)
			return;

		// Sonne (options)
		switch (nMinutes)
		{
		case 0:
			// Play sound if in option asked to play for this time
			if (theApp.m_nRingWhen & 4)			// 1h00
				theApp.PlayClock(0, theApp.m_nHorloge, nSonneries);
			else if (theApp.m_nRingWhen & 3)	// 0h15/0h45 ou 0h30
				theApp.PlayClock(0, theApp.m_nHorloge, 0);
			break;
		case 30:
			// Play sound if in option asked to play for this time
			if (theApp.m_nRingWhen & 3)			// 0h15/0h45 ou 0h30
				theApp.PlayClock(30, theApp.m_nHorloge, 0);
			break;
		case 15:
		case 45:
			// Play sound if in option asked to play for this time
			if (theApp.m_nRingWhen & 2)			// 0h15/0h45
				theApp.PlayClock(nMinutes, theApp.m_nHorloge, 0);
			break;
		}
	}
}
