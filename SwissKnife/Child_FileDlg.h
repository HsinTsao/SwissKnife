#pragma once
#include "afxcmn.h"
#include "_sm4.h"

// CChild_FileDlg �Ի���

class CChild_FileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChild_FileDlg)

public:
	CChild_FileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild_FileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CImageList* m_ImageList;  //ͼ���б�(��ͬ��Сͼ�񼯺�) 
	CImageList m_ImageListA;
	CImageList m_ImageListB;
	CImageList* m_CImageList_Large;
	CImageList* m_CImageList_Small;

	HTREEITEM m_hRoot;       //��� CTreeCtrl�ĸ����"�ҵĵ���" 
	CString g_FileFullPath;
	CString m_File_Server_Path;  //��·��
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
	//����ô�����Ϣ
	void GetLogicalDrives(HTREEITEM hParent);
	//������������б�
	void GetDriveDir(HTREEITEM hParent);
	//��չ��
	afx_msg void OnItemexpandedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	void AddSubDir(HTREEITEM hParent);
	CString GetFullPath(HTREEITEM hCurrent);
	afx_msg void OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);

	void InitListCtrl();
	//list��ñ���������Ϣ
	void FixedHostInformation();
	//list����ļ�ͼ��
	int GetHostIconIndex(LPCTSTR Volume, DWORD FileAttributes);
	//˫��list
	afx_msg void OnDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult);
	//list����ļ��б�
	void FixedHostFileList(CString strDirectory = "");
	//��ø�·��
	CString GetParentDirectory(CString strPath);
	//��ʾ��Ŀ¼
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
