#pragma once
#include "afxcmn.h"
#include "HostInfo.h"

// CChild_CPUDlg �Ի���
#define maxpix 200
class CChild_CPUDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_CPUDlg)

public:
	CChild_CPUDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild_CPUDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	
	static UINT DoSysCpu(LPVOID pParam);
	static UINT DoSysMem(LPVOID pParam);
	UINT m_cpuUse;
	UINT m_memUse;

	HBRUSH m_hPage2Face;
	POINT pointCpu[maxpix + 1];//���ڱ���Ҫ��������
	POINT pointMem[maxpix + 1];

	int m_nMovNum;//��ʾ�������Ƿ�ﵽmaxpix
	CBrush m_brs;

	CProgressCtrl m_ProcessCPU;
	CProgressCtrl m_ProcessMEM;
	afx_msg void OnPaint();
	BOOL CDrawCpu(CDC * pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_szStatus;
	afx_msg void OnBnClickedButton1();
	void ClearMemory();
};
