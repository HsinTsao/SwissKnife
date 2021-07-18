
// SwissKnifeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "SwissKnifeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSoftUnload();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_SOFT_UNLOAD, &CAboutDlg::OnSoftUnload)
END_MESSAGE_MAP()


// CSwissKnifeDlg 对话框



CSwissKnifeDlg::CSwissKnifeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SWISSKNIFE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CSwissKnifeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CSwissKnifeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_PROCESS, &CSwissKnifeDlg::OnButtonProcess)
	ON_COMMAND(ID_BUTTON_HOST, &CSwissKnifeDlg::OnButtonHost)
	ON_COMMAND(ID_BUTTON_FILE, &CSwissKnifeDlg::OnButtonFile)
	ON_COMMAND(ID_BUTTON_REG, &CSwissKnifeDlg::OnButtonReg)
	ON_COMMAND(ID_BUTTON_SOFT, &CSwissKnifeDlg::OnButtonSoft)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTON_SERVER, &CSwissKnifeDlg::OnButtonServer)
	ON_COMMAND(ID_BUTTON_STARTUP, &CSwissKnifeDlg::OnButtonStartup)
	ON_COMMAND(ID_BUTTON_WINDOW, &CSwissKnifeDlg::OnButtonWindow)
END_MESSAGE_MAP()


// CSwissKnifeDlg 消息处理程序

BOOL CSwissKnifeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	// TODO: 在此添加额外的初始化代码
	//设置背景
	SetBackgroundImage(IDB_BITMAP_BACK);
	//创建真彩ToolBar
	CreateToolBar();
	//初始化控件
	InitControl();
	//生成子窗口
	if (m_ChildProcessDlg.Create(IDD_DIALOG_PROCESS) == FALSE)
	{
		MessageBox("创建进程子窗口失败","ERROR");
	}

	if (m_ChildFileDlg.Create(IDD_DIALOG_FILE) == FALSE)
	{
		MessageBox("创建文件子窗口失败", "ERROR");
	}
	if (m_ChildRegisterDlg.Create(IDD_DIALOG_REG) == FALSE)
	{
		MessageBox("创建注册表子窗口失败", "ERROR");
	}
	
	if (m_ChildSoftwareDlg.Create(IDD_DIALOG_SOFTWARE) == FALSE)
	{
		MessageBox("创建软件子窗口失败", "ERROR");
	}
	if (m_ChildCPUDlg.Create(IDD_DIALOG_CPU) == FALSE)
	{
		MessageBox("创建CPU子窗口失败", "ERROR");
	}
	if (m_ChildServerDlg.Create(IDD_DIALOG_SERVER) == FALSE)
	{
		MessageBox("创建服务子窗口失败", "ERROR");
	}
	if (m_ChildStartUpDlg.Create(IDD_DIALOG_STARTUP) == FALSE)
	{
		MessageBox("创建启动项子窗口失败", "ERROR");
	}
	if (m_ChildWindowDlg.Create(IDD_DIALOG_WINDOW) == FALSE)
	{
		MessageBox("创建窗口子窗口失败", "ERROR");
	}
	
	

	DWORD dwThreadID = 0;
	//HANDLE hThread = CreateThread(NULL, 0, MonitorThread, 0, NULL, &dwThreadID);//创建下载线程
	
	
	//初始化子窗口位置
	CRect rect;
	
	GetWindowRect(&rect);
    rect.top += 80; 

	m_ChildProcessDlg.MoveWindow(&rect);
	m_ChildFileDlg.MoveWindow(&rect);
	m_ChildRegisterDlg.MoveWindow(&rect);
	m_ChildSoftwareDlg.MoveWindow(&rect);
	m_ChildCPUDlg.MoveWindow(&rect);
	m_ChildServerDlg.MoveWindow(&rect);
	m_ChildStartUpDlg.MoveWindow(&rect);
	m_ChildWindowDlg.MoveWindow(&rect);

	m_ChildProcessDlg.ShowWindow(SW_HIDE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);
	SetTimer(1, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CSwissKnifeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSwissKnifeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSwissKnifeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CSwissKnifeDlg::CreateToolBar()
{
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_ICON))  //创建一个工具条  加载资源
	{

		return;
	}
	m_ToolBar.LoadTrueColorToolBar
	(
		48,    //加载真彩工具条
		IDB_BITMAP_TOOLBAR,
		IDB_BITMAP_TOOLBAR,
		IDB_BITMAP_TOOLBAR
	);  //和我们的位图资源相关联



	RECT Rect, RectMain;
	GetWindowRect(&RectMain);   //得到整个窗口的大小
	Rect.left = -5;
	Rect.top = 0;
	Rect.bottom = 80;
	Rect.right = RectMain.right - RectMain.left + 10;
	m_ToolBar.MoveWindow(&Rect, TRUE);

	m_ToolBar.SetButtonText(0, "本机信息");     //在位图的下面添加文件
	m_ToolBar.SetButtonText(1, "进程管理");
	m_ToolBar.SetButtonText(2, "文件管理");
	m_ToolBar.SetButtonText(3, "注册表管理");
	m_ToolBar.SetButtonText(4, "启动项管理");
	m_ToolBar.SetButtonText(5, "软件管理");
	m_ToolBar.SetButtonText(6, "窗口管理");
	m_ToolBar.SetButtonText(7, "服务管理");
	m_ToolBar.SetButtonText(8, "网络管理");
	/*m_ToolBar.SetButtonText(9, L"参数设置");
	m_ToolBar.SetButtonText(10,L"生成服务端");
	m_ToolBar.SetButtonText(11,L"帮助");*/
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);  //显示
	return VOID();
}



void CSwissKnifeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if (SIZE_MINIMIZED == nType)
	{
		return;
	}

	//if (m_CList_Show.m_hWnd != NULL)   //（控件也是窗口因此也有句柄）
	//{
	//	CRect Rect;
	//	Rect.left = 1;          //列表的左坐标     
	//	Rect.top = 80;         //列表的上坐标
	//	Rect.right = cx - 1;    //列表的右坐标
	//	Rect.bottom = cy - 160;   //列表的下坐标
	//	m_CList_Show.MoveWindow(Rect);
	//}

	if (m_ToolBar.m_hWnd != NULL)                  //工具条
	{
		CRect Rect;
		Rect.top = 0;
		Rect.left = -5;
		Rect.right = cx;
		Rect.bottom = 80;
		m_ToolBar.MoveWindow(Rect);             //设置工具条大小位置
	}
}


void CSwissKnifeDlg::InitControl()
{
	CRect RectMain;
	GetWindowRect(&RectMain);
	RectMain.bottom += 20;
	MoveWindow(RectMain);
}


void CSwissKnifeDlg::OnButtonHost()
{
	// TODO: 在此添加命令处理程序代码
	m_ChildCPUDlg.ShowWindow(TRUE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);
}

void CSwissKnifeDlg::OnButtonProcess()
{
	// TODO: 在此添加命令处理程序代码

	m_ChildProcessDlg.ShowWindow(TRUE);

	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);

}



void CSwissKnifeDlg::OnButtonFile()
{
	m_ChildFileDlg.ShowWindow(TRUE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);

	// TODO: 在此添加命令处理程序代码
}

DWORD CSwissKnifeDlg::MonitorThread(LPVOID lpParam)
{
	/*CChild_UDiskDlg    m_ChildUDiskDlg;
	
	if (m_ChildUDiskDlg.Create(IDD_DIALOG_UDISK) == FALSE)
	{
		int a = GetLastError();
		::MessageBox(0,"创建U盘子窗口失败", "ERROR",0);
	}
*/
	

	
	return 0;
}




void CSwissKnifeDlg::OnButtonReg()
{
	m_ChildRegisterDlg.ShowWindow(TRUE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);
	// TODO: 在此添加命令处理程序代码
}


void CSwissKnifeDlg::OnButtonSoft()
{
	// TODO: 在此添加命令处理程序代码
	m_ChildSoftwareDlg.ShowWindow(TRUE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);
}


void CAboutDlg::OnSoftUnload()
{

	// TODO: 在此添加命令处理程序代码
}


HBRUSH CSwissKnifeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	//DWORD dwID = pWnd->GetDlgCtrlID();
	//// TODO:  在此更改 DC 的任何特性
	//if (IDC_STATIC == dwID || IDC_STATIC_MEM == dwID ||
	//	  IDC_STATIC_CPU == dwID)
	//{
	//	pDC->SetBkMode(TRANSPARENT);//设置透明输出
	//	pDC->SetTextColor(RGB(255, 255, 255));//设置文字颜色
	//	return m_brs;//用自定义的画刷去绘制背景
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}



void CSwissKnifeDlg::OnButtonServer()
{
	m_ChildServerDlg.ShowWindow(TRUE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);
	// TODO: 在此添加命令处理程序代码
}


void CSwissKnifeDlg::OnButtonStartup()
{
	// TODO: 在此添加命令处理程序代码
	m_ChildStartUpDlg.ShowWindow(TRUE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	m_ChildWindowDlg.ShowWindow(FALSE);
}


void CSwissKnifeDlg::OnButtonWindow()
{
	m_ChildWindowDlg.ShowWindow(TRUE);
	m_ChildStartUpDlg.ShowWindow(FALSE);
	m_ChildServerDlg.ShowWindow(FALSE);
	m_ChildFileDlg.ShowWindow(FALSE);
	m_ChildProcessDlg.ShowWindow(FALSE);
	m_ChildRegisterDlg.ShowWindow(FALSE);
	m_ChildSoftwareDlg.ShowWindow(FALSE);
	m_ChildCPUDlg.ShowWindow(FALSE);
	// TODO: 在此添加命令处理程序代码
}
