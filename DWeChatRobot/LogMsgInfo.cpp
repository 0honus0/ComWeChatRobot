#include "pch.h"

// ΢����־HOOK��ַƫ��
#define HookLogMsgInfoAddrOffset 0x103408A4 - 0x0FC40000
// HOOK��CALLƫ��
#define HookLogMsgInfoNextCallOffset 0x11586DFC - 0x0FC40000
// HOOK����ת��ַƫ��
#define HookLogMsgJmpBackOffset 0x103408A9 - 0x0FC40000

// ΢����־HOOK��ַ
DWORD HookLogMsgInfoAddr = GetWeChatWinBase() + HookLogMsgInfoAddrOffset;
// HOOK��CALL��ַ
DWORD NextCallAddr = GetWeChatWinBase() + HookLogMsgInfoNextCallOffset;
// HOOK����ת��ַ
DWORD JmpBackAddr = GetWeChatWinBase() + HookLogMsgJmpBackOffset;

// �Ƿ�����־HOOK��־
BOOL LogMsgHooked = false;
// ����HOOKǰ��ָ�����ڻָ�
char LogOldAsmCode[5] = { 0 };

/*
* ����������ӡ��־��Ϣ
* msg����־��Ϣ
* return��void
*/
VOID PrintMsg(DWORD msg) {
	if (!msg)
		return;
	string message = (char*)msg;
	cout << message;
	return;
}

/*
* HOOK�ľ���ʵ�֣�������־�����ô�����
*/
__declspec(naked) void doprintmsg(){
	__asm {
		pushad;
		pushfd;
		push eax;
		call PrintMsg;
		add esp, 0x4;
		popfd;
		popad;
		call NextCallAddr;
		jmp JmpBackAddr;
	}
}

/*
* ��ʼHOOK΢����־
* return��void
*/
VOID HookLogMsgInfo() {
	if (LogMsgHooked)
		return;
	HookAnyAddress(HookLogMsgInfoAddr,(LPVOID)doprintmsg, LogOldAsmCode);
	LogMsgHooked = true;
}

/*
* ֹͣHOOK΢����־
* return��void
*/
VOID UnHookLogMsgInfo() {
	if (!LogMsgHooked)
		return;
	UnHookAnyAddress(HookLogMsgInfoAddr, LogOldAsmCode);
	LogMsgHooked = false;
}