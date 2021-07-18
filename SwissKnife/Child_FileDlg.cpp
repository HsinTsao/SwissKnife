// Child_FileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_FileDlg.h"
#include "afxdialogex.h"


// CChild_FileDlg �Ի���

IMPLEMENT_DYNAMIC(CChild_FileDlg, CDialogEx)

CChild_FileDlg::CChild_FileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILE, pParent)
	, g_FileFullPath(_T(""))
{
	SHFILEINFO	sfi;
	//��ͼ��
	HIMAGELIST ImageListHwnd;   //SDK
								// ����ϵͳͼ���б�
	ImageListHwnd = (HIMAGELIST)SHGetFileInfo
	(
		NULL,
		0,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_LARGEICON | SHGFI_SYSICONINDEX
	);
	m_CImageList_Large = CImageList::FromHandle(ImageListHwnd);   //CimageList*
																  //����ϵͳͼ���б�
	ImageListHwnd = (HIMAGELIST)SHGetFileInfo
	(
		NULL,
		0,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_SMALLICON | SHGFI_SYSICONINDEX
	);
	m_CImageList_Small = CImageList::FromHandle(ImageListHwnd);
}

CChild_FileDlg::~CChild_FileDlg()
{
}

void CChild_FileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE, g_FileFullPath);
	DDX_Control(pDX, IDC_TREE_FILE, m_Tree_File);
	DDX_Control(pDX, IDC_LIST_FILE, m_List_File);
}


BEGIN_MESSAGE_MAP(CChild_FileDlg, CDialogEx)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, &CChild_FileDlg::OnDblclkListFile)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_FILE, &CChild_FileDlg::OnItemexpandedTreeFile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CChild_FileDlg::OnSelchangedTreeFile)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CChild_FileDlg::OnBnClickedButtonFind)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &CChild_FileDlg::OnRclickListFile)
	ON_COMMAND(ID_FILEOPEN, &CChild_FileDlg::OnFileOpen)
	ON_COMMAND(ID_FILEDELETE, &CChild_FileDlg::OnFileDelete)
	ON_COMMAND(ID_Menu_RENAME, &CChild_FileDlg::OnRename)
	ON_COMMAND(ID_Encrypt, &CChild_FileDlg::OnEncrypt)
	ON_COMMAND(ID_Decrypt, &CChild_FileDlg::OnDecrypt)
END_MESSAGE_MAP()


// CChild_FileDlg ��Ϣ�������







BOOL CChild_FileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ImageListA.Create(32, 32, ILC_COLOR32 | ILC_MASK, 10, 30);     //����ͼ������CImageList����   
	m_ImageListB.Create(20, 20, ILC_COLOR32 | ILC_MASK, 0, 3);


	//m_ListCtrl.SetImageList(m_CImageList_Large, LVSIL_NORMAL);
	m_List_File.SetImageList(m_CImageList_Small, LVSIL_SMALL);

	//m_ListCtrl.SetImageList(&m_ImageListA, LVSIL_SMALL);  //Ϊ���οؼ�����ͼ������  
	HICON hIcon = theApp.LoadIcon(IDI_ICON_FILE);        //ͼ����  
	m_ImageListB.Add(hIcon);                          //ͼ����ӵ�ͼ������  


	m_Tree_File.SetImageList(&m_ImageListB, LVSIL_NORMAL);
	m_Tree_File.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	m_hRoot = m_Tree_File.InsertItem("�ҵĵ���", 0, 0);         //������ڵ�  
	GetLogicalDrives(m_hRoot);                      //�Զ��庯�� ��ȡ����  
	GetDriveDir(m_hRoot);                           //�Զ��庯�� ��ȡ��������  
	m_Tree_File.Expand(m_hRoot, TVE_EXPAND);              //չ�����۵������б� TVE_EXPANDչ���б�   
	InitListCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CChild_FileDlg::GetLogicalDrives(HTREEITEM hParent)
{
	//��ȡϵͳ�����������ַ�����Ϣ  
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //�������ܳ���  
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //��������  
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);                        //������С  
	while (szDriveString > 0)
	{
		m_Tree_File.InsertItem(pDriveStrings, hParent);       //�ڸ��ڵ�hParent������̷�  
		pDriveStrings += szDriveString + 1;             //pDriveStrings��C:\ D:\ E:\��  
		szDriveString = strlen(pDriveStrings);
	}
}

