// DLLCheck.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "DLLCheck.h"
#include "afxdialogex.h"


// CDLLCheck �Ի���

IMPLEMENT_DYNAMIC(CDLLCheck, CDialogEx)

CDLLCheck::CDLLCheck(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DLL, pParent)
{

}

CDLLCheck::~CDLLCheck()
{
}

void CDLLCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DLL, m_List_DLL);
}


BEGIN_MESSAGE_MAP(CDLLCheck, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UNLOADDLL, &CDLLCheck::OnBnClickedButtonUnloaddll)
END_MESSAGE_MAP()


// CDLLCheck ��Ϣ�������


BOOL CDLLCheck::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDLLList();
	ShowModule();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDLLCheck::InitDLLList()
{
	//���á�List Control���ؼ�����չ���  
	m_List_DLL.SetExtendedStyle(
		m_List_DLL.GetExtendedStyle()
		| LVS_EX_GRIDLINES        //�������  
		| LVS_EX_FULLROWSELECT);  //ѡ��ĳ��ʹ���и�����ֻ������report���  

								  //�����Ŀ  
	m_List_DLL.InsertColumn(0, _T("���"));
	m_List_DLL.InsertColumn(1, _T("��    ��"));
	m_List_DLL.InsertColumn(2, _T("·    ��"));
	//�����еĿ��  
	m_List_DLL.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_List_DLL.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_List_DLL.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}

void CDLLCheck::ShowModule()
{
	//����б�
	m_List_DLL.DeleteAllItems();
	//��ȡ�������еĹ�����������ѡ�н��̵�PIDֵ��
	/*CChild_ProcessDlg *p;
	p = (CChild_ProcessDlg *)GetParent();
	int nPid = p->pid;*/
	int nPid = pid;
	MODULEENTRY32 Me32 = { 0 };
	Me32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
	int a = GetLastError();
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("��������ʧ�ܣ�");
		return;
	}

	BOOL bRet = Module32First(hSnap, &Me32);
	int i = 0;
	CString str;
	while (bRet)
	{
		str.Format("%d", i);
		m_List_DLL.InsertItem(i, str);
		m_List_DLL.SetItemText(i, 1, Me32.szModule);
		m_List_DLL.SetItemText(i, 2, Me32.szExePath);
		i++;
		bRet = Module32Next(hSnap, &Me32);
	}
}

void CDLLCheck::OnBnClickedButtonUnloaddll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (::MessageBox(0, "��������ж��ģ�飡", "����", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}
	int nPid = pid;

	//��ȡ�б������ѡ�е�λ��    
	POSITION Pos = m_List_DLL.GetFirstSelectedItemPosition();
	int nSelect = -1;
	while (Pos)
	{
		nSelect = m_List_DLL.GetNextSelectedItem(Pos);
	}
	//������б����û�н���ѡ���򱨴�        
	if (-1 == nSelect)
	{
		AfxMessageBox("��ѡ��ģ�飡");
		return;
	}
	//��ȡ�б����DLL������        
	char  szDllName[MAX_PATH] = { 0 };
	m_List_DLL.GetItemText(nSelect, 1, szDllName, MAX_PATH);
	//FreeLibrary();
	UnInjectDll(szDllName, nPid);
	ShowModule();

}

BOOL CDLLCheck::UnInjectDll(const TCHAR* ptszDllFile, DWORD dwProcessId)
{
	// ������Ч  
	if (NULL == ptszDllFile || 0 == ::_tcslen(ptszDllFile))
	{
		AfxMessageBox("������Ч��");
		return false;
	}
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	// ��ȡģ�����  
	hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (INVALID_HANDLE_VALUE == hModuleSnap)
	{
		AfxMessageBox("��������ʧ�ܣ�");
		return false;
	}
	MODULEENTRY32 me32;
	memset(&me32, 0, sizeof(MODULEENTRY32));
	me32.dwSize = sizeof(MODULEENTRY32);
	// ��ʼ����  
	if (FALSE == ::Module32First(hModuleSnap, &me32))
	{
		::CloseHandle(hModuleSnap);
		return false;
	}
	// ��������ָ��ģ��  
	bool isFound = false;
	do
	{
		isFound = (0 == ::_tcsicmp(me32.szModule, ptszDllFile) || 0 == ::_tcsicmp(me32.szExePath, ptszDllFile));
		if (isFound) // �ҵ�ָ��ģ��  
		{
			break;
		}
	} while (TRUE == ::Module32Next(hModuleSnap, &me32));

	::CloseHandle(hModuleSnap);
	if (false == isFound)
	{
		return false;
	}
	// ��ȡĿ����̾��  
	hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		return false;
	}
	// �� Kernel32.dll �л�ȡ FreeLibrary ������ַ  
	LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "FreeLibrary");
	if (NULL == lpThreadFun)
	{
		::CloseHandle(hProcess);
		return false;
	}
	// ����Զ���̵߳��� FreeLibrary  
	hThread = ::CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, me32.modBaseAddr /* ģ���ַ */, 0, NULL);
	if (NULL == hThread)
	{
		::CloseHandle(hProcess);
		return false;
	}
	// �ȴ�Զ���߳̽���  
	::WaitForSingleObject(hThread, INFINITE);
	// ����  
	::CloseHandle(hThread);
	::CloseHandle(hProcess);
	return true;
}
