#pragma once
#include<windows.h>
/*
* �ⲿ����ʱ���ݵĲ�������
* DbHandle��Ҫ���ݵ����ݿ���
* BackupFile�����ݵı���λ��
*/
struct BackupStruct {
	DWORD DbHandle;
	char* BackupFile;
};
int BackupSQLiteDB(DWORD DbHandle, const char* BackupFile);
extern "C" __declspec(dllexport) int BackupSQLiteDBRemote(LPVOID lpParameter);