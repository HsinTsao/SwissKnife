// Child_Process.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_ProcessDlg.h"
#include "afxdialogex.h"


// CChild_Process 对话框

IMPLEMENT_DYNAMIC(CChild_ProcessDlg, CDialogEx)


enum
{
	ONLINELIST_PROCESSNAME = 0,  // 进程名字      
	ONLINELIST_PID,              // 进程ID
	ONLINELIST_FULLPATH,         // 完整路径
};

typedef struct
{
	char*   TitleData;            //列表的名称
	int		TitleWidth;           //列表的宽度
}COLUMN_DATA;

COLUMN_DATA g_Column_Data_Show[] =
{
	{ "进程名称",	    150 },
	{ "进程ID",			150 },
	{ "进程完整路径",  	610 },

};

int  g_Column_Count_Show = 3;


CChild_ProcessDlg::CChild_ProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{
	
}

CChild_ProcessDlg::~CChild_ProcessDlg()
{

}

void CChild_ProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_List_Process);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_List_Process);
}


BEGIN_MESSAGE_MAP(CChild_ProcessDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Refresh, &CChild_ProcessDlg::OnBnClickedButtonRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, &CChild_ProcessDlg::OnRclickListProcess)
	ON_COMMAND(ID_Menu_KILL, &CChild_ProcessDlg::OnMenuKill)
	ON_BN_CLICKED(IDC_BUTTON1, &CChild_ProcessDlg::OnBnClickedButton1)
	ON_COMMAND(ID_STOP, &CChild_ProcessDlg::OnStop)
	ON_COMMAND(ID_REMUSE, &CChild_ProcessDlg::OnRemuse)
	ON_COMMAND(ID_DLL, &CChild_ProcessDlg::OnDll)
	ON_COMMAND(ID_PROCESSOPEN, &CChild_ProcessDlg::OnProcessopen)

END_MESSAGE_MAP()


BOOL CChild_ProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControl();
	//OnBnClickedButtonRefresh();
	ShowProcess();
	return TRUE;  			 
}

// CChild_Process 消息处理程序
void CChild_ProcessDlg::InitControl()
{
	for (int i = 0; i < g_Column_Count_Show; i++)
	{
		m_List_Process.InsertColumn(i, g_Column_Data_Show[i].TitleData,
			LVCFMT_LEFT, g_Column_Data_Show[i].TitleWidth);
	}

	m_List_Process.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}

void CChild_ProcessDlg::AddList(CString strProcessName, CString PID, CString Phandle,
	CString strFullPath, CString strCorporation)
{
	//默认为0行  这样所有插入的新列都在最上面
	int i = m_List_Process.InsertItem(m_List_Process.GetItemCount(), strProcessName);

	m_List_Process.SetItemText(i, ONLINELIST_PID, PID);
	m_List_Process.SetItemText(i, ONLINELIST_FULLPATH, strFullPath);


}

void CChild_ProcessDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	PROCESS stcProcessInfo = { 0 };
	m_List_Process.DeleteAllItems();
	m_vecProcessInfo.clear();

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString sPID;
	CString sCount;
	CString sPhandle = NULL;
	char* temp = NULL;
	m_Count = 0;

	//EnableDebugPrivilege();

	for (int PID = 0; PID <0xffff; PID++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, PID);
		if (hProcess)
		{
			char ProcessName[MAX_PATH] = { 0 };
			GetModuleFileNameEx(hProcess, NULL, ProcessName,MAX_PATH);
			if (PID == 4)
			{
				char* fname = "[system]";
				sPID.Format("%d", PID);
				AddList(fname, sPID, NULL, ProcessName, NULL);
				stcProcessInfo.PID = PID;
				stcProcessInfo.ProcessName = fname;
				stcProcessInfo.ProcessLocation = ProcessName;
				m_vecProcessInfo.push_back(stcProcessInfo);
			}
			if (ProcessName[0] != '\0')
			{
				_splitpath_s(ProcessName, drive, dir, fname, ext);
				strcat_s(fname, ext);
				sPID.Format("%d", PID);
				GetLogicalDriveStrings(0, NULL);
		
				AddList(fname, sPID, NULL, ProcessName, NULL);
				stcProcessInfo.PID = PID;
				stcProcessInfo.ProcessName = fname;
				stcProcessInfo.ProcessLocation = ProcessName;
				m_vecProcessInfo.push_back(stcProcessInfo);
			}
			m_Count++;
			PID += 4;
		}
	}
	sCount.Format("%d", m_Count);
	sCount = sCount + "(暴力枚举)";
	SetDlgItemText(IDC_STATIC_COUNT, sCount);
}

