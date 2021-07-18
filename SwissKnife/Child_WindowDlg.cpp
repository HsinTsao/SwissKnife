// Child_WindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_WindowDlg.h"
#include "afxdialogex.h"

using namespace std;
// CChild_WindowDlg 对话框

IMPLEMENT_DYNAMIC(CChild_WindowDlg, CDialogEx)

DWORD num = 0;

CChild_WindowDlg m_Dlg;
enum
{
	WINDOWHANDLE = 0,               // 句柄     
	WINDOWNAME,                     // 名称
	WINDOWSTATE,                    // 状态
};

typedef struct
{
	char*   TitleData;             //列表的名称
	int		TitleWidth;            //列表的宽度
}WINDOW_DATA;

WINDOW_DATA g_Window_Data_Show[] =
{
	{ "窗口句柄",	    100 },
	{ "窗口名称",		500 },
	{ "状态",	        100 },
};
int  g_Window_Count_Show = 3;

CChild_WindowDlg::CChild_WindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WINDOW, pParent)
{

}

CChild_WindowDlg::~CChild_WindowDlg()
{

}

void CChild_WindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINDOW, m_List_Window);
}


BEGIN_MESSAGE_MAP(CChild_WindowDlg, CDialogEx)
	ON_COMMAND(ID_32809, &CChild_WindowDlg::OnRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WINDOW, &CChild_WindowDlg::OnRclickListWindow)
	ON_COMMAND(ID_CLOSE, &CChild_WindowDlg::OnCloseWindow)
	ON_COMMAND(ID_SHOW, &CChild_WindowDlg::OnShowWindow)
	ON_COMMAND(ID_HIDE, &CChild_WindowDlg::OnHideWindow)
	ON_COMMAND(ID_MAX, &CChild_WindowDlg::OnMax)
	ON_COMMAND(ID_MIN, &CChild_WindowDlg::OnMin)
END_MESSAGE_MAP()


// CChild_WindowDlg 消息处理程序
BOOL CChild_WindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO
	InitControl();
	ShowWindowsList(GetWindowsList());
	//GetWindowsList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChild_WindowDlg::InitControl()
{
	m_List_Window.SetExtendedStyle(
		m_List_Window.GetExtendedStyle()
		| LVS_EX_GRIDLINES        //有网络格    
		| LVS_EX_FULLROWSELECT);  //选中某行使整行高亮（只适用于report风格）  

	for (int i = 0; i < g_Window_Count_Show; i++)
	{
		m_List_Window.InsertColumn(i, g_Window_Data_Show[i].TitleData,
			LVCFMT_LEFT, g_Window_Data_Show[i].TitleWidth);
	}
	m_List_Window.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}



LPBYTE CChild_WindowDlg::GetWindowsList()
{
	LPBYTE	szBuffer = NULL;  //char* p = NULL   &p
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&szBuffer);  //注册函数
																   //如果API函数参数当中有函数指针存在 
															       //就是向系统注册一个 回调函数
	return szBuffer;
}



BOOL CALLBACK CChild_WindowDlg::EnumWindowsProc(HWND hWnd, LPARAM lParam)  //要数据 **
{
	DWORD	dwLength = 0;
	DWORD	dwOffset = 0;
	DWORD	dwProcessID = 0;
	DWORD     TitleLen;
	LPBYTE	szBuffer = *(LPBYTE *)lParam;


	WINDOWINFO1 stcWindowInfo = { 0 };
	char	szTitle[1024];
	memset(szTitle, 0, sizeof(szTitle));
	//得到系统传递进来的窗口句柄的窗口标题
	::GetWindowText(hWnd, szTitle, sizeof(szTitle));             //是否枚举隐藏窗口？？？
	//这里判断 窗口是否可见 或标题为空
	if (!::IsWindowVisible(hWnd) || lstrlen(szTitle) == 0)
		return true;
	//同进程管理一样我们注意他的发送到主控端的数据结构

	//if (szBuffer == NULL)
	//	szBuffer = (LPBYTE)LocalAlloc(LPTR, 1);  //暂时分配缓冲区 

	//											 //[消息][4Notepad.exe\0]
	//dwLength = sizeof(DWORD) + lstrlen(szTitle) + 1;

	//dwOffset = LocalSize(szBuffer);  //1
	//								 //重新计算缓冲区大小
	//szBuffer = (LPBYTE)LocalReAlloc(szBuffer, dwOffset + dwLength, LMEM_ZEROINIT | LMEM_MOVEABLE);
	//int a  = LocalSize(szBuffer);
	////下面两个memcpy就能看到数据结构为 hwnd+窗口标题+0
	//memcpy((szBuffer + dwOffset), &hWnd, sizeof(DWORD));
	//memcpy(szBuffer + dwOffset + sizeof(DWORD), szTitle, lstrlen(szTitle) + 1);
	num++;
	
	if (szBuffer == NULL)
	{
		TitleLen = lstrlen(szTitle) + 1;
		dwLength = sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) + lstrlen(szTitle) + 1;
		szBuffer = (LPBYTE)LocalAlloc(LPTR, dwLength);
		memcpy(szBuffer, &num, sizeof(DWORD));
		memcpy(szBuffer + dwOffset + sizeof(DWORD), &hWnd, sizeof(DWORD));
		memcpy(szBuffer + dwOffset + sizeof(DWORD) * 2, &TitleLen, sizeof(DWORD));
		memcpy(szBuffer + dwOffset + sizeof(DWORD) * 3, szTitle, lstrlen(szTitle) + 1);
	}
	else
	{
		dwOffset = LocalSize(szBuffer);
		TitleLen = lstrlen(szTitle) + 1;
		dwLength = sizeof(DWORD) + sizeof(DWORD) + lstrlen(szTitle) + 1;
		szBuffer = (LPBYTE)LocalReAlloc(szBuffer, dwOffset + dwLength, LMEM_ZEROINIT | LMEM_MOVEABLE);
		memcpy(szBuffer, &num, sizeof(DWORD));
		memcpy(szBuffer + dwOffset, &hWnd, sizeof(DWORD));
		memcpy(szBuffer + dwOffset + sizeof(DWORD), &TitleLen, sizeof(DWORD));
		memcpy(szBuffer + dwOffset + sizeof(DWORD)*2, szTitle, lstrlen(szTitle) + 1);
	}	
	*(LPBYTE *)lParam = szBuffer;
	return true;
}


