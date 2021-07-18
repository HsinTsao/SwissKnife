// Child_StartUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_StartUpDlg.h"
#include "afxdialogex.h"


// CChild_StartUpDlg �Ի���

IMPLEMENT_DYNAMIC(CChild_StartUpDlg, CDialogEx)


enum
{
	BOOTNAME = 0,                  // ��Ŀ     
	BOOTORDER,                     // ����
	BOOTLOCATION,                   // λ��
};

typedef struct
{
	char*   TitleData;             //�б������
	int		TitleWidth;            //�б�Ŀ��
}STARTUP_DATA;

STARTUP_DATA g_StartUp_Data_Show[] =
{
	{ "������",	        300 },
	{ "����",			300 },
	{ "λ��",	        330 },
};
int  g_StartUp_Count_Show = 3;


CChild_StartUpDlg::CChild_StartUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_STARTUP, pParent)
{

}

CChild_StartUpDlg::~CChild_StartUpDlg()
{
}

void CChild_StartUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STARTUP, m_List_StartUp);
}


BEGIN_MESSAGE_MAP(CChild_StartUpDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STARTUP, &CChild_StartUpDlg::OnRclickListStartup)
	ON_COMMAND(ID_ADD, &CChild_StartUpDlg::OnAddBootItem)
	ON_COMMAND(ID_DELETE, &CChild_StartUpDlg::OnDelete)
	ON_COMMAND(ID_REFALSH, &CChild_StartUpDlg::OnRefalsh)
END_MESSAGE_MAP()


// CChild_StartUpDlg ��Ϣ�������


BOOL CChild_StartUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitControl();
	GetBootInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CChild_StartUpDlg::InitControl()
{
	for (int i = 0; i < g_StartUp_Count_Show; i++)
	{
		m_List_StartUp.InsertColumn(i, g_StartUp_Data_Show[i].TitleData,
			LVCFMT_LEFT, g_StartUp_Data_Show[i].TitleWidth);
	}
	m_List_StartUp.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}


void CChild_StartUpDlg::GetBootInfo()
{
	m_List_StartUp.DeleteAllItems();
	m_vecBootInfo.clear();
	BOOTINFO stcBootInfo = { 0 };

	HKEY RootKey = NULL;						// ����
	HKEY hkResult = NULL;						// ��Ҫ�򿪼��ľ�� 
	LONG lReturn;						// ��¼��ȡע����Ƿ�ɹ�
	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };       // ע���������
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;
	CString buffer;
	LPCTSTR lpSubKey[2];				// �Ӽ�����
	DWORD ListIndex = 0;

	lpSubKey[0] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	lpSubKey[1] = _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");

	for (DWORD i = 0; i < 2; i++)
	{
		if (i == 0)
			RootKey = HKEY_CURRENT_USER;
		if (i == 1)
			RootKey = HKEY_LOCAL_MACHINE;

		//��lpSubkey
		lReturn = RegOpenKeyEx(RootKey, lpSubKey[i], 0,
			KEY_ALL_ACCESS, &hkResult);
		if (lReturn == ERROR_SUCCESS)
		{
			//����򿪳ɹ�����ʼö����ֵ
			while (ERROR_NO_MORE_ITEMS !=
				RegEnumValue(hkResult, index, stcBootInfo.szBootName, &dwKeyLen,
					0, NULL, NULL, NULL))
			{
				//��ȡÿ��ֵ������ szKeyNameΪֵ������ bufferΪֵ������
				RegQueryValueEx(hkResult, stcBootInfo.szBootName,
					0, &dwType, (LPBYTE)stcBootInfo.szBootOrder, &dwNameLen);
				//������Ϣ
				//������	stcBootInfo.szBootName
				//����	stcBootInfo.szBootOrder

				//����λ����Ϣ
				//������Ϣ
				stcBootInfo.szHKEY;
				if (i == 0)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_CURRENT_USER"));
				if (i == 1)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_LOCAL_MACHINE"));
				//ɾ��ʱҪ�õ���
				_tcscpy_s(stcBootInfo.szBootPos, lpSubKey[i]);
				//List��Ҫ��ʾ��
				buffer.Empty();
				buffer = buffer + stcBootInfo.szHKEY + _T("\\") + stcBootInfo.szBootPos;
				_tcscpy_s(stcBootInfo.szBootPos_All, buffer);


				//��ӵ�vector
				m_vecBootInfo.push_back(stcBootInfo);
				//����List
				AddList(stcBootInfo.szBootName,
					stcBootInfo.szBootOrder,
					stcBootInfo.szBootPos_All);
				//
				index++;
				dwNameLen = 255;
				dwKeyLen = 255;
				memset(&stcBootInfo, 0, sizeof(BOOTINFO));
			}
			index = 0;
			RegCloseKey(hkResult);
		}
	}
}


