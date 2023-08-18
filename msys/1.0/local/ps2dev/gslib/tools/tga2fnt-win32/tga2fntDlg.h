// tga2fntDlg.h : header file
//

#pragma once


// Ctga2fntDlg dialog
class Ctga2fntDlg : public CDialog
{
// Construction
public:
	Ctga2fntDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TGA2FNT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTgaPick();
	afx_msg void OnBnClickedDatPick();
	afx_msg void OnBnClickedOk();
	CString tga_filename;
	CString dat_filename;
	UINT CharWidth;
	UINT CharHeight;
	UINT GridWidth;
	UINT GridHeight;
};
