#include "pch.h"
#include <direct.h>
#include <io.h>

/*
Executable modules, ��Ŀ 126
 ��ַ=78BD0000
 ��С=02624000 (39993344.)
 ���=7A0FC59D WeChatWi.<ģ����ڵ�>
 ����=WeChatWi
 �ļ��汾=3.5.0.46
 ·��=D:\Tencent\WeChat\[3.5.0.46]\WeChatWin.dll

78FC8980    E8 FB730000     call    78FCFD80                         ; ����λ�ã�eax-3C����С��eax-48
*/

#define SaveGifHookOffset (0x5D9A2E83 - 0x5D5B0000)
#define NextCallOffset (0x5D9A8910 - 0x5D5B0000)
CHAR originalRecieveCode[5] = { 0 };

BYTE bjmpcode[5] = { 0 };
DWORD SaveGifHookAddress = GetWeChatWinBase() + SaveGifHookOffset;
DWORD dwReternAddress = GetWeChatWinBase() + SaveGifHookOffset + 5;
DWORD NextCallAddr = GetWeChatWinBase() + NextCallOffset;
bool SaveGifHooked = false;

void  __declspec(naked) ExtractExpression()
{
	__asm
	{
		pushad;
		push eax;
		call OutputExpression;
		popad;
		call NextCallAddr;
		//���ط��ص�ַ
		jmp dwReternAddress;
	}
}

void HookExtractExpression()
{
	HookAnyAddress(SaveGifHookAddress, ExtractExpression,originalRecieveCode);
	SaveGifHooked = true;
}

void CreateDir(const char* dir)
{
	int m = 0, n;
	string str1, str2;

	str1 = dir;
	str2 = str1.substr(0, 2);
	str1 = str1.substr(3, str1.size());

	while (m >= 0)
	{
		m = str1.find('\\');
		str2 += '\\' + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //�жϸ�Ŀ¼�Ƿ����
		if (n == -1)
		{
			int status = _mkdir(str2.c_str());     //����Ŀ¼
		}
		str1 = str1.substr(m + 1, str1.size());
	}
}

void CreateFileWithCurrentTime(char* filedir, char* filepostfix, DWORD filedata, DWORD filedatalen)
{
	//��ȡ��ǰʱ����Ϊ�ļ���
	time_t rawtime;
	struct tm* ptminfo = new struct tm;
	time(&rawtime);
	localtime_s(ptminfo, &rawtime);
	char currenttime[30] = { 0 };
	sprintf_s(currenttime, "%02d%02d%02d%02d%02d%02d", ptminfo->tm_year + 1900,
		ptminfo->tm_mon + 1, ptminfo->tm_mday, ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

	//ƴ��������·��
	char filepath[MAX_PATH] = { 0 };
	sprintf_s(filepath, "%s%s%s", filedir, currenttime, filepostfix);
	//�����ļ�
	HANDLE hFile = CreateFileA(filepath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "�����ļ�ʧ��", "����", 0);
		return;
	}
	//д���ļ�
	DWORD dwWrite = 0;
	WriteFile(hFile, (LPCVOID)filedata, filedatalen, &dwWrite, 0);
	//�رվ��
	CloseHandle(hFile);
}

void __stdcall OutputExpression(DWORD dwExpressionAddr)
{
	//ȡ��ͼƬ����
	DWORD dwImageLen = *((DWORD*)(dwExpressionAddr + 0x4));
	//ȡ��ͼƬ����
	DWORD dwImage = *((DWORD*)(dwExpressionAddr));
	if (dwImageLen == 0) {
		return;
	}
	unsigned char magic_head[4] = { 0 };
	char postfix[5] = { 0 };
	memcpy(magic_head, (void*)dwImage, 3);
	//����MAGICͷ�жϺ�׺
	if (magic_head[0] == 137 && magic_head[1] == 80 && magic_head[2] == 78)
	{
		strcpy_s(postfix, 5, ".png");
	}
	else if (magic_head[0] == 71 && magic_head[1] == 73 && magic_head[2] == 70)
	{
		strcpy_s(postfix, 5, ".gif");
	}
	else if (magic_head[0] == 255 && magic_head[1] == 216 && magic_head[2] == 255)
	{
		strcpy_s(postfix, 5, ".jpg");
	}
	//��ȡ��ʱ�ļ���Ŀ¼
	char temppath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, temppath);
	char imagedir[25] = { "WeChatRecordExpressions" };

	//ƴ�Ӵ��΢�ű����Ŀ¼
	char WeChatExpressionsPath[MAX_PATH] = { 0 };
	sprintf_s(WeChatExpressionsPath, "%s%s\\", temppath, imagedir);
	//����Ŀ¼���ͼƬ
	CreateDir(WeChatExpressionsPath);

	//�������ͼƬ
	CreateFileWithCurrentTime(WeChatExpressionsPath, postfix, dwImage, dwImageLen);
}

void UnHookExtractExpression()
{
	// �ظ������ǵ�ָ��
	if (!SaveGifHooked)
		return;
	UnHookAnyAddress(SaveGifHookAddress, originalRecieveCode);
	PostMessage(HWND_BROADCAST, NULL, 0, 0);
	SaveGifHooked = false;
}