void CChild_ProcessDlg::ShowProcess()
{
	PROCESS stcProcessInfo = { 0 };
	m_vecProcessInfo.clear();
	//清空列表
	m_List_Process.DeleteAllItems();
	//给系统内所有的进程拍个快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("进程快照创建失败！");
		return;
	}

	PROCESSENTRY32 Pe32 = { 0 };
	//在使用这个结构前，先设置它的大小
	Pe32.dwSize = sizeof(PROCESSENTRY32);
	//遍历进程快照，轮流显示每个进程的信息
	BOOL bRet = Process32First(hSnap, &Pe32);
	int i = 0;
	CString str;
	CString sCount;
	m_Count = 0;

	while (bRet)
	{	
		//进程名
		m_List_Process.InsertItem(i, Pe32.szExeFile);
		//进程ID
		str.Format("%d", Pe32.th32ProcessID);
		m_List_Process.SetItemText(i, 1, str);
		
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, Pe32.th32ProcessID);
		char ProcessName[MAX_PATH] = { 0 };
		GetModuleFileNameEx(hProcess, NULL, ProcessName, MAX_PATH);
		str.Format("%s", ProcessName);
		m_List_Process.SetItemText(i, 2, str);
		
		stcProcessInfo.PID = Pe32.th32ProcessID;
		stcProcessInfo.ProcessName = Pe32.szExeFile;
		stcProcessInfo.ProcessLocation = ProcessName;
		m_vecProcessInfo.push_back(stcProcessInfo);

		i++;
		bRet = Process32Next(hSnap, &Pe32);

		m_Count++;
	}
	sCount.Format("%d", m_Count);
	SetDlgItemText(IDC_STATIC_COUNT, sCount);
	CloseHandle(hSnap);
}



void CChild_ProcessDlg::OnRclickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选软件的启动项
		m_Select = n;
		//下面的这段代码, 不单单适应于ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU_PROCESS);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}


void CChild_ProcessDlg::OnMenuKill()
{
	DWORD nPid = m_vecProcessInfo[m_Select].PID;

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, nPid);
	if (hProcess == NULL)
	{
		MessageBox("无法结束进程！");
		return;
	}
	TerminateProcess(hProcess, 0);

	CloseHandle(hProcess);
	MessageBox("已结束进程！");
	OnBnClickedButtonRefresh();
}


void CChild_ProcessDlg::OnBnClickedButton1()
{
	ShowProcess();
}


void CChild_ProcessDlg::OnStop()
{
	int nPid = -1;
	nPid = m_vecProcessInfo[m_Select].PID;
	//创建线程快照  
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, nPid);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("暂停进程失败！");
		return;
	}

	THREADENTRY32 Te32 = { 0 };
	Te32.dwSize = sizeof(THREADENTRY32);
	BOOL bRet = Thread32First(hSnap, &Te32);

	while (bRet)
	{
		//判断线程所属  
		if (Te32.th32OwnerProcessID == nPid)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Te32.th32ThreadID);
			SuspendThread(hThread);
			CloseHandle(hThread);
		}

		bRet = Thread32Next(hSnap, &Te32);
	}
	CloseHandle(hSnap);
}


void CChild_ProcessDlg::OnRemuse()
{
	int nPid = -1;
	nPid = m_vecProcessInfo[m_Select].PID;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, nPid);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("进程恢复失败！");
		return;
	}

	THREADENTRY32 Te32 = { 0 };
	Te32.dwSize = sizeof(THREADENTRY32);
	BOOL bRet = Thread32First(hSnap, &Te32);

	while (bRet)
	{
		if (Te32.th32OwnerProcessID == nPid)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Te32.th32ThreadID);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}

		bRet = Thread32Next(hSnap, &Te32);
	}
}


void CChild_ProcessDlg::OnDll()
{
	pid = m_vecProcessInfo[m_Select].PID;
	CDLLCheck DLLCheck;
	DLLCheck.pid = pid;
	DLLCheck.DoModal();
}


void CChild_ProcessDlg::OnProcessopen()
{
	if (!m_vecProcessInfo[m_Select].ProcessLocation[0])
	{
		return;
	}
	if (!OpenFolderAndSelectFile((TCHAR*)m_vecProcessInfo[m_Select].ProcessLocation.GetBuffer()))
	{
		AfxMessageBox("打开文件位置失败！");
		return;
	}
}


BOOL CChild_ProcessDlg::OpenFolderAndSelectFile(TCHAR* szPath)
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

