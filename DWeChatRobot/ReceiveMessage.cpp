#include "pch.h"
#include <vector>

// ������Ϣ��HOOK��ַƫ��
#define ReceiveMessageHookOffset 0x547C0F4C - 0x54270000
// ������ϢHOOK��CALLƫ��
#define ReceiveMessageNextCallOffset 0x54D04E60 - 0x54270000

// ������Ϣ��HOOK��ַƫ��
#define SendMessageHookOffset 0x102C8E32 - 0x0FDE0000
// ������ϢHOOK��CALLƫ��
#define SendMessageNextCallOffset 0x101E8170 - 0x0FDE0000

/*
* ���浥����Ϣ�Ľṹ
* messagetype����Ϣ����
* sender��������wxid��l_sender��`sender`�ַ���
* wxid�����sender��Ⱥ��id����˳�Ա������巢����wxid��������`sender`һ�£�l_wxid��`wxid`�ַ���
* message����Ϣ���ݣ����ı���Ϣ��xml��ʽ��l_message��`message`�ַ���
* filepath��ͼƬ���ļ���������Դ�ı���·����l_filepath��`filepath`�ַ���
*/
struct messageStruct {
	DWORD messagetype;
	BOOL isSendMessage;
	wchar_t* sender;
	DWORD l_sender;
	wchar_t* wxid;
	DWORD l_wxid;
	wchar_t* message;
	DWORD l_message;
	wchar_t* filepath;
	DWORD l_filepath;
	wchar_t* time;
	DWORD l_time;
};

// ���������Ϣ�Ķ�̬����
vector<messageStruct> messageVector;

// �Ƿ���������ϢHOOK��־
BOOL ReceiveMessageHooked = false;
// ����HOOKǰ���ֽ��룬���ڻָ�
char OldReceiveMessageAsmCode[5] = { 0 };
char OldSendMessageAsmCode[5] = { 0 };
// ������ϢHOOK��ַ
DWORD ReceiveMessageHookAddress = GetWeChatWinBase() + ReceiveMessageHookOffset;
// ������ϢHOOK��CALL��ַ
DWORD ReceiveMessageNextCall = GetWeChatWinBase() + ReceiveMessageNextCallOffset;
// ����HOOK����ת��ַ
DWORD ReceiveMessageJmpBackAddress = ReceiveMessageHookAddress + 0x5;
// ������ϢHOOK��ַ
DWORD SendMessageHookAddress = GetWeChatWinBase() + SendMessageHookOffset;
// ������ϢHOOK��CALL��ַ
DWORD SendMessageNextCall = GetWeChatWinBase() + SendMessageNextCallOffset;
// ����HOOK����ת��ַ
DWORD SendMessageJmpBackAddress = SendMessageHookAddress + 0x5;

/*
* ��Ϣ��������������Ϣ��������װ�ṹ����������
* messageAddr��������Ϣ�Ļ�������ַ
* return��void
*/
VOID ReceiveMessage(DWORD messageAddr) {
	// �˴����������Ƿ��͵Ļ��ǽ��յ���Ϣ
	BOOL isSendMessage = *(BOOL*)(messageAddr + 0x3C);

	messageStruct message = { 0 };
	message.isSendMessage = isSendMessage;
	message.time = GetTimeW();
	message.l_time = wcslen(message.time);
	message.messagetype = *(DWORD*)(messageAddr + 0x38);
	
	DWORD length = *(DWORD*)(messageAddr + 0x48 + 0x4);
	message.sender = new wchar_t[length + 1];
	ZeroMemory(message.sender, (length + 1) * 2);
	memcpy(message.sender,(wchar_t*)(*(DWORD*)(messageAddr + 0x48)),length * 2);
	message.l_sender = length;
	
	length = *(DWORD*)(messageAddr + 0x170 + 0x4);
	if (length == 0) {
		message.wxid = new wchar_t[message.l_sender + 1];
		ZeroMemory(message.wxid, (message.l_sender + 1) * 2);
		memcpy(message.wxid, (wchar_t*)(*(DWORD*)(messageAddr + 0x48)), message.l_sender * 2);
		message.l_wxid = message.l_sender;
	}
	else {
		message.wxid = new wchar_t[length + 1];
		ZeroMemory(message.wxid, (length + 1) * 2);
		memcpy(message.wxid, (wchar_t*)(*(DWORD*)(messageAddr + 0x170)), length * 2);
		message.l_wxid = length;
	}
	
	length = *(DWORD*)(messageAddr + 0x70 + 0x4);
	message.message = new wchar_t[length + 1];
	ZeroMemory(message.message, (length + 1) * 2);
	memcpy(message.message, (wchar_t*)(*(DWORD*)(messageAddr + 0x70)), length * 2);
	message.l_message = length;

	length = *(DWORD*)(messageAddr + 0x1AC + 0x4);
	message.filepath = new wchar_t[length + 1];
	ZeroMemory(message.filepath, (length + 1) * 2);
	memcpy(message.filepath, (wchar_t*)(*(DWORD*)(messageAddr + 0x1AC)), length * 2);
	message.l_filepath = length;
#ifdef _DEBUG
	wcout << message.time << endl;
#endif

	messageVector.push_back(message);
}