//��������:��ȡ�����̷������������ļ���
void CChild_FileDlg::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_Tree_File.GetChildItem(hParent);   //��ȡָ��λ���е�����
	while (hChild)
	{
		CString strText = m_Tree_File.GetItemText(hChild);  //�����б�����Ŀ����
		if (strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�
			strText += _T("\\");
		strText += "*.*";
		//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ
		CFileFind file;                                       //���屾���ļ�����
		BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //������һ���ļ�
			if (file.IsDirectory() && !file.IsDots())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."
				m_Tree_File.InsertItem(file.GetFileName(), hChild); //����̷�·������״�ļ���
		}
		GetDriveDir(hChild);                                  //�ݹ����
		hChild = m_Tree_File.GetNextItem(hChild, TVGN_NEXT);        //��ȡ���οؼ�TVGN_NEXT���ֵ���
	}
}


void CChild_FileDlg::InitListCtrl()
{
	m_List_File.InsertColumn(0, "����", LVCFMT_LEFT, 80);
	m_List_File.InsertColumn(1, "����", LVCFMT_LEFT, 80);
	m_List_File.InsertColumn(2, "�ļ�ϵͳ", LVCFMT_LEFT, 80);
	m_List_File.InsertColumn(3, "�ܴ�С", LVCFMT_LEFT, 80);
	m_List_File.InsertColumn(4, "���ÿռ�", LVCFMT_LEFT, 80);

	m_List_File.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EDITLABELS);

	FixedHostInformation();
}


void CChild_FileDlg::FixedHostInformation()
{
	m_List_File.DeleteAllItems();
	char	VolumeList[0x500] = { 0 };
	CHAR	*Travel = NULL;
	//���������·��
	GetLogicalDriveStrings(sizeof(VolumeList), (LPSTR)VolumeList);  //c:\.d:\.
	Travel = VolumeList;

	CHAR	FileSystem[MAX_PATH];  //ntfs  fat32
	unsigned __int64	HardDiskAmount = 0;   //HardDisk
	unsigned __int64	HardDiskFreeSpace = 0;
	unsigned long		HardDiskAmountMB = 0; // �ܴ�С
	unsigned long		HardDiskFreeSpaceMB = 0;   // ʣ��ռ�


	for (int i = 0; *Travel != '\0'; i++, Travel += lstrlen(Travel) + 1)
	{
		// �õ����������Ϣ
		memset(FileSystem, 0, sizeof(FileSystem));
		// �õ��ļ�ϵͳ��Ϣ����С
		GetVolumeInformation(Travel, NULL, 0, NULL, NULL, NULL, FileSystem, MAX_PATH);

		ULONG	FileSystemLength = lstrlen(FileSystem) + 1;
		if (GetDiskFreeSpaceEx(Travel, (PULARGE_INTEGER)&HardDiskFreeSpace, (PULARGE_INTEGER)&HardDiskAmount, NULL))
		{
			HardDiskAmountMB = HardDiskAmount / 1024 / 1024;
			HardDiskFreeSpaceMB = HardDiskFreeSpace / 1024 / 1024;
		}
		else
		{
			HardDiskAmountMB = 0;
			HardDiskFreeSpaceMB = 0;
		}


		int	iItem = m_List_File.InsertItem(i, Travel, GetHostIconIndex(Travel, GetFileAttributes(Travel)));    //���ϵͳ��ͼ��		

		m_List_File.SetItemData(iItem, 1);
		m_List_File.SetItemText(iItem, 2, FileSystem);

		SHFILEINFO	sfi;
		SHGetFileInfo(Travel, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
		m_List_File.SetItemText(iItem, 1, sfi.szTypeName);

		CString	v1;
		v1.Format("%10.1f GB", (float)HardDiskAmountMB / 1024);
		m_List_File.SetItemText(iItem, 3, v1);
		v1.Format("%10.1f GB", (float)HardDiskFreeSpaceMB / 1024);
		m_List_File.SetItemText(iItem, 4, v1);
	}


}


int	CChild_FileDlg::GetHostIconIndex(LPCTSTR Volume, DWORD FileAttributes)
{
	SHFILEINFO	sfi;
	if (FileAttributes == INVALID_FILE_ATTRIBUTES)
		FileAttributes = FILE_ATTRIBUTE_NORMAL;
	else
		FileAttributes |= FILE_ATTRIBUTE_NORMAL;

	SHGetFileInfo
	(
		Volume,
		FileAttributes,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
	);

	return sfi.iIcon;
}



void CChild_FileDlg::OnDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_List_File.GetSelectedCount() == 0 ||
		m_List_File.GetItemData(m_List_File.GetSelectionMark()) != 1)
		return;

	FixedHostFileList();

	*pResult = 0;
}



