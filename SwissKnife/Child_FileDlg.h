#pragma once
#include "afxcmn.h"
#include "_sm4.h"

// CChild_FileDlg 对话框

class CChild_FileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_FileDlg)

public:
	CChild_FileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild_FileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CImageList* m_ImageList;  //图像列表(相同大小图像集合) 
	CImageList m_ImageListA;
	CImageList m_ImageListB;
	CImageList* m_CImageList_Large;
	CImageList* m_CImageList_Small;

	HTREEITEM m_hRoot;       //句柄 CTreeCtrl的根结点"我的电脑" 
	CString g_FileFullPath;
	CString m_File_Server_Path;  //父路径
	CTreeCtrl m_Tree_File;
	CListCtrl m_List_File;

	sm4_context ctx;
	unsigned char key[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	unsigned char  SrcBuffer[128];
	unsigned char  DestBuffer[128];

	CString m_strSourFilePath;
	CString m_strEncryptFilePath;
	CString m_strDecryptFilePath;
	CString m_strSourFileName;
	CString m_strEncryptFileName;
	CString m_strDecryptFileName;
	CString m_strEncryptFileType;

	virtual BOOL OnInitDialog();
	//树获得磁盘信息
	void GetLogicalDrives(HTREEITEM hParent);
	//树获得驱动器列表
	void GetDriveDir(HTREEITEM hParent);
	//树展开
	afx_msg void OnItemexpandedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	void AddSubDir(HTREEITEM hParent);
	CString GetFullPath(HTREEITEM hCurrent);
	afx_msg void OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);

	void InitListCtrl();
	//list获得本机磁盘信息
	void FixedHostInformation();
	//list获得文件图标
	int GetHostIconIndex(LPCTSTR Volume, DWORD FileAttributes);
	//双击list
	afx_msg void OnDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult);
	//list获得文件列表
	void FixedHostFileList(CString strDirectory = "");
	//获得父路径
	CString GetParentDirectory(CString strPath);
	//显示子目录
	void ShowSubList(CString m_File_Server_Path);
	int m_Select = 0;
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileOpen();
	afx_msg void OnFileDelete();
	BOOL DeleteDirectory(LPCTSTR strDirectoryFullPath);
	afx_msg void OnRename();
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
};
