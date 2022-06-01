#include "pch.h"

// ����ͼƬCALL1ƫ��
#define SendImageCall1Offset (0x5432D730 - 0x54270000)
// ����ͼƬCALL2ƫ��
#define SendImageCall2Offset (0x549E0980 - 0x54270000)
// ����ͼƬCALL3ƫ��
#define SendImageCall3Offset (0x54791640 - 0x54270000)
// ��ջ����CALLƫ��
#define DeleteSendImageCacheCallOffset (0x54327720 - 0x54270000)

/*
* �ⲿ����ʱ���ݵĲ����ṹ
* wxid������wxid�ĵ�ַ
* imagepath������ͼƬ����·���ĵ�ַ
*/
struct ImageParamStruct {
	DWORD wxid;
	DWORD imagepath;
};

/*
* ���ⲿ���õķ���ͼƬ��Ϣ�ӿ�
* lpParamStruct��ImageParamStruct���ͽṹ��ָ��
* return��void
*/
void SendImageRemote(LPVOID lpParamStruct) {
	ImageParamStruct* params = (ImageParamStruct*)lpParamStruct;
	SendImage((WCHAR*)params->wxid, (WCHAR*)params->imagepath);
}

/*
* ����ͼƬ��Ϣ�ľ���ʵ��
* receiver��������wxid
* ImagePath��ͼƬ����·��
* return��void
*/
void __stdcall SendImage(wchar_t* receiver, wchar_t* ImagePath) {
	DWORD WeChatWinBase = GetWeChatWinBase();
	DWORD SendImageCall1 = WeChatWinBase + SendImageCall1Offset;
	DWORD SendImageCall2 = WeChatWinBase + SendImageCall2Offset;
	DWORD SendImageCall3 = WeChatWinBase + SendImageCall3Offset;
	DWORD DeleteSendImageCacheCall = WeChatWinBase + DeleteSendImageCacheCallOffset;
	char nullbuffer[0x50] = { 0 };
	char buffer[0x3B0] = { 0 };
	WxBaseStruct pReceiver(receiver);
	WxBaseStruct pImagePath(ImagePath);
	WxString nullStruct = { 0 };
	
	__asm {
		pushad;
		call SendImageCall1;
		sub esp, 0x14;
		mov ebx, eax;
		lea eax, nullStruct;
		mov ecx, esp;
		lea edi, pImagePath;
		push eax;
		call SendImageCall2;
		mov ecx, ebx;
		lea eax, pReceiver;
		push edi;
		push eax;
		lea eax, buffer;
		push eax;
		call SendImageCall3;
		lea ecx, buffer;
		call DeleteSendImageCacheCall;
		popad;
	}
}