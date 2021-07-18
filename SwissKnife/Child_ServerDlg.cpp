// Child_ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_ServerDlg.h"
#include "afxdialogex.h"


// CChild_ServerDlg 对话框

IMPLEMENT_DYNAMIC(CChild_ServerDlg, CDialogEx)


enum
{
	SERVERNAME = 0,              // 名称      
	SERVERDESCRIPTION,           // 描述
    SERVERSTATE,                 // 状态
	SERVERSTARTUPTYPE,           // 启动类型
	SERVERTYPE,                  // 服务类型
};

typedef struct
{
	char*   TitleData;             //列表的名称
	int		TitleWidth;            //列表的宽度
}SERVER_DATA;

SERVER_DATA g_Server_Data_Show[] =
{
	{ "服务名称",	    150 },
	{ "描述",			300 },
	{ "状态",	        150 },
	{ "启动类型",		100 },
	{ "服务类型",		220 },
};
int  g_Server_Count_Show = 5;

CChild_ServerDlg::CChild_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SERVER, pParent)
{

}

CChild_ServerDlg::~CChild_ServerDlg()
{
}

void CChild_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVER, m_List_Server);
}


BEGIN_MESSAGE_MAP(CChild_ServerDlg, CDialogEx)
	ON_COMMAND(ID_OPENSERVICE, &CChild_ServerDlg::OnOpenservice)
	ON_WM_TIMER()
	ON_COMMAND(ID_SHUTDOWNSERVICE, &CChild_ServerDlg::OnShutDownService)
	ON_COMMAND(ID_REFLASH, &CChild_ServerDlg::OnReflash)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVER, &CChild_ServerDlg::OnRclickListServer)
END_MESSAGE_MAP()


// CChild_ServerDlg 消息处理程序
BOOL CChild_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControl();
	GetServiceInfo();
	SetTimer(UPDATE_SERVICE_INFO, 1000, NULL);

	return TRUE; 
}


void CChild_ServerDlg::InitControl()
{
	for (int i = 0; i < g_Server_Count_Show; i++)
	{
		m_List_Server.InsertColumn(i, g_Server_Data_Show[i].TitleData,
			LVCFMT_LEFT, g_Server_Data_Show[i].TitleWidth);
	}
	m_List_Server.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}

//获取服务信息
void CChild_ServerDlg::GetServiceInfo()
{
	//1.打开服务控制管理器
	m_List_Server.DeleteAllItems();
	m_vecSerInfo.clear();
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS lpEnumService = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.第二次枚举
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		lpEnumService,		//缓冲区
		dwSize,				//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//5.遍历信息
	SC_HANDLE hService = nullptr;
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//获取基础信息
		//1.服务名称
		//2.服务描述
		//3.服务状态(根据得到的值手动输出字符串)
		//“已停止” “正在运行" "正在暂停"...
		m_vecSerInfo.push_back(lpEnumService[i]);

		//获取更多信息
		//1.打开服务
		hService = OpenService(hSCM, lpEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 2.第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwSize);
		//3.第二次调用,获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//获取服务信息完毕，开始插入List

		AddList(m_vecSerInfo[i].lpServiceName,
			m_vecSerInfo[i].lpDisplayName,
			m_vecSerInfo[i].ServiceStatus.dwCurrentState,
			pServiceConfig->dwStartType,
			m_vecSerInfo[i].ServiceStatus.dwServiceType);

		LocalFree(pServiceConfig);
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}


