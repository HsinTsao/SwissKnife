#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <TLHELP32.H>
#include <psapi.h>
#include "DLLCheck.h"
#include <vector>
using namespace std;
// CChild_Process 对话框

typedef struct _PROCESS
{
	DWORD PID;
	CString ProcessName;
	CString ProcessLocation;

}PROCESS, *PPROCESS;

class CChild_ProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_ProcessDlg)

public:
	CChild_ProcessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild_ProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int m_Count = 0;
	int m_Select;
	int pid;
	vector<PROCESS> m_vecProcessInfo;
	CListCtrl m_List_Process;

	virtual BOOL OnInitDialog();
	void InitControl();
	void AddList(CString strProcessName, CString PID, CString Phandle, CString strFullPath, CString strCorporation);
	void ShowProcess();
	BOOL OpenFolderAndSelectFile(TCHAR * szPath);

	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnRclickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuKill();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnStop();
	afx_msg void OnRemuse();
	afx_msg void OnDll();
	afx_msg void OnProcessopen();
	
	
};
