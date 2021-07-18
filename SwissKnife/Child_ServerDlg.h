#pragma once
#include "afxcmn.h"
#include <winsvc.h>
#include <vector>
using namespace std;
// CChild_ServerDlg 对话框

#define UPDATE_SERVICE_INFO		WM_USER+3			//更新服务信息消息宏

class CChild_ServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_ServerDlg)

public:
	CChild_ServerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild_ServerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//所选服务的名字
	CString m_ServiceName;
	//是否需要更新进程
	BOOL m_IsUpDateSer;
	//更新次数
	DWORD m_UpDataTimes;
	//所选服务在List中的序号
	DWORD m_SelectNum;
	vector<ENUM_SERVICE_STATUS> m_vecSerInfo;

	virtual BOOL OnInitDialog();
	void InitControl();
	void GetServiceInfo();
	void AddList(LPSTR lpServiceName, LPSTR lpDisplayName, DWORD ServiceStatus, DWORD dwStartType, DWORD dwServiceType);
	void UpDataSerInfo();
	CListCtrl m_List_Server;
	afx_msg void OnOpenservice();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShutDownService();
	afx_msg void OnReflash();
	afx_msg void OnRclickListServer(NMHDR *pNMHDR, LRESULT *pResult);
};
