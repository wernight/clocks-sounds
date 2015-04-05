#if !defined(AFX_OPTIONSDLG_H__F03A3F60_3F6D_11D2_B53D_C8C769A91C07__INCLUDED_)
#define AFX_OPTIONSDLG_H__F03A3F60_3F6D_11D2_B53D_C8C769A91C07__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_OPTIONS };
	CComboBox	m_cboHorloge;
	BOOL	m_b12heures;
	BOOL	m_b14heures;
	BOOL	m_bHeures;
	int		m_nHorloge;
	BOOL	m_bWinStart;
	BOOL	m_bScreenSaver;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD m_dwParam;
	CString m_strEntree;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeHorlogeCombo();
	afx_msg void OnTester15h00Button();
	afx_msg void OnTester00h30Button();
	afx_msg void OnTester00h15Button();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__F03A3F60_3F6D_11D2_B53D_C8C769A91C07__INCLUDED_)
