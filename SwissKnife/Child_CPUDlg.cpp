// Child_CPUDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Child_CPUDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <psapi.h>

// CChild_CPUDlg �Ի���

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


// CChild_CPUDlg ��Ϣ�������


BOOL CChild_CPUDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ProcessCPU.SetRange32(0, 100);
	m_ProcessMEM.SetRange32(0, 100);

	m_nMovNum = 0;
	m_cpuUse = 0;
	m_memUse = 0;
	
	AfxBeginThread(DoSysCpu, (LPVOID)this);
	AfxBeginThread(DoSysMem, (LPVOID)this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect rc;
	GetClientRect(&rc);
	//���������ڱ���ɫ
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
	CDC dcMem; //���ڻ�����ͼ���ڴ�DC 
	CBitmap bmp; //�ڴ��г�����ʱͼ���λͼ 
	CBitmap *oldBmp;
	dcMem.CreateCompatibleDC(pDC); //��������DC���������ڴ�DC 

	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());//��������λͼ 

	oldBmp = dcMem.SelectObject(&bmp); //��λͼѡ����ڴ�DC 

	int i = 1;
	//��ͼ

	//���񱳾�
	dcMem.FillSolidRect(rect, RGB(222, 222, 222));

	CPen pen;
	if (!pen.CreatePen(PS_DOT, 1, RGB(255, 255, 255)))
	{
		return FALSE;
	}

	CPen *pOldPen = dcMem.SelectObject(&pen);//����ɻ���

	//����
	for (i = 1; i < 10; i++)
	{
		dcMem.MoveTo(0, rect.Height()*i / 10);
		dcMem.LineTo(rect.Width(), rect.Height()*i / 10);
	}


	//����
	for (i = 1; i < 20; i++)
	{
		dcMem.MoveTo(rect.Width()*i / 20, 0);
		dcMem.LineTo(rect.Width()*i / 20, rect.Height());
	}


	//�ջ���Դ���ͷ�
	dcMem.SelectObject(pOldPen);
	pen.DeleteObject();


	//�����ıߣ���ֹ��˸��
	dcMem.MoveTo(0, 0);
	dcMem.LineTo(rect.Width(), 0);

	dcMem.MoveTo(0, rect.Height() - 1);
	dcMem.LineTo(rect.Width(), rect.Height() - 1);

	dcMem.MoveTo(0, 0);
	dcMem.LineTo(0, rect.Height());

	dcMem.MoveTo(rect.Width() - 1, 0);
	dcMem.LineTo(rect.Width() - 1, rect.Height());


	//����CPU����״̬��
	if (!pen.CreatePen(0, 2, RGB(0, 200, 0)))
	{
		return FALSE;
	}

	pOldPen = dcMem.SelectObject(&pen);//����ɻ���

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


	//����MEM����״̬��
	if (!pen.CreatePen(0, 2, RGB(0, 0, 200)))
	{
		return FALSE;
	}
	pOldPen = dcMem.SelectObject(&pen);//����ɻ���

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


	//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨ 
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);


	//�ͷ���Դ
	dcMem.SelectObject(oldBmp);
	dcMem.DeleteDC(); //ɾ��DC
	bmp.DeleteObject(); //ɾ��λͼ

	return TRUE;
}

HBRUSH CChild_CPUDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	int a = pWnd->GetDlgCtrlID();
	if (pWnd->GetDlgCtrlID() == IDC_LINECPU)
	{
		pDC->SetTextColor(RGB(0, 200, 0));
		return   m_hPage2Face;
	}


	DWORD dwID = pWnd->GetDlgCtrlID();
	// TODO:  �ڴ˸��� DC ���κ�����

	if (IDC_STATIC_CPU == dwID || IDC_STATIC_MEM == dwID ||
		IDC_STATIC == dwID || IDC_STATIC_STATE)
	{
		pDC->SetBkMode(TRANSPARENT);//����͸�����
		pDC->SetTextColor(RGB(255, 255, 255));//����������ɫ
		return m_brs;//���Զ���Ļ�ˢȥ���Ʊ���
	}

	else if (dwID == IDC_STATIC_MEMLINE)
	{
		pDC->SetBkMode(TRANSPARENT);//����͸�����
		pDC->SetTextColor(RGB(0, 0, 200));
		return   m_brs;
	}

	else if(dwID == IDC_STATIC_CPULINE)
	{
		pDC->SetBkMode(TRANSPARENT);//����͸�����
		pDC->SetTextColor(RGB(0, 200, 0));
		return   m_brs;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	return hbr;
}


void CChild_CPUDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_szStatus = _T("���������ڴ���... ...");
	UpdateData(FALSE);
	//��ȡ����ǰ���ڴ���Ϣ
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//��ʼ�����ڴ�
	ClearMemory();
	//��ȡ�������ڴ���Ϣ
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//���������Ϣ
	m_szStatus.Format(_T("�ڴ�������ϣ��������� : %.2f MB"), preUsedMem - afterUsedMem);
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

	//��ȡϵͳ�е�һ�����̵���Ϣ  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//���ý��̹�������С
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//�����ܶ�Ľ�ָ�����̵�ҳ��ӹ������Ƴ�
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//��ȡ��һ�����̵���Ϣ  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}



