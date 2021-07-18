#pragma once
#include "afxcmn.h"
#include "RegisterOperation.h"

// CChild_RegisterDlg 对话框

class CChild_RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_RegisterDlg)

public:
	CChild_RegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild_RegisterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CImageList	m_ImageListTree;   //树控件上的图标	
	CImageList m_ImageListControlList;         //ControlList上的图标
	CTreeCtrl m_Tree;
	CListCtrl m_ControlList;
	HICON     m_hIcon;
	HTREEITEM	m_hRoot;
	HTREEITEM	HKLM;
	HTREEITEM	HKCR;
	HTREEITEM	HKCU;
	HTREEITEM	HKUS;
	HTREEITEM	HKCC;
	HTREEITEM   m_hSelectedItem;
	afx_msg void OnSelchangedTreeReg(NMHDR *pNMHDR, LRESULT *pResult);
	VOID Find(char bToken, char * szPath);
	CString GetFullPath(HTREEITEM hCurrent);
	char GetFatherPath(CString & strFullPath);
	void AddPath(char * szBuffer);
	void AddKey(char * szBuffer);
};
