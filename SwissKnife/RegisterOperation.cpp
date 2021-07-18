// RegisterOperation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SwissKnife.h"
#include "RegisterOperation.h"
#include "afxdialogex.h"
#include <IOSTREAM>
using namespace std;

// CRegisterOperation �Ի���

IMPLEMENT_DYNAMIC(CRegisterOperation, CDialogEx)

enum MYKEY {
	MHKEY_CLASSES_ROOT,
	MHKEY_CURRENT_USER,
	MHKEY_LOCAL_MACHINE,
	MHKEY_USERS,
	MHKEY_CURRENT_CONFIG
};


enum KEYVALUE {
	MREG_SZ,
	MREG_DWORD,
	MREG_BINARY,
	MREG_EXPAND_SZ
};

struct REGMSG {
	int count;         //���ָ���
	DWORD size;             //���ִ�С
	DWORD valsize;     //ֵ��С

};

CRegisterOperation::CRegisterOperation(char bToken, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_REG, pParent)
{
	switch (bToken) {
	case MHKEY_CLASSES_ROOT:
		MKEY = HKEY_CLASSES_ROOT;
		break;
	case MHKEY_CURRENT_USER:
		MKEY = HKEY_CURRENT_USER;
		break;
	case MHKEY_LOCAL_MACHINE:
		MKEY = HKEY_LOCAL_MACHINE;
		break;
	case MHKEY_USERS:
		MKEY = HKEY_USERS;
		break;
	case MHKEY_CURRENT_CONFIG:
		MKEY = HKEY_CURRENT_CONFIG;
		break;
	default:
		MKEY = HKEY_LOCAL_MACHINE;
		break;
	}

	ZeroMemory(KeyPath, MAX_PATH);
}

CRegisterOperation::~CRegisterOperation()
{
}

void CRegisterOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterOperation, CDialogEx)
END_MESSAGE_MAP()


// CRegisterOperation ��Ϣ�������


char* CRegisterOperation::FindPath()
{
	char* szBuffer = NULL;
	HKEY		hKey;			//ע����ؾ��
								/*��ע��� User  kdjfjkf\kdjfkdjf\  */
	if (RegOpenKeyEx(MKEY, KeyPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)//��
	{
		DWORD dwIndex = 0, NameSize, NameCount, NameMaxLen, Type;
		DWORD KeySize, KeyCount, KeyMaxLen, DateSize, MaxDateLen;
		//�����ö����
		if (RegQueryInfoKey(hKey, NULL, NULL, NULL, &KeyCount,  //14
			&KeyMaxLen, NULL, &NameCount, &NameMaxLen, &MaxDateLen, NULL, NULL) != ERROR_SUCCESS)
		{

			return NULL;
		}
		//һ�㱣����ʩ
		KeySize = KeyMaxLen + 1;
		if (KeyCount>0 && KeySize>1)
		{
			int Size = sizeof(REGMSG) + 1;

			//buf=new char[KeyCnt*KeySize+size+1];
			DWORD DataSize = KeyCount*KeySize + Size + 1;    //[TOKEN_REG_PATH][2 11 ccccc\0][11][11]
			szBuffer = (char*)LocalAlloc(LPTR, DataSize);
			ZeroMemory(szBuffer, DataSize);
			szBuffer[0] = TOKEN_REG_PATH;           //����ͷ
			REGMSG msg;                     //����ͷ
			msg.size = KeySize;
			msg.count = KeyCount;
			memcpy(szBuffer + 1, (void*)&msg, Size);

			char* szTemp = new char[KeySize];
			for (dwIndex = 0; dwIndex<KeyCount; dwIndex++)		//ö����
			{
				ZeroMemory(szTemp, KeySize);
				DWORD i = KeySize;  //22
				RegEnumKeyEx(hKey, dwIndex, szTemp, &i, NULL, NULL, NULL, NULL);
				//char* TempBuffer = szBuffer[dwIndex*KeySize + Size];
				//memcpy(&szBuffer[dwIndex*KeySize + Size], szTemp, sizeof(szTemp));
				int a = sizeof(szTemp);
				strcpy_s(szBuffer + dwIndex*KeySize + Size, KeySize, szTemp);
				//szBuffer[dwIndex+1] = szTemp;
			}
			delete[] szTemp;
			RegCloseKey(hKey);

		}

	}

	return szBuffer;
}


void CRegisterOperation::SetPath(char *szPath)
{
	ZeroMemory(KeyPath, MAX_PATH);
	int a = strlen(szPath);
	//strcpy_s(KeyPath, MAX_PATH, szPath);
	memcpy(KeyPath, szPath, a);
}




char* CRegisterOperation::FindKey()
{
	char	*szValueName;		//��ֵ����
	char	*szKeyName ;			//�Ӽ�����
	LPBYTE	 szValueDate;		//��ֵ����

	char *szBuffer = NULL;
	HKEY  hKey;			//ע����ؾ��
	if (RegOpenKeyEx(MKEY, KeyPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)//��
	{
		DWORD dwIndex = 0, NameSize = 0, NameCount, NameMaxLen, Type;
		DWORD KeySize, KeyCount, KeyMaxLen, DataSize, MaxDateLen;
		//�����ö����
		if (RegQueryInfoKey(hKey, NULL, NULL, NULL,
			&KeyCount, &KeyMaxLen, NULL, &NameCount, &NameMaxLen, &MaxDateLen, NULL, NULL) != ERROR_SUCCESS)
		{

			return NULL;
		}
		if (NameCount>0 && MaxDateLen>0 /*&& NameSize>0*/)
		{
			DataSize = MaxDateLen + 1;
			NameSize = NameMaxLen + 100;
			REGMSG  msg;
			msg.count = NameCount;        //�ܸ���
			msg.size = NameSize;          //���ִ�С
			msg.valsize = DataSize;       //���ݴ�С
			int msgsize = sizeof(REGMSG);
			// ͷ                   ���            ����                ����
			DWORD size = sizeof(REGMSG) +
				sizeof(BYTE)*NameCount + NameSize*NameCount + DataSize*NameCount + 10;
			szBuffer = (char*)LocalAlloc(LPTR, size);
			ZeroMemory(szBuffer, size);
			szBuffer[0] = TOKEN_REG_KEY;         //����ͷ
			memcpy(szBuffer + 1, (void*)&msg, msgsize);     //����ͷ

			szValueName = (char *)malloc(NameSize);
			szValueDate = (LPBYTE)malloc(DataSize);

			char *szTemp = szBuffer + msgsize + 1;
			for (dwIndex = 0; dwIndex<NameCount; dwIndex++)	//ö�ټ�ֵ
			{
				ZeroMemory(szValueName, NameSize);
				ZeroMemory(szValueDate, DataSize);

				DataSize = MaxDateLen + 1;
				NameSize = NameMaxLen + 100;

				RegEnumValue(hKey, dwIndex, szValueName, &NameSize,
					NULL, &Type, szValueDate, &DataSize);//��ȡ��ֵ

				if (Type == REG_SZ)
				{
					szTemp[0] = MREG_SZ;
				}
				if (Type == REG_DWORD)
				{

					szTemp[0] = MREG_DWORD;
				}
				if (Type == REG_BINARY)
				{
					szTemp[0] = MREG_BINARY;
				}
				if (Type == REG_EXPAND_SZ)
				{
					szTemp[0] = MREG_EXPAND_SZ;
				}
				szTemp += sizeof(BYTE);
				//strcpy_s(szTemp, strlen(szValueName), szValueName);
				memcpy(szTemp, szValueName, strlen(szValueName));
				szTemp += msg.size;
				memcpy(szTemp, szValueDate, msg.valsize);
				szTemp += msg.valsize;
			}
			free(szValueName);
			free(szValueDate);

		}

	}
	return szBuffer;
}