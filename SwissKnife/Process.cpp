// Process.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "Process.h"
#include "afxdialogex.h"


// CProcess 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

CProcess::CProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SWISSKNIFE_DIALOG, pParent)
{

}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
END_MESSAGE_MAP()


// CProcess 消息处理程序
