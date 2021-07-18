// Child_SoftwareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_SoftwareDlg.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
using namespace std;

// CChild_SoftwareDlg 对话框

IMPLEMENT_DYNAMIC(CChild_SoftwareDlg, CDialogEx)

typedef struct _SOFTWAREINFO
{
	string displayName;    //软件名称
	string publisher;	   //发行商
	string displayVersion; //版本
	string installDate;    //安装日期
	string uninstallString;//卸载命令及参数
	string strSoftInsPath;		//软件安装路径

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


// CChild_SoftwareDlg 消息处理程序



BOOL CChild_SoftwareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListSoft.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// TODO:  在此添加额外的初始化
	m_ListSoft.InsertColumn(0, "序号", NULL, 50);
	m_ListSoft.InsertColumn(1, "软件名称", NULL, 300);
	m_ListSoft.InsertColumn(2, "发行商", NULL, 200);
	m_ListSoft.InsertColumn(3, "版本信息", NULL, 200);
	m_ListSoft.InsertColumn(4, "安装时间", NULL, 200);
	m_ListSoft.InsertColumn(5, "卸载参数及命令", NULL, 200);
	ShowList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
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
					info.displayName = regBufferValue; //软件名称
				
					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("Publisher"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.publisher = regBufferValue;  //发行商

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("DisplayVersion"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.displayVersion = regBufferValue; //版本

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					// 判断是否能在注册表中获取到安装时间, 否取子项创建时间
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
					info.uninstallString = regBufferValue;//卸载命令及参数

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					RegQueryValueEx(hSubKey, _T("InstallLocation"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					info.strSoftInsPath = regBufferValue;//安装路径

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, "");
					// 取ParentKeyName键值,判断是否是补丁信息, 是补丁信息键值为"OperatingSystem"
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
						&& (_tcscmp(tchLocal, _T("OperatingSystem")))) //判断是否是补丁信息 
					{
						m_softwareInfo.push_back(info);
					}

				}
			}//end for(; ;)
		}
	}
	else
	{
		return FALSE; //打开键失败
	}
	RegCloseKey(hKey);

	return TRUE;
}


void CChild_SoftwareDlg::OnLvnItemchangedListSoft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CChild_SoftwareDlg::OnRclickListSoft(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//防止在空白区点击弹出菜单  
	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选软件的卸载命令
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
		//下面的这段代码, 不单单适应于ListCtrl  
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
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令处理程序代码
	if (!m_softwareInfo[m_Select].strSoftInsPath[0])
	{
		return;
	}
	if (!OpenFolderAndSelectFile((TCHAR*)m_softwareInfo[m_Select].strSoftInsPath.data()))
	{
		MessageBox("打开文件位置失败！");
		return;
	}

	
}
