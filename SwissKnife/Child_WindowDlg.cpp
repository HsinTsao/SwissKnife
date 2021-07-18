// Child_WindowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_WindowDlg.h"
#include "afxdialogex.h"

using namespace std;
// CChild_WindowDlg �Ի���

IMPLEMENT_DYNAMIC(CChild_WindowDlg, CDialogEx)

DWORD num = 0;

CChild_WindowDlg m_Dlg;
enum
{
	WINDOWHANDLE = 0,               // ���     
	WINDOWNAME,                     // ����
	WINDOWSTATE,                    // ״̬
};

typedef struct
{
	char*   TitleData;             //�б������
	int		TitleWidth;            //�б�Ŀ��
}WINDOW_DATA;

WINDOW_DATA g_Window_Data_Show[] =
{
	{ "���ھ��",	    100 },
	{ "��������",		500 },
	{ "״̬",	        100 },
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


// CChild_WindowDlg ��Ϣ�������
BOOL CChild_WindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO
	InitControl();
	ShowWindowsList(GetWindowsList());
	//GetWindowsList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CChild_WindowDlg::InitControl()
{
	m_List_Window.SetExtendedStyle(
		m_List_Window.GetExtendedStyle()
		| LVS_EX_GRIDLINES        //�������    
		| LVS_EX_FULLROWSELECT);  //ѡ��ĳ��ʹ���и�����ֻ������report���  

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
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&szBuffer);  //ע�ắ��
																   //���API�������������к���ָ����� 
															       //������ϵͳע��һ�� �ص�����
	return szBuffer;
}



BOOL CALLBACK CChild_WindowDlg::EnumWindowsProc(HWND hWnd, LPARAM lParam)  //Ҫ���� **
{
	DWORD	dwLength = 0;
	DWORD	dwOffset = 0;
	DWORD	dwProcessID = 0;
	DWORD     TitleLen;
	LPBYTE	szBuffer = *(LPBYTE *)lParam;


	WINDOWINFO1 stcWindowInfo = { 0 };
	char	szTitle[1024];
	memset(szTitle, 0, sizeof(szTitle));
	//�õ�ϵͳ���ݽ����Ĵ��ھ���Ĵ��ڱ���
	::GetWindowText(hWnd, szTitle, sizeof(szTitle));             //�Ƿ�ö�����ش��ڣ�����
	//�����ж� �����Ƿ�ɼ� �����Ϊ��
	if (!::IsWindowVisible(hWnd) || lstrlen(szTitle) == 0)
		return true;
	//ͬ���̹���һ������ע�����ķ��͵����ض˵����ݽṹ

	//if (szBuffer == NULL)
	//	szBuffer = (LPBYTE)LocalAlloc(LPTR, 1);  //��ʱ���仺���� 

	//											 //[��Ϣ][4Notepad.exe\0]
	//dwLength = sizeof(DWORD) + lstrlen(szTitle) + 1;

	//dwOffset = LocalSize(szBuffer);  //1
	//								 //���¼��㻺������С
	//szBuffer = (LPBYTE)LocalReAlloc(szBuffer, dwOffset + dwLength, LMEM_ZEROINIT | LMEM_MOVEABLE);
	//int a  = LocalSize(szBuffer);
	////��������memcpy���ܿ������ݽṹΪ hwnd+���ڱ���+0
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
	CString restr="����";                                //show  sh
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
		LPDWORD	lpPID = LPDWORD(szBuffer + dwOffset);   //���ھ��
		dwOffset = dwOffset + sizeof(DWORD);
		int TitleLen = (int)szBuffer[dwOffset];
		dwOffset = dwOffset + sizeof(DWORD);
		//szTitle = (char *)szBuffer + dwOffset + sizeof(DWORD);   //���ڱ��� 
		szTitle = (char*)LocalAlloc(LPTR, TitleLen);
		memcpy(szTitle, szBuffer + dwOffset, TitleLen);
		str.Format("%5u", *lpPID);

		stcWindowInfo.WindowHandle = str;
		stcWindowInfo.szWindowTitle = szTitle;
		stcWindowInfo.strState = "��ʾ";
		m_vecWindowInfo.push_back(stcWindowInfo);

		AddList(stcWindowInfo.WindowHandle, stcWindowInfo.szWindowTitle, stcWindowInfo.strState);
		m_List_Window.SetItemData(i, *lpPID);  //(d)   
		dwOffset += TitleLen;
	}
	str.Format("��������   ���ڸ�����%d", i);   //�޸�CtrlList 
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_List_Window.SetColumn(1, &lvc);

}

//List��Ҫ��ʾ��
void CChild_WindowDlg::AddList(CString hwnd, CString name, CString state)	
{
	//Ĭ��Ϊ0��  �������в�������ж���������
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

	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ�����������
		m_Select = n;
		//ͨ�ô��� ����Ҽ�
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
	m_List_Window.SetItemText(m_Select, 2, "��ʾ");
}


void CChild_WindowDlg::OnHideWindow()
{
	DWORD Hwnd = atoi(m_vecWindowInfo[m_Select].WindowHandle);
	::ShowWindow((HWND__ *)Hwnd, SW_HIDE);
	m_List_Window.SetItemText(m_Select, 2, "����");
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
