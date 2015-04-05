// SystemTrayDemo.h : main header file for the SYSTEMTRAYDEMO application
//

#if !defined(AFX_SYSTEMTRAYDEMO_H__DB628085_D595_11D1_97E7_00A0243D1382__INCLUDED_)
#define AFX_SYSTEMTRAYDEMO_H__DB628085_D595_11D1_97E7_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSystemTrayDemoApp:
// See SystemTrayDemo.cpp for the implementation of this class
//

struct PLAY_SOUND
{
	int	nMinutes,
		nHorloge,
		nSonneries;
	HANDLE	hStopEvent;
};

class CSystemTrayDemoApp : public CWinApp
{
public:
	void StopClock();
	CSystemTrayDemoApp();
	void LoadConfig();
	void PlayClock(int nMinutes, int nHorloge, int nSonneries);
	static void ThreadPlayClock(void *lpParam);
	int		m_nRingWhen;
	int		m_nHorloge;
	int		m_nBellActivate;
	int		m_nBellType;
	int		m_nDateDay;
	int		m_nDateMonth;
	int		m_nDateYear;
	int		m_nBellTimeHours;
	int		m_nBellTimeMins;
	bool	m_bScreenSaver;
	HANDLE	m_hStopEvent;
	HANDLE	m_hThread;
	PLAY_SOUND m_dwParam;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTrayDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSystemTrayDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRAYDEMO_H__DB628085_D595_11D1_97E7_00A0243D1382__INCLUDED_)