void CChild_ServerDlg::AddList(LPSTR lpServiceName, LPSTR lpDisplayName, DWORD ServiceStatus, DWORD dwStartType,
	DWORD dwServiceType)
{
	CString strServiceStatus;
	CString strStartType;
	CString strServiceType;
	//默认为0行  这样所有插入的新列都在最上面
	int i = m_List_Server.InsertItem(m_List_Server.GetItemCount(), lpServiceName);

	m_List_Server.SetItemText(i, SERVERDESCRIPTION, lpDisplayName);
	
	switch (ServiceStatus)
	{
	case SERVICE_STOPPED:
		strServiceStatus = "已停止";
		break;
	case SERVICE_STOP_PENDING:	
		strServiceStatus = "正在停止";
		break;
	case SERVICE_START_PENDING:	
		strServiceStatus = "正在启动";
		break;
	case SERVICE_RUNNING:	
		strServiceStatus = "正在运行";
		break;
	case SERVICE_PAUSED:		
		strServiceStatus = "已暂停";
		break;
	case SERVICE_PAUSE_PENDING:	
		strServiceStatus = "正在暂停";
		break;
	case SERVICE_CONTINUE_PENDING:
		strServiceStatus = "准备继续";
		break;
	}
	m_List_Server.SetItemText(i, SERVERSTATE, strServiceStatus);

	switch (dwStartType)
	{
	case SERVICE_AUTO_START:
		strStartType = "自动";
		break;
	case SERVICE_BOOT_START:
		strStartType = "SERVICE_BOOT_START";
		break;
	case SERVICE_DEMAND_START:
		strStartType = "手动";
		break;
	case SERVICE_DISABLED:
		strStartType = "禁用";
		break;
	case SERVICE_SYSTEM_START:
		strStartType = "SERVICE_SYSTEM_START";
		break;
	}

	m_List_Server.SetItemText(i, SERVERSTARTUPTYPE, strStartType);

	switch (dwServiceType)
	{
	case SERVICE_FILE_SYSTEM_DRIVER:
		strServiceType = "文件系统驱动服务";
		break;
	case SERVICE_KERNEL_DRIVER:
		strServiceType = "驱动服务";
		break;
	case SERVICE_WIN32_OWN_PROCESS:
		strServiceType = "独立进程服务";
		break;
	case SERVICE_WIN32_SHARE_PROCESS:
		strServiceType = "共享进程服务";
		break;
	}
	m_List_Server.SetItemText(i, SERVERTYPE, strServiceType);

}

void CChild_ServerDlg::UpDataSerInfo()
{
	//1.打开计算机服务控制管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.第二次枚举
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		lpEnumService,		//缓冲区
		dwSize,				//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL);
	//5.遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//如果是要更新的服务
		if (!_tcscmp(lpEnumService[i].lpServiceName, m_ServiceName))
		{
			//更新list中该服务的状态信息
			switch (lpEnumService[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_STOPPED:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("已停止"));
				break;
			case SERVICE_STOP_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("正在停止"));
				break;
			case SERVICE_START_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("正在启动"));
				break;
			case SERVICE_RUNNING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("正在运行"));
				break;
			case SERVICE_PAUSED:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("已暂停"));
				break;
			case SERVICE_PAUSE_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("正在暂停"));
				break;
			case SERVICE_CONTINUE_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("准备继续"));
				break;
			}
		}
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
}


void CChild_ServerDlg::OnOpenservice()
{

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //打开服务控制管理器
	SC_HANDLE hService = OpenService(hSCM, m_ServiceName, SERVICE_START);//打开服务
	StartService(hService, 0, 0); //启动服务
	//更新信息
	UpDataSerInfo();
	m_IsUpDateSer = TRUE;
	m_UpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}




void CChild_ServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (UPDATE_SERVICE_INFO == nIDEvent&&m_UpDataTimes != 0 && m_IsUpDateSer)
	{
		UpDataSerInfo();
		if (m_UpDataTimes == 1)
		{
			m_IsUpDateSer = FALSE;
		}
		m_UpDataTimes--;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CChild_ServerDlg::OnShutDownService()
{
	//停止服务
	SERVICE_STATUS status;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //打开服务控制管理器
	SC_HANDLE hService = OpenService(hSCM, m_ServiceName, SERVICE_STOP); //打开服务
	ControlService(hService, SERVICE_CONTROL_STOP, &status);             //结束服务
	//更新信息
	UpDataSerInfo();

	m_IsUpDateSer = TRUE;
	m_UpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}


void CChild_ServerDlg::OnReflash()
{
	// TODO: 在此添加命令处理程序代码
	GetServiceInfo();
}


void CChild_ServerDlg::OnRclickListServer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选服务的 服务名
		m_ServiceName = m_List_Server.GetItemText(n, 0);
		m_SelectNum = n;
		//下面的这段代码, 不单单适应于ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU_SERVICE);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}
