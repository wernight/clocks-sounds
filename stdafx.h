// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DB628087_D595_11D1_97E7_00A0243D1382__INCLUDED_)
#define AFX_STDAFX_H__DB628087_D595_11D1_97E7_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <MMSystem.h>
#include <process.h>

// CG: The following line was added by the Windows Multimedia component.
#pragma comment(lib, "winmm.lib")

#define IDW_ALARM_COQ			_T("Alarm\\coq.wav")
#define IDW_ALARM_CUCKOO		_T("Alarm\\cuckoo.wav")
#define IDW_ALARM_WESTMINSTER	_T("Alarm\\westminster.wav")
#define IDW_CUCKOO_CL			_T("Chime\\cuckoocl.wav")
#define IDW_CUCKOO_END			_T("Chime\\cuckooclFin.wav")
#define IDW_CUCKOO_START		_T("Chime\\cuckooclDebut.wav")
#define IDW_WMINSTER_15M		_T("Chime\\carillon14H.wav")
#define IDW_WMINSTER_30M		_T("Chime\\carillon12H.wav")
#define IDW_WMINSTER_45M		_T("Chime\\carillon34H.wav")
#define IDW_WMINSTER_H			_T("Chime\\carillonH.wav")
#define IDW_WMINSTER_HPLUS		_T("Chime\\carillonHPlus.wav")
#define IDW_OTHER(a)			(a)==1?_T("Chime\\cloche1.wav"):(a)==2?_T("Chime\\clocktow.wav"):(a)==4?_T("Chime\\grandfat.wav"):(a)==5?_T("Chime\\spiral.wav"):NULL

#endif // !defined(AFX_STDAFX_H__DB628087_D595_11D1_97E7_00A0243D1382__INCLUDED_)
