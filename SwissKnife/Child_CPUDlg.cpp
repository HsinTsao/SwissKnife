// Child_CPUDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_CPUDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <psapi.h>

// CChild_CPUDlg 对话框

IMPLEMENT_DYNAMIC(CChild_CPUDlg, CDialogEx)

CChild_CPUDlg::CChild_CPUDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CPU, pParent)
	, m_szStatus(_T(""))
{
	m_brs.CreateSolidBrush(RGB(0, 151, 209));
}

CChild_CPUDlg::~CChild_CPUDlg()
{
}

void CChild_CPUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS_CPU, m_ProcessCPU);
	DDX_Control(pDX, IDC_PROGRESS_MEM, m_ProcessMEM);
	DDX_Text(pDX, IDC_STATIC_STATE, m_szStatus);
}


BEGIN_MESSAGE_MAP(CChild_CPUDlg, CDialogEx)
	
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CChild_CPUDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CChild_CPUDlg 消息处理程序


BOOL CChild_CPUDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProcessCPU.SetRange32(0, 100);
	m_ProcessMEM.SetRange32(0, 100);

	m_nMovNum = 0;
	m_cpuUse = 0;
	m_memUse = 0;
	
	AfxBeginThread(DoSysCpu, (LPVOID)this);
	AfxBeginThread(DoSysMem, (LPVOID)this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




UINT CChild_CPUDlg::DoSysCpu(LPVOID pParam)
{
	CChild_CPUDlg *pthis = (CChild_CPUDlg *)pParam;
	CHostInfo      m_CPUInf;
	CString        showCpu;
	UINT           sumCpu = 0;
	BOOL           bFrist = TRUE;

	sumCpu = 1;
	sumCpu = sumCpu % 100;
	pthis->m_ProcessCPU.SetPos(sumCpu);
	showCpu.Format("%u %%", sumCpu);
	pthis->GetDlgItem(IDC_STATIC_CPU)->SetWindowText(showCpu);
	pthis->m_cpuUse = sumCpu;
	//pthis->UpdateWindow();
	pthis->Invalidate(FALSE);
	Sleep(1000);

	while (1)
	{
		/*if (bFrist == TRUE)
		{
			sumCpu = m_CPUInf.GetUsage();
			sumCpu = 1;
			bFrist = FALSE;
		}
		else
		{
			sumCpu = m_CPUInf.GetUsage();
		}*/
		
		sumCpu = m_CPUInf.GetUsage();

		sumCpu = sumCpu % 100;
		pthis->m_ProcessCPU.SetPos(sumCpu);
		showCpu.Format("%u %%", sumCpu);
		pthis->GetDlgItem(IDC_STATIC_CPU)->SetWindowText(showCpu);
		pthis->m_cpuUse = sumCpu;
		//pthis->UpdateWindow();
		pthis->Invalidate(FALSE);
		Sleep(1000);
	}

	return 0;
}



UINT CChild_CPUDlg::DoSysMem(LPVOID pParam)
{
	CChild_CPUDlg *pthis = (CChild_CPUDlg *)pParam;
	MEMORYSTATUSEX MemStat;
	MemStat.dwLength = sizeof(MemStat);
	unsigned long m_ulNewUsges;

	CString show = "";
	__int64 memSize;

	//pthis->m_ProcessMEM.SetBarColor(RGB(0,0,0));

	while (1)
	{
		GlobalMemoryStatusEx(&MemStat);
		m_ulNewUsges = MemStat.dwMemoryLoad;

		memSize = (__int64)MemStat.ullTotalPhys;
		show.Format("%uM", memSize*m_ulNewUsges / 1024 / 1024 / 100);

		pthis->m_ProcessMEM.SetPos(m_ulNewUsges);
		pthis->GetDlgItem(IDC_STATIC_MEM)->SetWindowText(show);
		pthis->m_memUse = m_ulNewUsges;
		::PostMessage(pthis->GetSafeHwnd(), WM_PAINT, 0, 0);
		Sleep(1000);

	}

	return 0;
}



void CChild_CPUDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CRect rc;
	GetClientRect(&rc);
	//设置主窗口背景色
	dc.FillSolidRect(rc, RGB(0, 151, 209));

	//dc.FillSolidRect(rc, RGB(255, 255, 255));
	//CDC *pDC1 = GetDlgItem(IDC_LINECPU)->GetDC();
	//pDC1->SetBkMode(TRANSPARENT);

	CDC *pDC = GetDlgItem(IDC_SHOWCPU)->GetDC();

	if (m_nMovNum != maxpix)
	{
		m_nMovNum++;
	}

	CDrawCpu(pDC);

	ReleaseDC(pDC);
}



