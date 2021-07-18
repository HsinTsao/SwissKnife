// DLLCheck.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "DLLCheck.h"
#include "afxdialogex.h"


// CDLLCheck 对话框

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


// CDLLCheck 消息处理程序


BOOL CDLLCheck::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDLLList();
	ShowModule();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDLLCheck::InitDLLList()
{
	//设置“List Control”控件的扩展风格  
	m_List_DLL.SetExtendedStyle(
		m_List_DLL.GetExtendedStyle()
		| LVS_EX_GRIDLINES        //有网络格  
		| LVS_EX_FULLROWSELECT);  //选中某行使整行高亮（只适用于report风格）  

								  //添加列目  
	m_List_DLL.InsertColumn(0, _T("序号"));
	m_List_DLL.InsertColumn(1, _T("名    称"));
	m_List_DLL.InsertColumn(2, _T("路    径"));
	//设置列的宽度  
	m_List_DLL.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_List_DLL.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_List_DLL.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}

void CDLLCheck::ShowModule()
{
	//清空列表
	m_List_DLL.DeleteAllItems();
	//获取父窗口中的公共变量（所选中进程的PID值）
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
		AfxMessageBox("创建快照失败！");
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
	// TODO: 在此添加控件通知处理程序代码

	if (::MessageBox(0, "请勿随意卸载模块！", "继续", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}
	int nPid = pid;

	//获取列表框中所选中的位置    
	POSITION Pos = m_List_DLL.GetFirstSelectedItemPosition();
	int nSelect = -1;
	while (Pos)
	{
		nSelect = m_List_DLL.GetNextSelectedItem(Pos);
	}
	//如果在列表框中没有进行选择，则报错        
	if (-1 == nSelect)
	{
		AfxMessageBox("请选择模块！");
		return;
	}
	//获取列表框中DLL的名称        
	char  szDllName[MAX_PATH] = { 0 };
	m_List_DLL.GetItemText(nSelect, 1, szDllName, MAX_PATH);
	//FreeLibrary();
	UnInjectDll(szDllName, nPid);
	ShowModule();

}

BOOL CDLLCheck::UnInjectDll(const TCHAR* ptszDllFile, DWORD dwProcessId)
{
	// 参数无效  
	if (NULL == ptszDllFile || 0 == ::_tcslen(ptszDllFile))
	{
		AfxMessageBox("参数无效！");
		return false;
	}
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	// 获取模块快照  
	hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (INVALID_HANDLE_VALUE == hModuleSnap)
	{
		AfxMessageBox("创建快照失败！");
		return false;
	}
	MODULEENTRY32 me32;
	memset(&me32, 0, sizeof(MODULEENTRY32));
	me32.dwSize = sizeof(MODULEENTRY32);
	// 开始遍历  
	if (FALSE == ::Module32First(hModuleSnap, &me32))
	{
		::CloseHandle(hModuleSnap);
		return false;
	}
	// 遍历查找指定模块  
	bool isFound = false;
	do
	{
		isFound = (0 == ::_tcsicmp(me32.szModule, ptszDllFile) || 0 == ::_tcsicmp(me32.szExePath, ptszDllFile));
		if (isFound) // 找到指定模块  
		{
			break;
		}
	} while (TRUE == ::Module32Next(hModuleSnap, &me32));

	::CloseHandle(hModuleSnap);
	if (false == isFound)
	{
		return false;
	}
	// 获取目标进程句柄  
	hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		return false;
	}
	// 从 Kernel32.dll 中获取 FreeLibrary 函数地址  
	LPTHREAD_START_ROUTINE lpThreadFun = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "FreeLibrary");
	if (NULL == lpThreadFun)
	{
		::CloseHandle(hProcess);
		return false;
	}
	// 创建远程线程调用 FreeLibrary  
	hThread = ::CreateRemoteThread(hProcess, NULL, 0, lpThreadFun, me32.modBaseAddr /* 模块地址 */, 0, NULL);
	if (NULL == hThread)
	{
		::CloseHandle(hProcess);
		return false;
	}
	// 等待远程线程结束  
	::WaitForSingleObject(hThread, INFINITE);
	// 清理  
	::CloseHandle(hThread);
	::CloseHandle(hProcess);
	return true;
}
