
// SwissKnifeDlg.h : ͷ�ļ�
//

#pragma once



#include "Child_ProcessDlg.h"
#include "Child_FileDlg.h"
#include "��չ\TrueColorToolBar.h" 
#include "Child_RegisterDlg.h"
#include "Child_SoftwareDlg.h"
#include "Child_CPUDlg.h"
#include "Child_ServerDlg.h"
#include "Child_StartUpDlg.h"
#include "Child_WindowDlg.h"
#include "HostInfo.h"

#include "afxcmn.h"
// CSwissKnifeDlg �Ի���
class CSwissKnifeDlg : public CDialogEx
{
// ����
public:
	CSwissKnifeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SWISSKNIFE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CTrueColorToolBar m_ToolBar;

	CChild_ProcessDlg  m_ChildProcessDlg;
	CChild_FileDlg     m_ChildFileDlg;
	CChild_RegisterDlg m_ChildRegisterDlg;
	CChild_SoftwareDlg m_ChildSoftwareDlg;
	CChild_CPUDlg      m_ChildCPUDlg;
	CChild_ServerDlg   m_ChildServerDlg;
	CChild_StartUpDlg  m_ChildStartUpDlg;
	CChild_WindowDlg   m_ChildWindowDlg;

	CBrush m_brs;

	void CreateToolBar();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��ʼ���ؼ�
	void InitControl();

	afx_msg void OnButtonProcess();
	afx_msg void OnButtonHost();
	afx_msg void OnButtonFile();
	afx_msg void OnButtonReg();
	afx_msg void OnButtonSoft();
	afx_msg void OnButtonServer();
	afx_msg void OnButtonStartup();
	afx_msg void OnButtonWindow();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	static DWORD WINAPI MonitorThread(LPVOID lpParam);


	


};