BOOL CChild_CPUDlg::CDrawCpu(CDC *pDC)
{
	CRect rect;
	GetDlgItem(IDC_SHOWCPU)->GetClientRect(&rect);
	CDC dcMem; //用于缓冲作图的内存DC 
	CBitmap bmp; //内存中承载临时图象的位图 
	CBitmap *oldBmp;
	dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC 

	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());//创建兼容位图 

	oldBmp = dcMem.SelectObject(&bmp); //将位图选择进内存DC 

	int i = 1;
	//绘图

	//网格背景
	dcMem.FillSolidRect(rect, RGB(222, 222, 222));

	CPen pen;
	if (!pen.CreatePen(PS_DOT, 1, RGB(255, 255, 255)))
	{
		return FALSE;
	}

	CPen *pOldPen = dcMem.SelectObject(&pen);//保存旧画笔

	//横线
	for (i = 1; i < 10; i++)
	{
		dcMem.MoveTo(0, rect.Height()*i / 10);
		dcMem.LineTo(rect.Width(), rect.Height()*i / 10);
	}


	//竖线
	for (i = 1; i < 20; i++)
	{
		dcMem.MoveTo(rect.Width()*i / 20, 0);
		dcMem.LineTo(rect.Width()*i / 20, rect.Height());
	}


	//收回资源并释放
	dcMem.SelectObject(pOldPen);
	pen.DeleteObject();


	//绘制四边（防止闪烁）
	dcMem.MoveTo(0, 0);
	dcMem.LineTo(rect.Width(), 0);

	dcMem.MoveTo(0, rect.Height() - 1);
	dcMem.LineTo(rect.Width(), rect.Height() - 1);

	dcMem.MoveTo(0, 0);
	dcMem.LineTo(0, rect.Height());

	dcMem.MoveTo(rect.Width() - 1, 0);
	dcMem.LineTo(rect.Width() - 1, rect.Height());


	//绘制CPU运行状态线
	if (!pen.CreatePen(0, 2, RGB(0, 200, 0)))
	{
		return FALSE;
	}

	pOldPen = dcMem.SelectObject(&pen);//保存旧画笔

	pointCpu[0].x = rect.Width();
	pointCpu[0].y = rect.Height() - rect.Height()*m_cpuUse / 100;


	for (i = m_nMovNum; i > 0; i--)
	{
		if (i > 1)
		{
			dcMem.MoveTo(pointCpu[i - 2]);
			dcMem.LineTo(pointCpu[i - 1]);
		}

		pointCpu[i].x = pointCpu[i - 1].x - rect.Width() / maxpix - 5;
		pointCpu[i].y = pointCpu[i - 1].y;

	}

	dcMem.SelectObject(pOldPen);
	pen.DeleteObject();


	//绘制MEM运行状态线
	if (!pen.CreatePen(0, 2, RGB(0, 0, 200)))
	{
		return FALSE;
	}
	pOldPen = dcMem.SelectObject(&pen);//保存旧画笔

	pointMem[0].x = rect.Width();
	pointMem[0].y = rect.Height() - rect.Height()*m_memUse / 100;

	for (i = m_nMovNum; i > 0; i--)
	{
		if (i > 1)
		{
			dcMem.MoveTo(pointMem[i - 2]);
			dcMem.LineTo(pointMem[i - 1]);
		}

		pointMem[i].x = pointMem[i - 1].x - rect.Width() / maxpix - 5;
		pointMem[i].y = pointMem[i - 1].y;

	}

	dcMem.SelectObject(pOldPen);
	pen.DeleteObject();
	pOldPen = NULL;


	//将内存DC上的图象拷贝到前台 
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);


	//释放资源
	dcMem.SelectObject(oldBmp);
	dcMem.DeleteDC(); //删除DC
	bmp.DeleteObject(); //删除位图

	return TRUE;
}

HBRUSH CChild_CPUDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	int a = pWnd->GetDlgCtrlID();
	if (pWnd->GetDlgCtrlID() == IDC_LINECPU)
	{
		pDC->SetTextColor(RGB(0, 200, 0));
		return   m_hPage2Face;
	}


	DWORD dwID = pWnd->GetDlgCtrlID();
	// TODO:  在此更改 DC 的任何特性

	if (IDC_STATIC_CPU == dwID || IDC_STATIC_MEM == dwID ||
		IDC_STATIC == dwID || IDC_STATIC_STATE)
	{
		pDC->SetBkMode(TRANSPARENT);//设置透明输出
		pDC->SetTextColor(RGB(255, 255, 255));//设置文字颜色
		return m_brs;//用自定义的画刷去绘制背景
	}

	else if (dwID == IDC_STATIC_MEMLINE)
	{
		pDC->SetBkMode(TRANSPARENT);//设置透明输出
		pDC->SetTextColor(RGB(0, 0, 200));
		return   m_brs;
	}

	else if(dwID == IDC_STATIC_CPULINE)
	{
		pDC->SetBkMode(TRANSPARENT);//设置透明输出
		pDC->SetTextColor(RGB(0, 200, 0));
		return   m_brs;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	return hbr;
}


void CChild_CPUDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_szStatus = _T("正在清理内存中... ...");
	UpdateData(FALSE);
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	m_szStatus.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);
	UpdateData(FALSE);
}


void CChild_CPUDlg::ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;

	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}