void CChild_FileDlg::FixedHostFileList(CString strDirectory)
{
	if (strDirectory.GetLength() == 0)
	{
		int	iItem = m_List_File.GetSelectionMark();

		// �����ѡ�еģ���Ŀ¼
		if (iItem != -1)
		{
			if (m_List_File.GetItemData(iItem) == 1)   //������������
			{
				strDirectory = m_List_File.GetItemText(iItem, 0);
			}
		}
		// ����Ͽ���õ�·��
		else
		{

			//m_CCombo_File_Server.GetWindowText(m_File_Server_Path);
		}
	}

	if (strDirectory == "..")
	{
		m_File_Server_Path = GetParentDirectory(m_File_Server_Path);

	}
	// ˢ�µ�ǰ��
	else if (strDirectory != ".")   //c:\  1
	{
		/*  c:\   */
		m_File_Server_Path += strDirectory;
		if (m_File_Server_Path.Right(1) != "\\")
		{
			m_File_Server_Path += "\\";
		}
	}

	if (m_File_Server_Path.GetLength() == 0)
	{
		FixedHostInformation();

		return;
	}


	g_FileFullPath = m_File_Server_Path;
	CWnd* pWnd = GetDlgItem(IDC_EDIT_FILE);
	pWnd->SetWindowText(g_FileFullPath);

	ShowSubList(m_File_Server_Path);


}

CString CChild_FileDlg::GetParentDirectory(CString strPath)
{
	CString	v1 = strPath;
	int iIndex = v1.ReverseFind('\\');
	if (iIndex == -1)
	{
		return v1;
	}
	CString v3 = v1.Left(iIndex);
	iIndex = v3.ReverseFind('\\');
	if (iIndex == -1)
	{
		v1 = "";
		return v1;
	}
	v1 = v3.Left(iIndex);

	if (v1.Right(1) != "\\")
		v1 += "\\";
	return v1;
}

void CChild_FileDlg::ShowSubList(CString m_File_Server_Path)
{
	m_List_File.DeleteAllItems();
	while (m_List_File.DeleteColumn(0) != 0);  //ɾ��
	m_List_File.InsertColumn(0, "����", LVCFMT_LEFT, 150);
	m_List_File.InsertColumn(1, "��С", LVCFMT_LEFT, 100);
	m_List_File.InsertColumn(2, "����", LVCFMT_LEFT, 100);
	m_List_File.InsertColumn(3, "�޸�����", LVCFMT_LEFT, 115);

	m_List_File.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	int			v10 = 0;

	m_List_File.SetItemData(m_List_File.InsertItem(v10++, "..",
		GetHostIconIndex(NULL, FILE_ATTRIBUTE_DIRECTORY)), 1);
	for (int i = 0; i < 2; i++) //0 �ļ���  1  �ļ�
	{
		CFileFind	FileFindObject;
		BOOL		bContinue;
		bContinue = FileFindObject.FindFile(m_File_Server_Path + "*.*");   //c:\*.*      //.. .  1.txt
		while (bContinue)
		{
			bContinue = FileFindObject.FindNextFile();
			if (FileFindObject.IsDots())
				continue;
			BOOL bIsInsert = !FileFindObject.IsDirectory() == i;

			if (!bIsInsert)
				continue;

			int iItem = m_List_File.InsertItem(v10++, FileFindObject.GetFileName(),
				GetHostIconIndex(FileFindObject.GetFileName(), GetFileAttributes(FileFindObject.GetFilePath())));
			m_List_File.SetItemData(iItem, FileFindObject.IsDirectory());
			SHFILEINFO	sfi;
			SHGetFileInfo(FileFindObject.GetFileName(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO),
				SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);

			if (FileFindObject.IsDirectory())
			{
				m_List_File.SetItemText(iItem, 2, "�ļ���");
			}
			else
			{
				m_List_File.SetItemText(iItem, 2, sfi.szTypeName);
			}
			CString v1;
			v1.Format("%10d KB", FileFindObject.GetLength() / 1024 + (FileFindObject.GetLength() % 1024 ? 1 : 0));
			m_List_File.SetItemText(iItem, 1, v1);
			CTime Time;
			FileFindObject.GetLastWriteTime(Time);
			m_List_File.SetItemText(iItem, 3, Time.Format("%Y-%m-%d %H:%M"));
		}
	}


}

