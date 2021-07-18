// Child_SoftwareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_SoftwareDlg.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
using namespace std;

// CChild_SoftwareDlg �Ի���

IMPLEMENT_DYNAMIC(CChild_SoftwareDlg, CDialogEx)

typedef struct _SOFTWAREINFO
{
	string displayName;    //�������
	string publisher;	   //������
	string displayVersion; //�汾
	string installDate;    //��װ����
	string uninstallString;//ж���������
	string strSoftInsPath;		//�����װ·��

}SoftWareInfo, *pSoftWareInfo;

vector<_SOFTWAREINFO> m_softwareInfo;

CChild_SoftwareDlg::CChild_SoftwareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SOFTWARE, pParent)
{

}

CChild_SoftwareDlg::~CChild_SoftwareDlg()
{
}

void CChild_SoftwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOFT, m_ListSoft);
}


BEGIN_MESSAGE_MAP(CChild_SoftwareDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SOFT, &CChild_SoftwareDlg::OnLvnItemchangedListSoft)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SOFT, &CChild_SoftwareDlg::OnRclickListSoft)
	ON_COMMAND(ID_SOFT_UNLOAD, &CChild_SoftwareDlg::OnSoftUnload)
	ON_COMMAND(ID_SPFTWARE_REFLASH, &CChild_SoftwareDlg::OnSpftwareReflash)
	ON_COMMAND(ID_SOFETWARE_OPEN, &CChild_SoftwareDlg::OnSofetwareOpen)
END_MESSAGE_MAP()


// CChild_SoftwareDlg ��Ϣ�������



BOOL CChild_SoftwareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListSoft.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListSoft.InsertColumn(0, "���", NULL, 50);
	m_ListSoft.InsertColumn(1, "�������", NULL, 300);
	m_ListSoft.InsertColumn(2, "������", NULL, 200);
	m_ListSoft.InsertColumn(3, "�汾��Ϣ", NULL, 200);
	m_ListSoft.InsertColumn(4, "��װʱ��", NULL, 200);
	m_ListSoft.InsertColumn(5, "ж�ز���������", NULL, 200);
	ShowList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CChild_SoftwareDlg::ShowList()
{
	StupSoftwareInfo();
	for (int i = 0; i < m_softwareInfo.size(); i++)
	{
		CString v1;
		v1.Format("%d", i);
		m_ListSoft.InsertItem(i, v1);
		m_ListSoft.SetItemText(i, 1, m_softwareInfo[i].displayName.data());
		m_ListSoft.SetItemText(i, 2, m_softwareInfo[i].publisher.data());
		m_ListSoft.SetItemText(i, 3, m_softwareInfo[i].displayVersion.data());
		m_ListSoft.SetItemText(i, 4, m_softwareInfo[i].installDate.data());
		m_ListSoft.SetItemText(i, 5, m_softwareInfo[i].uninstallString.data());
	}
	return TRUE;
}





