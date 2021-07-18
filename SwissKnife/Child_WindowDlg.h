#pragma once
#include "afxcmn.h"

#include <vector>
using namespace std;
// CChild_WindowDlg 对话框
typedef struct _WINDOWINFO1
{
	CString WindowHandle;			
	CString szWindowTitle;			
	CString strState;
}WINDOWINFO1, *PWINDOWINFO1;

class CChild_WindowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_WindowDlg)

public:
	CChild_WindowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild_WindowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	vector<WINDOWINFO1> m_vecWindowInfo;
	int m_Select;
	CListCtrl m_List_Window;

	virtual BOOL OnInitDialog();
	void InitControl();
	LPBYTE GetWindowsList();
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	void ShowWindowsList(LPBYTE Buffer);
	void AddList(CString hwnd, CString name, CString state);
	
	afx_msg void OnRefresh();
	afx_msg void OnRclickListWindow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCloseWindow();
	afx_msg void OnShowWindow();
	afx_msg void OnHideWindow();
	afx_msg void OnMax();
	afx_msg void OnMin();
};