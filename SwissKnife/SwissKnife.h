
// SwissKnife.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSwissKnifeApp: 
// �йش����ʵ�֣������ SwissKnife.cpp
//

class CSwissKnifeApp : public CWinApp
{
public:
	CSwissKnifeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSwissKnifeApp theApp;