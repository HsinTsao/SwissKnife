#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;


//��������Ϣ�ṹ��
typedef struct _BOOTINFO
{
	TCHAR szBootName[50];			//������
	TCHAR szBootOrder[255];			//����
	TCHAR szHKEY[50];				//������Ϣ
	TCHAR szBootPos[255];			//ɾ��ʱҪ�õ���
	TCHAR szBootPos_All[255];		//List��Ҫ��ʾ��
}BOOTINFO, *PBOOTINFO;

// CChild_StartUpDlg �Ի���

class CChild_StartUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_StartUpDlg)

public:
	CChild_StartUpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild_StartUpDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STARTUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//������������Ϣ
	vector<BOOTINFO> m_vecBootInfo;
	//��ѡ������
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
