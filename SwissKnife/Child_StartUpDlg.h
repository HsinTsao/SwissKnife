#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;


//启动项信息结构体
typedef struct _BOOTINFO
{
	TCHAR szBootName[50];			//启动项
	TCHAR szBootOrder[255];			//命令
	TCHAR szHKEY[50];				//主键信息
	TCHAR szBootPos[255];			//删除时要用到的
	TCHAR szBootPos_All[255];		//List中要显示的
}BOOTINFO, *PBOOTINFO;

// CChild_StartUpDlg 对话框

class CChild_StartUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_StartUpDlg)

public:
	CChild_StartUpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild_StartUpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STARTUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//保存软件相关信息
	vector<BOOTINFO> m_vecBootInfo;
	//所选则的软件
	int m_Select;
	CString strTemp;

	CListCtrl m_List_StartUp;
	virtual BOOL OnInitDialog();
	void InitControl();
	void GetBootInfo();
	void AddList(TCHAR szBootName[50], TCHAR szBootOrder[255], TCHAR szBootPos_All[255]);
	afx_msg void OnRclickListStartup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddBootItem();
	afx_msg void OnDelete();
	afx_msg void OnRefalsh();
};
