#include "pch.h"
#include <typeinfo>
#include <string>
#include <vector>

// ��ȡ������ϢCALL1ƫ��
#define GetUserInfoCall1Offset 0x5946D570 - 0x593B0000
// ��ȡ������ϢCALL2ƫ��
#define GetUserInfoCall2Offset 0x59B20980 - 0x593B0000
// ��ȡ������ϢCALL3ƫ��
#define GetUserInfoCall3Offset 0x59816270 - 0x593B0000
// ���������Ϣ�������
#define DeleteUserInfoCacheCall1Offset 0x59A752B0 - 0x593B0000
// ���������Ϣ����CALL2
#define DeleteUserInfoCacheCall2Offset 0x5946E680 - 0x593B0000

/*
* �ⲿ����ʱ�ķ�������
* message��wUserInfo.c_str()
* length��wUserInfo�ַ�������
*/
struct GetUserInfoStruct {
	DWORD message;
	DWORD length;
};

// ���������Ϣ���ַ���
wstring wUserInfo = L"";
// �ⲿ����ʱ�ľ��巵�ض���
GetUserInfoStruct ret = { 0 };

/*
* ���ݻ���������ƴ�Ӻ�����Ϣ
* address����������ַ
* return��void
*/
VOID WxUserInfo(DWORD address) {
	vector<DWORD> InfoType{
		address + 0x10,
		address + 0x24,
		address + 0x38,
		address + 0x58,
		address + 0x6C,
		address + 0xFC,
		address + 0x110,
		address + 0x19C,
		address + 0x1B0,
		address + 0x1C4,
		address + 0x1D8,
		address + 0x27C
	};
	vector<wchar_t*> InfoTypeName{
		(WCHAR*)L"\"wxId\"",
		(WCHAR*)L"\"wxNumber\"",
		(WCHAR*)L"\"wxV3\"",
		(WCHAR*)L"\"wxRemark\"",
		(WCHAR*)L"\"wxNickName\"",
		(WCHAR*)L"\"wxBigAvatar\"",
		(WCHAR*)L"\"wxSmallAvatar\"",
		(WCHAR*)L"\"wxSignature\"",
		(WCHAR*)L"\"wxNation\"",
		(WCHAR*)L"\"wxProvince\"",
		(WCHAR*)L"\"wxCity\"",
		(WCHAR*)L"\"wxBackground\"",
	};
	wUserInfo += L"{";
	for (unsigned int i = 0; i < InfoType.size(); i++) {
		wchar_t* wstemp = ((*((DWORD*)InfoType[i])) != 0) ? (WCHAR*)(*((LPVOID*)InfoType[i])) : (WCHAR*)L"null";
		wstring wsrtemp = wreplace(wstemp,L'\"',L"\\\"");
		wUserInfo = wUserInfo + InfoTypeName[i] + L":\"" + wsrtemp + L"\"";
		if (i != InfoType.size() - 1) {
			wUserInfo += L",";
		}
	}
	wUserInfo += L"}";
#ifdef _DEBUG
	wcout.imbue(locale("chs"));
	wcout << wUserInfo.c_str() << endl;
#endif
}

/*
* ���ⲿ���õĻ�ȡ������Ϣ�ӿ�
* lparamter���������wxid�ĵ�ַ
* return��DWORD��`ret`���׵�ַ
*/
DWORD GetWxUserInfoRemote(LPVOID lparamter) {
	wchar_t* userwxid = (wchar_t*)lparamter;
	
	if (!GetUserInfoByWxId(userwxid)) {
		return 0;
	}
	ret.message = (DWORD)wUserInfo.c_str();
	ret.length = (DWORD)wUserInfo.length();
	return (DWORD)&ret;
}

/*
* ���ⲿ���õ���պ�����Ϣ����Ľӿ�
* return��void
*/
VOID DeleteUserInfoCacheRemote() {
	if (ret.length) {
		ZeroMemory((wchar_t*)ret.message, ret.length * 2 + 2);
		ret.length = 0;
		wUserInfo = L"";
	}
}

