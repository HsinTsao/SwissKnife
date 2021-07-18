#pragma once
#include "afxcmn.h"
#include "Child_ProcessDlg.h"

// CDLLCheck �Ի���

class CDLLCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CDLLCheck)

public:
	CDLLCheck(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLLCheck();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DLL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int pid;
	virtual BOOL OnInitDialog();
	void InitDLLList();
	void ShowModule();
	CListCtrl m_List_DLL;
	afx_msg void OnBnClickedButtonUnloaddll();
	BOOL UnInjectDll(const TCHAR * ptszDllFile, DWORD dwProcessId);
};