void CChild_FileDlg::OnItemexpandedTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TVITEM item = pNMTreeView->itemNew;                  //����\���ܹ���������ͼ��Ŀ��Ϣ  
	if (item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_Tree_File.GetChildItem(item.hItem);  //��ȡָ��λ���е�����  
	while (hChild)
	{
		AddSubDir(hChild);                               //�����Ŀ¼  
		hChild = m_Tree_File.GetNextItem(hChild, TVGN_NEXT);   //��ȡ���οؼ�TVGN_NEXT���ֵ���  
	}
	*pResult = 0;
}

//��������:�����Ŀ¼  
void CChild_FileDlg::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);     //��ȡȫ·��  
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //���Ұ����ַ������ļ�  
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //������һ���ļ�  
		if (file.IsDirectory() && !file.IsDots())
			m_Tree_File.InsertItem(file.GetFileName(), hParent);
	}
}


//��������:��ȡ����Ŀȫ��·��
CString CChild_FileDlg::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = "";
	while (hCurrent != m_hRoot)
	{
		strTemp = m_Tree_File.GetItemText(hCurrent);    //�����б�����Ŀ����
		if (strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp + strReturn;
		hCurrent = m_Tree_File.GetParentItem(hCurrent); //���ظ���Ŀ���
	}
	return strReturn;
}


void CChild_FileDlg::OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_List_File.DeleteAllItems();
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == m_hRoot)
		return;
	m_File_Server_Path = GetFullPath(item.hItem);

	g_FileFullPath = m_File_Server_Path;
	CWnd* pWnd = GetDlgItem(IDC_EDIT_FILE);
	pWnd->SetWindowText(g_FileFullPath);

	ShowSubList(m_File_Server_Path);
	*pResult = 0;
}


void CChild_FileDlg::OnBnClickedButtonFind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Str;
	CWnd* pWnd = GetDlgItem(IDC_EDIT_FILE);
	pWnd->GetWindowText(Str);
	if(Str != "")
		ShowSubList(Str);
}


void CChild_FileDlg::OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ�����������

		m_Select = n;

		//�������δ���, ��������Ӧ��ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU_FILE);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}


void CChild_FileDlg::OnFileOpen()
{
	// TODO: �ڴ���������������
	CString	strFileFullPath;
	strFileFullPath = m_File_Server_Path + m_List_File.GetItemText(m_List_File.GetSelectionMark(), 0);
	ShellExecute(NULL, "open", strFileFullPath, NULL, NULL, SW_SHOW);   //CreateProcess 
}


