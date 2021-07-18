// Child_ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_ServerDlg.h"
#include "afxdialogex.h"


// CChild_ServerDlg �Ի���

IMPLEMENT_DYNAMIC(CChild_ServerDlg, CDialogEx)


enum
{
	SERVERNAME = 0,              // ����      
	SERVERDESCRIPTION,           // ����
    SERVERSTATE,                 // ״̬
	SERVERSTARTUPTYPE,           // ��������
	SERVERTYPE,                  // ��������
};

typedef struct
{
	char*   TitleData;             //�б������
	int		TitleWidth;            //�б�Ŀ��
}SERVER_DATA;

SERVER_DATA g_Server_Data_Show[] =
{
	{ "��������",	    150 },
	{ "����",			300 },
	{ "״̬",	        150 },
	{ "��������",		100 },
	{ "��������",		220 },
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


// CChild_ServerDlg ��Ϣ�������
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

//��ȡ������Ϣ
void CChild_ServerDlg::GetServiceInfo()
{
	//1.�򿪷�����ƹ�����
	m_List_Server.DeleteAllItems();
	m_vecSerInfo.clear();
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		NULL,				//������
		0,					//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//3.������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS lpEnumService = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.�ڶ���ö��
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		lpEnumService,		//������
		dwSize,				//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//5.������Ϣ
	SC_HANDLE hService = nullptr;
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//��ȡ������Ϣ
		//1.��������
		//2.��������
		//3.����״̬(���ݵõ���ֵ�ֶ�����ַ���)
		//����ֹͣ�� ����������" "������ͣ"...
		m_vecSerInfo.push_back(lpEnumService[i]);

		//��ȡ������Ϣ
		//1.�򿪷���
		hService = OpenService(hSCM, lpEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		// 2.��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwSize);
		//3.�ڶ��ε���,��ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//��ȡ������Ϣ��ϣ���ʼ����List

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
	//Ĭ��Ϊ0��  �������в�������ж���������
	int i = m_List_Server.InsertItem(m_List_Server.GetItemCount(), lpServiceName);

	m_List_Server.SetItemText(i, SERVERDESCRIPTION, lpDisplayName);
	
	switch (ServiceStatus)
	{
	case SERVICE_STOPPED:
		strServiceStatus = "��ֹͣ";
		break;
	case SERVICE_STOP_PENDING:	
		strServiceStatus = "����ֹͣ";
		break;
	case SERVICE_START_PENDING:	
		strServiceStatus = "��������";
		break;
	case SERVICE_RUNNING:	
		strServiceStatus = "��������";
		break;
	case SERVICE_PAUSED:		
		strServiceStatus = "����ͣ";
		break;
	case SERVICE_PAUSE_PENDING:	
		strServiceStatus = "������ͣ";
		break;
	case SERVICE_CONTINUE_PENDING:
		strServiceStatus = "׼������";
		break;
	}
	m_List_Server.SetItemText(i, SERVERSTATE, strServiceStatus);

	switch (dwStartType)
	{
	case SERVICE_AUTO_START:
		strStartType = "�Զ�";
		break;
	case SERVICE_BOOT_START:
		strStartType = "SERVICE_BOOT_START";
		break;
	case SERVICE_DEMAND_START:
		strStartType = "�ֶ�";
		break;
	case SERVICE_DISABLED:
		strStartType = "����";
		break;
	case SERVICE_SYSTEM_START:
		strStartType = "SERVICE_SYSTEM_START";
		break;
	}

	m_List_Server.SetItemText(i, SERVERSTARTUPTYPE, strStartType);

	switch (dwServiceType)
	{
	case SERVICE_FILE_SYSTEM_DRIVER:
		strServiceType = "�ļ�ϵͳ��������";
		break;
	case SERVICE_KERNEL_DRIVER:
		strServiceType = "��������";
		break;
	case SERVICE_WIN32_OWN_PROCESS:
		strServiceType = "�������̷���";
		break;
	case SERVICE_WIN32_SHARE_PROCESS:
		strServiceType = "������̷���";
		break;
	}
	m_List_Server.SetItemText(i, SERVERTYPE, strServiceType);

}

void CChild_ServerDlg::UpDataSerInfo()
{
	//1.�򿪼����������ƹ�����
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		NULL,				//������
		0,					//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//3.������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS lpEnumService =
		(LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, dwSize);
	//4.�ڶ���ö��
	EnumServicesStatus(
		hSCM,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		lpEnumService,		//������
		dwSize,				//��������С
		&dwSize,			//��Ҫ�Ĵ�С
		&dwServiceNum,		//�������еķ������
		NULL);
	//5.������Ϣ
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//�����Ҫ���µķ���
		if (!_tcscmp(lpEnumService[i].lpServiceName, m_ServiceName))
		{
			//����list�и÷����״̬��Ϣ
			switch (lpEnumService[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_STOPPED:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("��ֹͣ"));
				break;
			case SERVICE_STOP_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("����ֹͣ"));
				break;
			case SERVICE_START_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("��������"));
				break;
			case SERVICE_RUNNING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("��������"));
				break;
			case SERVICE_PAUSED:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("����ͣ"));
				break;
			case SERVICE_PAUSE_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("������ͣ"));
				break;
			case SERVICE_CONTINUE_PENDING:
				m_List_Server.SetItemText(m_SelectNum, 2, _T("׼������"));
				break;
			}
		}
	}
	LocalFree(lpEnumService);
	CloseServiceHandle(hSCM);
}


void CChild_ServerDlg::OnOpenservice()
{

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //�򿪷�����ƹ�����
	SC_HANDLE hService = OpenService(hSCM, m_ServiceName, SERVICE_START);//�򿪷���
	StartService(hService, 0, 0); //��������
	//������Ϣ
	UpDataSerInfo();
	m_IsUpDateSer = TRUE;
	m_UpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}




void CChild_ServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//ֹͣ����
	SERVICE_STATUS status;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);   //�򿪷�����ƹ�����
	SC_HANDLE hService = OpenService(hSCM, m_ServiceName, SERVICE_STOP); //�򿪷���
	ControlService(hService, SERVICE_CONTROL_STOP, &status);             //��������
	//������Ϣ
	UpDataSerInfo();

	m_IsUpDateSer = TRUE;
	m_UpDataTimes = 5;
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hService);
}


void CChild_ServerDlg::OnReflash()
{
	// TODO: �ڴ���������������
	GetServiceInfo();
}


void CChild_ServerDlg::OnRclickListServer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ����� ������
		m_ServiceName = m_List_Server.GetItemText(n, 0);
		m_SelectNum = n;
		//�������δ���, ��������Ӧ��ListCtrl  
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