void CChild_WindowDlg::ShowWindowsList(LPBYTE Buffer)
{
	char *szBuffer = (char *)Buffer;
	DWORD	dwOffset = 0;
	char	*szTitle = NULL;
	bool isDel = false;
	/*do
	{
	isDel=false;
	for (int j=0;j<m_list_process.GetItemCount();j++)  //
	{
	CString temp=m_list_process.GetItemText(j,2);
	CString restr="隐藏";                                //show  sh
	//if (temp!=restr)                                     //
	{
	m_list_process.DeleteItem(j);
	isDel=true;
	break;
	}
	}

	} while (isDel);*/

	m_List_Window.DeleteAllItems();
	m_vecWindowInfo.clear();
	CString	str;
	int i;
	WINDOWINFO1 stcWindowInfo;
	DWORD size = 0;
	size = (BYTE)szBuffer[0];
	dwOffset = sizeof(int);
	for (i = 0; i < size; i++)
	{
		LPDWORD	lpPID = LPDWORD(szBuffer + dwOffset);   //窗口句柄
		dwOffset = dwOffset + sizeof(DWORD);
		int TitleLen = (int)szBuffer[dwOffset];
		dwOffset = dwOffset + sizeof(DWORD);
		//szTitle = (char *)szBuffer + dwOffset + sizeof(DWORD);   //窗口标题 
		szTitle = (char*)LocalAlloc(LPTR, TitleLen);
		memcpy(szTitle, szBuffer + dwOffset, TitleLen);
		str.Format("%5u", *lpPID);

		stcWindowInfo.WindowHandle = str;
		stcWindowInfo.szWindowTitle = szTitle;
		stcWindowInfo.strState = "显示";
		m_vecWindowInfo.push_back(stcWindowInfo);

		AddList(stcWindowInfo.WindowHandle, stcWindowInfo.szWindowTitle, stcWindowInfo.strState);
		m_List_Window.SetItemData(i, *lpPID);  //(d)   
		dwOffset += TitleLen;
	}
	str.Format("窗口名称   窗口个数：%d", i);   //修改CtrlList 
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_List_Window.SetColumn(1, &lvc);

}

//List中要显示的
void CChild_WindowDlg::AddList(CString hwnd, CString name, CString state)	
{
	//默认为0行  这样所有插入的新列都在最上面
	int i = m_List_Window.InsertItem(m_List_Window.GetItemCount(), hwnd);
	m_List_Window.SetItemText(i, WINDOWNAME, name);
	m_List_Window.SetItemText(i, WINDOWSTATE, state);

}

void CChild_WindowDlg::OnRefresh()
{
	num = 0;
	ShowWindowsList(GetWindowsList());

}


void CChild_WindowDlg::OnRclickListWindow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选软件的启动项
		m_Select = n;
		//通用代码 添加右键
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU_WINDOW);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}


void CChild_WindowDlg::OnCloseWindow()
{
	DWORD Hwnd = atoi(m_vecWindowInfo[m_Select].WindowHandle);
	::PostMessage((HWND__ *)Hwnd, WM_CLOSE, 0, 0);
	OnRefresh();
}


void CChild_WindowDlg::OnShowWindow()
{
	DWORD Hwnd = atoi(m_vecWindowInfo[m_Select].WindowHandle);
	::ShowWindow((HWND__ *)Hwnd, SW_SHOW);
	m_List_Window.SetItemText(m_Select, 2, "显示");
}


void CChild_WindowDlg::OnHideWindow()
{
	DWORD Hwnd = atoi(m_vecWindowInfo[m_Select].WindowHandle);
	::ShowWindow((HWND__ *)Hwnd, SW_HIDE);
	m_List_Window.SetItemText(m_Select, 2, "隐藏");
}


void CChild_WindowDlg::OnMax()
{
	DWORD Hwnd = atoi(m_vecWindowInfo[m_Select].WindowHandle);
	::ShowWindow((HWND__ *)Hwnd, SW_MAXIMIZE);
}


void CChild_WindowDlg::OnMin()
{
	DWORD Hwnd = atoi(m_vecWindowInfo[m_Select].WindowHandle);
	::ShowWindow((HWND__ *)Hwnd, SW_MINIMIZE);
}