void CChild_FileDlg::OnFileDelete()
{
	// TODO: �ڴ���������������
	CString strString;
	if (m_List_File.GetSelectedCount() > 1)
		strString.Format("ȷ��Ҫ���� %d ��ɾ����?", m_List_File.GetSelectedCount());
	else
	{
		CString strFileName = m_List_File.GetItemText(m_List_File.GetSelectionMark(), 0);

		int iItem = m_List_File.GetSelectionMark();   //.. fff 1  Hello

		if (iItem == -1)
		{
			return;
		}
		if (m_List_File.GetItemData(iItem) == 1)
		{
			strString.Format("ȷʵҪɾ���ļ��С�%s��������������ɾ����?", strFileName);
		}
		else
		{
			strString.Format("ȷʵҪ�ѡ�%s��ɾ����?", strFileName);
		}
	}
	if (::MessageBox(m_hWnd, strString, "ȷ��ɾ��", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}

	//EnableControl(FALSE);

	POSITION Pos = m_List_File.GetFirstSelectedItemPosition();
	while (Pos)
	{
		int iItem = m_List_File.GetNextSelectedItem(Pos);
		CString	strFileFullPath = m_File_Server_Path + m_List_File.GetItemText(iItem, 0);
		// �����Ŀ¼
		if (m_List_File.GetItemData(iItem))
		{
			strFileFullPath += '\\';
			DeleteDirectory(strFileFullPath);
		}
		else
		{
			DeleteFile(strFileFullPath);
		}
	}


	FixedHostFileList(".");
}


BOOL CChild_FileDlg::DeleteDirectory(LPCTSTR strDirectoryFullPath)
{
	WIN32_FIND_DATA	wfd;
	char	szBuffer[MAX_PATH] = { 0 };

	wsprintf(szBuffer, "%s\\*.*", strDirectoryFullPath);

	HANDLE hFind = FindFirstFile(szBuffer, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
		return FALSE;

	do
	{
		if (wfd.cFileName[0] != '.')
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char szv1[MAX_PATH];
				wsprintf(szv1, "%s\\%s", strDirectoryFullPath, wfd.cFileName);
				DeleteDirectory(szv1);
			}
			else
			{
				char szv2[MAX_PATH];
				wsprintf(szv2, "%s\\%s", strDirectoryFullPath, wfd.cFileName);
				DeleteFile(szv2);
			}
		}
	} while (FindNextFile(hFind, &wfd));

	FindClose(hFind); // �رղ��Ҿ��

	if (!RemoveDirectory(strDirectoryFullPath))
	{
		return FALSE;
	}
	return true;
}



void CChild_FileDlg::OnRename()
{
	// TODO: �ڴ���������������
	POINT Point;
	GetCursorPos(&Point);
	if (GetFocus()->m_hWnd == m_List_File.m_hWnd)
	{
		m_List_File.EditLabel(m_List_File.GetSelectionMark());
	}

}


void CChild_FileDlg::OnEncrypt()
{
	// TODO: �ڴ���������������
	CString	strFileFullPath;
	strFileFullPath = m_File_Server_Path + m_List_File.GetItemText(m_List_File.GetSelectionMark(), 0);

	POSITION Pos = m_List_File.GetFirstSelectedItemPosition();
	int iItem = m_List_File.GetNextSelectedItem(Pos);
	BOOL bOk = m_List_File.GetItemData(iItem);
	if (bOk)
	{
		MessageBox("��ѡ������ļ���");
		return;
	}


	m_strSourFilePath = strFileFullPath;
	m_strSourFileName = m_List_File.GetItemText(m_List_File.GetSelectionMark(), 0);

	m_strEncryptFileName = "�Ѽ���_" + m_strSourFileName;
	int n = m_strSourFilePath.ReverseFind('\\') + 1;
	m_strEncryptFilePath = m_strSourFilePath.Left(n);
	m_strEncryptFilePath = m_strEncryptFilePath + m_strEncryptFileName;


	m_strEncryptFileType = ".sm4";
	m_strEncryptFilePath = m_strEncryptFilePath + m_strEncryptFileType;


	FILE* In;
	FILE* Out;

	WORD flag1, flag2;

	/*int SrcLen = WideCharToMultiByte(CP_ACP, 0, m_strSourFilePath, -1, NULL, 0, NULL, NULL);
	char *SrcPath = new char[SrcLen + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strSourFilePath, -1, SrcPath, SrcLen, NULL, NULL);

	int EncryptLen = WideCharToMultiByte(CP_ACP, 0, m_strEncryptFilePath, -1, NULL, 0, NULL, NULL);
	char *EncryptPath = new char[EncryptLen + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strEncryptFilePath, -1, EncryptPath, EncryptLen, NULL, NULL);
*/
	char *SrcPath = new char[m_strSourFilePath.GetLength() + 1];
	SrcPath = m_strSourFilePath.GetBuffer();
	In = fopen(SrcPath, "rb");
	if (In == NULL)
	{
		fclose(In);
		return;
	}

	Out = fopen(m_strEncryptFilePath.GetBuffer(), "wb");
	if (Out == NULL)
	{
		fclose(Out);
		return;
	}


	int last;
	int act;

	fseek(In, 0, SEEK_END);
	long soulen = ftell(In);
	fseek(In, 0, SEEK_SET);

	last = soulen;
	clock_t start = clock();
	while (last > 0)
	{
		act = min(128, last);
		memset(SrcBuffer, 0, 128);
		fread(SrcBuffer, act, 1, In);
		last -= act;
		if (act == 16)
			flag1 = 0;          //16�ֽ�
		else
			flag1 = act;        //����16�ֽ� 

								//������Կ
		sm4_setkey_enc(&ctx, key);
		//����
		sm4_crypt_ecb(&ctx, 1, 128, SrcBuffer, DestBuffer); //1 Ϊ���� 0Ϊ����

		fwrite(DestBuffer, 128, 1, Out);
		int d = GetLastError();

	}
	clock_t end = clock();
	long time = (end - start);
	CString strtime;
	strtime.Format("%d", time);
	strtime = "���ܳɹ�!\r����ʱ��:" + strtime + "ms";
	fclose(In);
	fclose(Out);

	FixedHostFileList(".");
	MessageBox(strtime);
}


