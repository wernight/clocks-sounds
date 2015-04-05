#if !defined(AFX_SONNERIE_H__3333BF40_6D7B_11D2_B53F_CDBA4875CF17__INCLUDED_)
#define AFX_SONNERIE_H__3333BF40_6D7B_11D2_B53F_CDBA4875CF17__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Sonnerie.h : header file
//
#include "MaskEd.h"
#include "afxdtctl.h"

/////////////////////////////////////////////////////////////////////////////
// CSonnerieDlg dialog

class CSonnerieDlg : public CDialog
{
// Construction
public:
	CSonnerieDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SONNERIE };
	int		m_nBellType;
	CString	m_strDateNow;
	int		m_nAlarm;
	CDateTimeCtrl m_ctlDate;
	CDateTimeCtrl m_ctlTime;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSonnerieDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSonnerieDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnTesterButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAlarmRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SONNERIE_H__3333BF40_6D7B_11D2_B53F_CDBA4875CF17__INCLUDED_)
