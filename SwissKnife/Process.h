#pragma once
#include "afxcmn.h"


// CProcess �Ի���

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcess();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SWISSKNIFE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	
};