BOOL CChild_SoftwareDlg::StupSoftwareInfo()
{
	const int  MAX_LEG = 256 * sizeof(TCHAR);
	HKEY hKey;
	DWORD dwRegNum = MAX_LEG;
	TCHAR regBufferName[MAX_LEG] = { 0 };
	SoftWareInfo info ;
	m_softwareInfo.clear();
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),
		NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwRegNum, NULL, NULL, NULL,
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (int i = 0; i < dwRegNum; i++)
			{
				DWORD dwRegSize = MAX_LEG;
				RegEnumKeyEx(hKey, i, regBufferName, &dwRegSize, NULL, NULL,
					NULL, NULL);
				DWORD dwType;
				HKEY hSubKey;
				if (RegOpenKeyEx(hKey, regBufferName, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					
					TCHAR regBufferValue[MAX_LEG] = { 0 };
					dwRegSize = MAX_LEG;
					RegQueryValueEx(hSubKey, _T("DisplayName"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.displayName = regBufferValue; //�������
				
					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("Publisher"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.publisher = regBufferValue;  //������

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("DisplayVersion"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.displayVersion = regBufferValue; //�汾

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					// �ж��Ƿ�����ע����л�ȡ����װʱ��, ��ȡ�����ʱ��
					if (RegQueryValueEx(hSubKey, _T("InstallDate"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize) == ERROR_SUCCESS)
					{
						info.installDate = regBufferValue;
					}
					else
					{
						FILETIME fileLastTime;
						RegQueryInfoKey(hSubKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
							NULL, NULL, NULL, &fileLastTime);
						SYSTEMTIME sTime, stLocal;
						FileTimeToSystemTime(&fileLastTime, &sTime);
						SystemTimeToTzSpecificLocalTime(NULL, &sTime, &stLocal);
						TCHAR tchTime[MAX_LEG] = { 0 };
						sprintf(tchTime, "%d-%02d-%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay);
						info.installDate = tchTime;
					}
					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("UninstallString"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.uninstallString = regBufferValue;//ж���������

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("InstallLocation"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.strSoftInsPath = regBufferValue;//��װ·��

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					// ȡParentKeyName��ֵ,�ж��Ƿ��ǲ�����Ϣ, �ǲ�����Ϣ��ֵΪ"OperatingSystem"
					RegQueryValueEx(hSubKey, _T("ParentKeyName"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					TCHAR tchLocal[MAX_LEG] = { 0 };
					_tcscpy(tchLocal, regBufferValue);

					_tcscpy(regBufferValue, "");
					dwRegSize = MAX_LEG;
					RegQueryValueEx(hSubKey, _T("QuietUninstallString"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					if (_tcscmp(regBufferValue, _T("")))
					{
						info.displayName = regBufferName;
						info.uninstallString = regBufferValue;
					}
					if ((info.displayName != _T(""))
						&& (_tcscmp(tchLocal, _T("OperatingSystem")))) //�ж��Ƿ��ǲ�����Ϣ 
					{
						m_softwareInfo.push_back(info);
					}

				}
			}//end for(; ;)
		}
	}
	else
	{
		return FALSE; //�򿪼�ʧ��
	}
	RegCloseKey(hKey);

	return TRUE;
}


void CChild_SoftwareDlg::OnLvnItemchangedListSoft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CChild_SoftwareDlg::OnRclickListSoft(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//��ֹ�ڿհ�����������˵�  
	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ�����ж������
		strTemp = m_ListSoft.GetItemText(n, 1);
		for (DWORD i = 0; i < m_softwareInfo.size(); i++)
		{
			if (strTemp == m_softwareInfo[i].displayName.data())
			{
				m_Select = i;
				break;
			}
		}
		strTemp.Empty();
		//�������δ���, ��������Ӧ��ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU_SOFTWARE);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}

	*pResult = 0;
}


void CChild_SoftwareDlg::OnSoftUnload()
{
	// TODO: �ڴ���������������
	if (m_Select == -1)
	{
		return;
	}
	ShellExecute(NULL, _T("open"),
		m_softwareInfo[m_Select].uninstallString.data(), NULL, NULL, SW_SHOWNORMAL);
	OnSpftwareReflash();
	return;
}


void CChild_SoftwareDlg::OnSpftwareReflash()
{
	// TODO: �ڴ���������������
	m_ListSoft.DeleteAllItems();
	ShowList();

}

BOOL CChild_SoftwareDlg::OpenFolderAndSelectFile(TCHAR* szPath)
{
	LPSHELLFOLDER pDesktopFolder;
	WCHAR* Path = NULL;
	int a = strlen(szPath);

	DWORD num = MultiByteToWideChar(CP_ACP, 0, szPath, -1, NULL, 0);
	Path = (wchar_t*)malloc(num * sizeof(wchar_t));
	memset(Path, 0, num * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, szPath, -1, Path, num);

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		LPITEMIDLIST     pidl;
		ULONG             chEaten;
		ULONG             dwAttributes;
		HRESULT             hr;

		hr = pDesktopFolder->ParseDisplayName(
			NULL, NULL, Path, &chEaten, &pidl, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return FALSE;
		}
		LPCITEMIDLIST pidlFolder = pidl;

		CoInitialize(NULL);
		hr = SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);

		pDesktopFolder->Release();

		if (hr == S_OK)
		{
			return TRUE;
		}
	}
	return FALSE;
}


void CChild_SoftwareDlg::OnSofetwareOpen()
{
	// TODO: �ڴ���������������
	if (!m_softwareInfo[m_Select].strSoftInsPath[0])
	{
		return;
	}
	if (!OpenFolderAndSelectFile((TCHAR*)m_softwareInfo[m_Select].strSoftInsPath.data()))
	{
		MessageBox("���ļ�λ��ʧ�ܣ�");
		return;
	}

	
}