void CChild_StartUpDlg::AddList(TCHAR szBootName[50],TCHAR szBootOrder[255],TCHAR szBootPos_All[255])		//List��Ҫ��ʾ��)
{
	
	//Ĭ��Ϊ0��  �������в�������ж���������
	int i = m_List_StartUp.InsertItem(m_List_StartUp.GetItemCount(), szBootName);

	m_List_StartUp.SetItemText(i, BOOTORDER, szBootOrder);

	m_List_StartUp.SetItemText(i, BOOTLOCATION, szBootPos_All);

}


void CChild_StartUpDlg::OnRclickListStartup(NMHDR *pNMHDR, LRESULT *pResult)
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
		menu.LoadMenu(IDR_MENU_BOOT);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}


void CChild_StartUpDlg::OnAddBootItem()
{
	// TODO: �ڴ���������������
	//���ļ�����
	TCHAR FilePathName[MAX_PATH] = { 0 };
	CString FileName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		//�ļ�����������FilePathName��
		_tcscpy_s(FilePathName, dlg.GetPathName());
	}
	else
	{
		return;
	}

	//��������Key
	HKEY hKey;
	LPCTSTR lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//��ȡ·������
		DWORD dwRet = _tcslen(FilePathName);

		//�����������Ϣ
		FileName = dlg.GetFileTitle();
		int n = sizeof(TCHAR);
		lRet = RegSetValueEx(hKey, FileName, 0, REG_SZ,
			(BYTE *)FilePathName, dwRet * sizeof(TCHAR));

		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("���������ʧ��!"));
		}
		else
		{
			AfxMessageBox(_T("���������ɹ�!"));
			GetBootInfo();
		}
	}
}


void CChild_StartUpDlg::OnDelete()
{
	// TODO: �ڴ���������������
	if (m_Select == -1)
	{
		return;
	}
	HKEY RootKey = NULL;		//����
	HKEY hKey;
	//ȷ��Ҫɾ���ĸ���
	int a = m_vecBootInfo.size();
	strTemp = m_vecBootInfo[m_Select].szHKEY;
	if (strTemp == _T("HKEY_CURRENT_USER"))
	{
		RootKey = HKEY_CURRENT_USER;
	}
	if (strTemp == _T("HKEY_LOCAL_MACHINE"))
	{
		RootKey = HKEY_LOCAL_MACHINE;
	}
	if (RootKey == NULL)
	{
		return;
	}
	//ȷ��Ҫɾ����Run�Ӽ�·��
	LPCTSTR lpRun = m_vecBootInfo[m_Select].szBootPos;
	//��������Key   
	long lRet = RegOpenKeyEx(RootKey, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//��ȡҪɾ�������������
		strTemp = m_vecBootInfo[m_Select].szBootName;
		//ɾ���������� 
		lRet = RegDeleteValue(hKey, strTemp);
		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("ɾ��������ɹ���"));
		}
		else {
			AfxMessageBox(_T("ɾ��������ɹ���"));
			GetBootInfo();
		}
	}
}


void CChild_StartUpDlg::OnRefalsh()
{
	// TODO: �ڴ���������������
	GetBootInfo();
}