/*
* ���ⲿ���õĻ�ȡ��Ϣ�ӿڣ����ȷ��ؽ�����Ϣ
* return��DWORD��messageVector��һ����Ա��ַ
*/
DWORD GetHeadMessage() {
	if (messageVector.size() == 0)
		return 0;
	return (DWORD)&messageVector[0].messagetype;
}

/*
* ���ⲿ���õ�ɾ����Ϣ�ӿڣ�����ɾ��messageVector��һ����Ա��ÿ��һ����Ҫִ��һ��
* return��void
*/
VOID PopHeadMessage() {
	if (messageVector.size() == 0)
		return;
	delete[] messageVector[0].message;
	messageVector[0].message = NULL;
	delete[] messageVector[0].sender;
	messageVector[0].sender = NULL;
	delete[] messageVector[0].wxid;
	messageVector[0].wxid = NULL;
	delete[] messageVector[0].filepath;
	messageVector[0].filepath = NULL;
	delete[] messageVector[0].time;
	messageVector[0].time = NULL;
	vector<messageStruct>::iterator k = messageVector.begin();
	messageVector.erase(k);
}

/*
* HOOK�ľ���ʵ�֣����յ���Ϣ����ô�����
*/
_declspec(naked) void dealReceiveMessage() {
	__asm {
		pushad;
		pushfd;
		mov eax, [edi];
		push eax;
		call ReceiveMessage;
		add esp, 0x4;
		popfd;
		popad;
		call ReceiveMessageNextCall;
		jmp ReceiveMessageJmpBackAddress;
	}
}

/*
* HOOK�ľ���ʵ�֣�������Ϣ����ô�����
*/
_declspec(naked) void dealSendMessage() {
	__asm {
		pushad;
		pushfd;
		push edi;
		call ReceiveMessage;
		add esp, 0x4;
		popfd;
		popad;
		call SendMessageNextCall;
		jmp SendMessageJmpBackAddress;
	}
}

/*
* ��ʼ������ϢHOOK
* return��void
*/
VOID HookReceiveMessage() {
	if (ReceiveMessageHooked)
		return;
	HookAnyAddress(ReceiveMessageHookAddress,(LPVOID)dealReceiveMessage,OldReceiveMessageAsmCode);
	HookAnyAddress(SendMessageHookAddress, (LPVOID)dealSendMessage, OldSendMessageAsmCode);
	ReceiveMessageHooked = TRUE;
}

/*
* ֹͣ������ϢHOOK
* return��void
*/
VOID UnHookReceiveMessage() {
	if (!ReceiveMessageHooked)
		return;
	UnHookAnyAddress(ReceiveMessageHookAddress,OldReceiveMessageAsmCode);
	UnHookAnyAddress(SendMessageHookAddress, OldSendMessageAsmCode);
	ReceiveMessageHooked = FALSE;
}