/*
* ����wxid��ȡ������Ϣ�ľ���ʵ��
* wxid������wxid
* return��BOOL���ɹ�����`1`��ʧ�ܷ���`0`
*/
BOOL __stdcall GetUserInfoByWxId(wchar_t* wxid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD WxGetUserInfoCall1 = WeChatWinBase + GetUserInfoCall1Offset;
	DWORD WxGetUserInfoCall2 = WeChatWinBase + GetUserInfoCall2Offset;
	DWORD WxGetUserInfoCall3 = WeChatWinBase + GetUserInfoCall3Offset;
	DWORD DeleteUserInfoCacheCall1 = WeChatWinBase + DeleteUserInfoCacheCall1Offset;
	DWORD DeleteUserInfoCacheCall2 = WeChatWinBase + DeleteUserInfoCacheCall2Offset;
	char buffer[0x3FC] = { 0 };
	WxBaseStruct pWxid(wxid);
	DWORD address = 0;
	DWORD isSuccess = 0;
	__asm
	{
		pushad;
		call WxGetUserInfoCall1;
		lea ebx, buffer;
		push ebx;
		sub esp, 0x14;
		mov esi, eax;
		lea eax, pWxid;
		mov ecx, esp;
		push eax;
		call WxGetUserInfoCall2;
		mov ecx, esi;
		call WxGetUserInfoCall3;
		mov isSuccess, eax;
		mov address, ebx;
		popad;
	}
	if(isSuccess)
		WxUserInfo(address);
	char deletebuffer[0x410] = { 0 };
	__asm {
		pushad;
		lea ecx, deletebuffer;
		call DeleteUserInfoCacheCall1;
		push eax;
		lea ebx,buffer;
		mov ecx, ebx;
		call DeleteUserInfoCacheCall2;
		popad;
	}
	return isSuccess;
}

/*
* ����wxid��ȡ��ϵ���ǳƣ���Ҫ���ڷ��Ͱ�����Ϣ�ӿ�
* wxid����ϵ��wxid
* return��wchar_t*����ȡ����wxid
*/
wchar_t* __stdcall GetUserNickNameByWxId(wchar_t* wxid) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD WxGetUserInfoCall1 = WeChatWinBase + GetUserInfoCall1Offset;
	DWORD WxGetUserInfoCall2 = WeChatWinBase + GetUserInfoCall2Offset;
	DWORD WxGetUserInfoCall3 = WeChatWinBase + GetUserInfoCall3Offset;
	DWORD DeleteUserInfoCacheCall1 = WeChatWinBase + DeleteUserInfoCacheCall1Offset;
	DWORD DeleteUserInfoCacheCall2 = WeChatWinBase + DeleteUserInfoCacheCall2Offset;
	char buffer[0x3FC] = { 0 };
	WxBaseStruct pWxid(wxid);
	DWORD address = 0;
	DWORD isSuccess = 0;
	__asm
	{
		pushad;
		call WxGetUserInfoCall1;
		lea ebx, buffer;
		push ebx;
		sub esp, 0x14;
		mov esi, eax;
		lea eax, pWxid;
		mov ecx, esp;
		push eax;
		call WxGetUserInfoCall2;
		mov ecx, esi;
		call WxGetUserInfoCall3;
		mov isSuccess, eax;
		mov address, ebx;
		popad;
	}
	wchar_t* NickName = NULL;
	if (isSuccess) {
		DWORD length = *(DWORD*)(address + 0x6C + 0x4);
		NickName = new wchar_t[length + 1];
		ZeroMemory(NickName, (length + 1) * 2);
		memcpy(NickName, (wchar_t*)(*(DWORD*)(address + 0x6C)), length * 2);
	}
	char deletebuffer[0x410] = { 0 };
	__asm {
		pushad;
		lea ecx, deletebuffer;
		call DeleteUserInfoCacheCall1;
		push eax;
		lea ebx, buffer;
		mov ecx, ebx;
		call DeleteUserInfoCacheCall2;
		popad;
	}
	return NickName;
}