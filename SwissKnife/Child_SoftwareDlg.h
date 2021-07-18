#pragma once
#include "afxcmn.h"


// CChild_SoftwareDlg �Ի���



class CChild_SoftwareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_SoftwareDlg)

public:
	CChild_SoftwareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild_SoftwareDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SOFTWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	BOOL ShowList();

	CListCtrl	m_list;
	HICON       m_hIcon;
	CListCtrl   m_ListSoft;
	int         m_Select;
	CString     strTemp;


	afx_msg BOOL StupSoftwareInfo();
	void OnLvnItemchangedListSoft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListSoft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSoftUnload();
	afx_msg void OnSpftwareReflash();
	BOOL OpenFolderAndSelectFile(TCHAR * szPath);
	afx_msg void OnSofetwareOpen();
};
