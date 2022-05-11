#pragma once
#include<windows.h>
#include<vector>

/*
* �������ݿⵥ������Ϣ�Ľṹ��
* name��������l_name��`name`�ַ���
* tbl_name��������l_tbl_name��`tbl_name`�ַ���
* sql��������䣻l_sql��`sql`�ַ���
* rootpage�����ţ�l_rootpage��`rootpage`�ַ���
*/
struct TableInfoStruct {
    char* name;
    DWORD l_name;
    char* tbl_name;
    DWORD l_tbl_name;
    char* sql;
    DWORD l_sql;
    char* rootpage;
    DWORD l_rootpage;
};

/*
* �������ݿ���Ϣ�Ľṹ��
* handle�����ݿ���
* dbname�����ݿ���
* l_dbname��`dbname`�ַ���
* tables������������б���Ϣ������
* count�����б������
*/
struct DbInfoStruct {
    DWORD handle;
    wchar_t* dbname;
    DWORD l_dbname;
    vector<TableInfoStruct> tables;
    DWORD count;
};

void GetDbHandles();
extern "C" __declspec(dllexport) DWORD GetDbHandlesRemote();
DWORD GetDbHandleByDbName(wchar_t* dbname);