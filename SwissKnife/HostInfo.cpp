// HostInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "HostInfo.h"
#include "afxdialogex.h"

#define SystemBasicInformation       0
#define SystemPerformanceInformation 2
#define SystemTimeInformation        3
#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))


typedef struct
{
	DWORD   dwUnknown1;
	ULONG   uKeMaximumIncrement;
	ULONG   uPageSize;
	ULONG   uMmNumberOfPhysicalPages;
	ULONG   uMmLowestPhysicalPage;
	ULONG   uMmHighestPhysicalPage;
	ULONG   uAllocationGranularity;
	PVOID   pLowestUserAddress;
	PVOID   pMmHighestUserAddress;
	ULONG   uKeActiveProcessors;
	BYTE    bKeNumberProcessors;
	BYTE    bUnknown2;
	WORD    wUnknown3;
} SYSTEM_BASIC_INFORMATION;

typedef struct
{
	LARGE_INTEGER   liIdleTime;
	DWORD           dwSpare[76];
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
	LARGE_INTEGER liKeBootTime;
	LARGE_INTEGER liKeSystemTime;
	LARGE_INTEGER liExpTimeZoneBias;
	ULONG         uCurrentTimeZoneId;
	DWORD         dwReserved;
} SYSTEM_TIME_INFORMATION;


// CHostInfo 对话框
CHostInfo::CHostInfo()
	
{

}

CHostInfo::~CHostInfo()
{
}



// CHostInfo 消息处理程序

int CHostInfo::GetUsage()
{
	EnableDebugPrivilege();
	if (GetVersion() < 0x80000000) // current operation system is NT
		return GetNTCPUInf();
	else
		return Get9XCPUInf();

	return 0;
}

int CHostInfo::GetNTCPUInf()
{
#ifdef _WIN64
	
	return 0;
#else
	
	int nNewUsges;
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo;
	SYSTEM_TIME_INFORMATION        SysTimeInfo;
	SYSTEM_BASIC_INFORMATION       SysBaseInfo;
	double                         dbIdleTime;
	double                         dbSystemTime;
	typedef LONG(WINAPI *PROCNTQSI)(UINT, PVOID, ULONG, PULONG);
	PROCNTQSI NtQuerySystemInformation;

	if (!(NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"), "NtQuerySystemInformation")))
		return 0;

	if (NtQuerySystemInformation(SystemBasicInformation, &SysBaseInfo, sizeof(SysBaseInfo), NULL) != NO_ERROR)
		return 0;

	if (NtQuerySystemInformation(SystemTimeInformation, &SysTimeInfo, sizeof(SysTimeInfo), 0) != NO_ERROR)
		return 0;

	if (NtQuerySystemInformation(SystemPerformanceInformation, &SysPerfInfo, sizeof(SysPerfInfo), NULL) != NO_ERROR)
		return 0;

	// 第一次跳过
	if (m_liOldIdleTime.QuadPart != 0)
	{
		// CurrentValue = NewValue - OldValue
		dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(m_liOldIdleTime);
		dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(m_liOldSystemTime);

		//（总的时间-空闲时间）/总的时间=占用cpu的时间就是使用率
		dbIdleTime = dbIdleTime / dbSystemTime;

		//(1 - 空闲时间)/处理器个数
		dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;
		nNewUsges = (int)dbIdleTime;
	}

	// 保存
	m_liOldIdleTime = SysPerfInfo.liIdleTime;
	m_liOldSystemTime = SysTimeInfo.liKeSystemTime;

	return nNewUsges;

#endif 

	
}



int CHostInfo::Get9XCPUInf()
{
	HKEY hkey;
	DWORD dwDataSize;
	DWORD dwType;
	DWORD dwCpuUsage;
	int nNewUsges;

	// starting the counter
	if (RegOpenKeyEx(HKEY_DYN_DATA, "PerfStats\\StartStat", 0, KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS)
		return 0;
	dwDataSize = sizeof(DWORD);
	if (RegQueryValueEx(hkey, "KERNEL\\CPUUsage", NULL, &dwType, (LPBYTE)&dwCpuUsage, &dwDataSize) != ERROR_SUCCESS)
		return 0;
	RegCloseKey(hkey);

	// geting current counter's value
	if (RegOpenKeyEx(HKEY_DYN_DATA, "PerfStats\\StatData", 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return 0;
	dwDataSize = sizeof(DWORD);
	if (RegQueryValueEx(hkey, "KERNEL\\CPUUsage", NULL, &dwType, (LPBYTE)&dwCpuUsage, &dwDataSize) != ERROR_SUCCESS)
		return 0;
	nNewUsges = dwCpuUsage;
	RegCloseKey(hkey);

	// stoping the counter
	if (RegOpenKeyEx(HKEY_DYN_DATA, "PerfStats\\StopStat", 0, KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS)
		return 0;
	dwDataSize = sizeof(DWORD);
	if (RegQueryValueEx(hkey, "KERNEL\\CPUUsage", NULL, &dwType, (LPBYTE)&dwCpuUsage, &dwDataSize) != ERROR_SUCCESS)
		return 0;
	RegCloseKey(hkey);

	return nNewUsges;
}

BOOL CHostInfo::EnableDebugPrivilege()
{
	HANDLE TokenHandle = NULL;
	TOKEN_PRIVILEGES TokenPrivilege;
	LUID uID;

	//打开权限令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))   //Open  查看 Set(10)  Close     [张飞][10]   [关羽][20]   
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID))
	{
		CloseHandle(TokenHandle);
		TokenHandle = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	TokenPrivilege.PrivilegeCount = 1;
	TokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	TokenPrivilege.Privileges[0].Luid = uID;

	//在这里我们进行调整权限
	if (!AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivilege, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		CloseHandle(TokenHandle);
		TokenHandle = INVALID_HANDLE_VALUE;
		return  FALSE;
	}

	CloseHandle(TokenHandle);
	TokenHandle = INVALID_HANDLE_VALUE;
	return TRUE;

}
