#pragma once
#include "afxcmn.h"
#include "Child_ProcessDlg.h"

// CDLLCheck 对话框

class CDLLCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CDLLCheck)

public:
	CDLLCheck(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLLCheck();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DLL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
