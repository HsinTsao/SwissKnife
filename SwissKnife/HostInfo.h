#pragma once


// CHostInfo �Ի���

class CHostInfo 
{


public:
	CHostInfo();   // ��׼���캯��
	virtual ~CHostInfo();


public:
	LARGE_INTEGER   m_liOldIdleTime;
	LARGE_INTEGER   m_liOldSystemTime;
	int GetUsage();

	int GetNTCPUInf();

	int Get9XCPUInf();

	//���Ȩ����Dug
	BOOL EnableDebugPrivilege();
	
};
