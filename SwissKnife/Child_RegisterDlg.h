#pragma once
#include "afxcmn.h"
#include "RegisterOperation.h"

// CChild_RegisterDlg �Ի���

class CChild_RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_RegisterDlg)

public:
	CChild_RegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild_RegisterDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CImageList	m_ImageListTree;   //���ؼ��ϵ�ͼ��	
	CImageList m_ImageListControlList;         //ControlList�ϵ�ͼ��
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