void CChild_FileDlg::OnDecrypt()
{
	CString	strFileFullPath;
	strFileFullPath = m_File_Server_Path + m_List_File.GetItemText(m_List_File.GetSelectionMark(), 0);

	POSITION Pos = m_List_File.GetFirstSelectedItemPosition();
	int iItem = m_List_File.GetNextSelectedItem(Pos);
	BOOL bOk = m_List_File.GetItemData(iItem);
	if (bOk)
	{
		MessageBox("��ѡ������ļ���");
		return;
	}

	m_strSourFilePath = strFileFullPath;
	m_strSourFileName = m_List_File.GetItemText(m_List_File.GetSelectionMark(), 0);

	m_strDecryptFileName = "�ѽ���_" + m_strSourFileName;
	int n = m_strSourFilePath.ReverseFind('\\') + 1;
	m_strDecryptFilePath = m_strSourFilePath.Left(n);
	m_strDecryptFilePath = m_strDecryptFilePath + m_strDecryptFileName;

	int v1 = m_strDecryptFilePath.GetLength() - m_strDecryptFilePath.ReverseFind('.') - 1;
	m_strEncryptFileType = m_strDecryptFilePath.Right(v1);

	int v2 = m_strDecryptFilePath.ReverseFind('.');
	m_strDecryptFilePath = m_strDecryptFilePath.Left(v2);

	if (m_strEncryptFileType != "sm4")
	{
		MessageBox("��ѡ��sm4�ļ����н��ܣ�");
		return;
	}

	FILE* In;
	FILE* Out;

	WORD flag1, flag2;

	/*int SrcLen = WideCharToMultiByte(CP_ACP, 0, m_strSourFilePath, -1, NULL, 0, NULL, NULL);
	char *SrcPath = new char[SrcLen + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strSourFilePath, -1, SrcPath, SrcLen, NULL, NULL);

	int DecryptLen = WideCharToMultiByte(CP_ACP, 0, m_strDecryptFilePath, -1, NULL, 0, NULL, NULL);
	char *DecryptPath = new char[DecryptLen + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strDecryptFilePath, -1, DecryptPath, DecryptLen, NULL, NULL);*/

	In = fopen(m_strSourFilePath, "rb");
	if (In == NULL)
	{
		fclose(In);
		return;
	}

	Out = fopen(m_strDecryptFilePath, "wb");
	if (Out == NULL)
	{
		fclose(Out);
		return;
	}


	fseek(In, 0, SEEK_END);
	long clen = ftell(In);
	fseek(In, 0, SEEK_SET);

	int last = clen;
	int act = 128;
	clock_t start = clock();
	while (last > 0)
	{
		memset(SrcBuffer, 0, 128);
		fread(SrcBuffer, act, 1, In);

		sm4_setkey_dec(&ctx, key);
		sm4_crypt_ecb(&ctx, 0, 128, SrcBuffer, DestBuffer);

		fwrite(DestBuffer, 128, 1, Out);

		last -= act;
	}
	clock_t end = clock();
	long time = (end - start);
	CString strtime;
	strtime.Format("%d", time);
	strtime = "���ܳɹ�!\r����ʱ��:" + strtime + "ms";
	fclose(In);
	fclose(Out);
	FixedHostFileList(".");
	MessageBox(strtime);
	// TODO: �ڴ���������������
}
