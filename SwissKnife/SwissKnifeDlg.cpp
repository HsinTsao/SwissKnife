
// SwissKnifeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "SwissKnifeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSwissKnifeDlg �Ի���



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


// CSwissKnifeDlg ��Ϣ�������

BOOL CSwissKnifeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//���ñ���
	SetBackgroundImage(IDB_BITMAP_BACK);
	//�������ToolBar
	CreateToolBar();
	//��ʼ���ؼ�
	InitControl();
	//�����Ӵ���
	if (m_ChildProcessDlg.Create(IDD_DIALOG_PROCESS) == FALSE)
	{
		MessageBox("���������Ӵ���ʧ��","ERROR");
	}

	if (m_ChildFileDlg.Create(IDD_DIALOG_FILE) == FALSE)
	{
		MessageBox("�����ļ��Ӵ���ʧ��", "ERROR");
	}
	if (m_ChildRegisterDlg.Create(IDD_DIALOG_REG) == FALSE)
	{
		MessageBox("����ע����Ӵ���ʧ��", "ERROR");
	}
	
	if (m_ChildSoftwareDlg.Create(IDD_DIALOG_SOFTWARE) == FALSE)
	{
		MessageBox("��������Ӵ���ʧ��", "ERROR");
	}
	if (m_ChildCPUDlg.Create(IDD_DIALOG_CPU) == FALSE)
	{
		MessageBox("����CPU�Ӵ���ʧ��", "ERROR");
	}
	if (m_ChildServerDlg.Create(IDD_DIALOG_SERVER) == FALSE)
	{
		MessageBox("���������Ӵ���ʧ��", "ERROR");
	}
	if (m_ChildStartUpDlg.Create(IDD_DIALOG_STARTUP) == FALSE)
	{
		MessageBox("�����������Ӵ���ʧ��", "ERROR");
	}
	if (m_ChildWindowDlg.Create(IDD_DIALOG_WINDOW) == FALSE)
	{
		MessageBox("���������Ӵ���ʧ��", "ERROR");
	}
	
	

	DWORD dwThreadID = 0;
	//HANDLE hThread = CreateThread(NULL, 0, MonitorThread, 0, NULL, &dwThreadID);//���������߳�
	
	
	//��ʼ���Ӵ���λ��
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSwissKnifeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSwissKnifeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CSwissKnifeDlg::CreateToolBar()
{
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_ICON))  //����һ��������  ������Դ
	{

		return;
	}
	m_ToolBar.LoadTrueColorToolBar
	(
		48,    //������ʹ�����
		IDB_BITMAP_TOOLBAR,
		IDB_BITMAP_TOOLBAR,
		IDB_BITMAP_TOOLBAR
	);  //�����ǵ�λͼ��Դ�����



	RECT Rect, RectMain;
	GetWindowRect(&RectMain);   //�õ��������ڵĴ�С
	Rect.left = -5;
	Rect.top = 0;
	Rect.bottom = 80;
	Rect.right = RectMain.right - RectMain.left + 10;
	m_ToolBar.MoveWindow(&Rect, TRUE);

	m_ToolBar.SetButtonText(0, "������Ϣ");     //��λͼ����������ļ�
	m_ToolBar.SetButtonText(1, "���̹���");
	m_ToolBar.SetButtonText(2, "�ļ�����");
	m_ToolBar.SetButtonText(3, "ע������");
	m_ToolBar.SetButtonText(4, "���������");
	m_ToolBar.SetButtonText(5, "�������");
	m_ToolBar.SetButtonText(6, "���ڹ���");
	m_ToolBar.SetButtonText(7, "�������");
	m_ToolBar.SetButtonText(8, "�������");
	/*m_ToolBar.SetButtonText(9, L"��������");
	m_ToolBar.SetButtonText(10,L"���ɷ����");
	m_ToolBar.SetButtonText(11,L"����");*/
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);  //��ʾ
	return VOID();
}



void CSwissKnifeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	if (SIZE_MINIMIZED == nType)
	{
		return;
	}

	//if (m_CList_Show.m_hWnd != NULL)   //���ؼ�Ҳ�Ǵ������Ҳ�о����
	//{
	//	CRect Rect;
	//	Rect.left = 1;          //�б��������     
	//	Rect.top = 80;         //�б��������
	//	Rect.right = cx - 1;    //�б��������
	//	Rect.bottom = cy - 160;   //�б��������
	//	m_CList_Show.MoveWindow(Rect);
	//}

	if (m_ToolBar.m_hWnd != NULL)                  //������
	{
		CRect Rect;
		Rect.top = 0;
		Rect.left = -5;
		Rect.right = cx;
		Rect.bottom = 80;
		m_ToolBar.MoveWindow(Rect);             //���ù�������Сλ��
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������

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

	// TODO: �ڴ���������������
}

DWORD CSwissKnifeDlg::MonitorThread(LPVOID lpParam)
{
	/*CChild_UDiskDlg    m_ChildUDiskDlg;
	
	if (m_ChildUDiskDlg.Create(IDD_DIALOG_UDISK) == FALSE)
	{
		int a = GetLastError();
		::MessageBox(0,"����U���Ӵ���ʧ��", "ERROR",0);
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
	// TODO: �ڴ���������������
}


void CSwissKnifeDlg::OnButtonSoft()
{
	// TODO: �ڴ���������������
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

	// TODO: �ڴ���������������
}


HBRUSH CSwissKnifeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	//DWORD dwID = pWnd->GetDlgCtrlID();
	//// TODO:  �ڴ˸��� DC ���κ�����
	//if (IDC_STATIC == dwID || IDC_STATIC_MEM == dwID ||
	//	  IDC_STATIC_CPU == dwID)
	//{
	//	pDC->SetBkMode(TRANSPARENT);//����͸�����
	//	pDC->SetTextColor(RGB(255, 255, 255));//����������ɫ
	//	return m_brs;//���Զ���Ļ�ˢȥ���Ʊ���
	//}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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
	// TODO: �ڴ���������������
}


void CSwissKnifeDlg::OnButtonStartup()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
}
