#pragma once
#include "afxcmn.h"
#include <winsvc.h>
#include <vector>
using namespace std;
// CChild_ServerDlg �Ի���

#define UPDATE_SERVICE_INFO		WM_USER+3			//���·�����Ϣ��Ϣ��

class CChild_ServerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_ServerDlg)

public:
	CChild_ServerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild_ServerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//��ѡ���������
	CString m_ServiceName;
	//�Ƿ���Ҫ���½���
	BOOL m_IsUpDateSer;
	//���´���
	DWORD m_UpDataTimes;
	//��ѡ������List�е����
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
