#pragma once

#define TOKEN_REG_PATH 1001
#define TOKEN_REG_KEY  1002
// CRegisterOperation �Ի���

class CRegisterOperation : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterOperation)

public:
	CRegisterOperation(char bToken, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterOperation();
	HKEY MKEY;
	char KeyPath[MAX_PATH];
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	char * FindPath();

	void SetPath(char * szPath);

	char * FindKey();

	DECLARE_MESSAGE_MAP()
};
