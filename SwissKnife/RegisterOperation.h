#pragma once

#define TOKEN_REG_PATH 1001
#define TOKEN_REG_KEY  1002
// CRegisterOperation 对话框

class CRegisterOperation : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterOperation)

public:
	CRegisterOperation(char bToken, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterOperation();
	HKEY MKEY;
	char KeyPath[MAX_PATH];
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	char * FindPath();

	void SetPath(char * szPath);

	char * FindKey();

	DECLARE_MESSAGE_MAP()
};
