#pragma once


// CHostInfo 对话框

class CHostInfo 
{


public:
	CHostInfo();   // 标准构造函数
	virtual ~CHostInfo();


public:
	LARGE_INTEGER   m_liOldIdleTime;
	LARGE_INTEGER   m_liOldSystemTime;
	int GetUsage();

	int GetNTCPUInf();

	int Get9XCPUInf();

	//提高权限至Dug
	BOOL EnableDebugPrivilege();
	
};
