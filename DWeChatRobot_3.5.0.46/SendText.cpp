#include "pch.h"

/*
WeChatWin.dll: 0x78670000 

787D42EE    8D46 38         lea     eax, dword ptr [esi+38]          ; ȡat�ṹ��
787D42F1    6A 01           push    1                                ; 0x1
787D42F3    50              push    eax                              ; Ⱥ��Ϣat���ѣ���at��ϢΪ0
787D42F4    57              push    edi                              ; ��Ϣ����,[edi]
787D42F5    8D95 7CFFFFFF   lea     edx, dword ptr [ebp-84]          ; ������,[edx]
787D42FB    8D8D 58FCFFFF   lea     ecx, dword ptr [ebp-3A8]         ; ����������˵���౾��
787D4301    E8 7A793300     call    78B0BC80                         ; ������ϢCALL
787D4306    83C4 0C         add     esp, 0C                          ; ƽ���ջ
*/

#define SendTextCallOffset 0x49BC80;

struct SendTextStruct
{
    DWORD wxid;
    DWORD wxmsg;
};

void SendTextRemote(LPVOID lpParameter) {
    SendTextStruct* rp = (SendTextStruct*)lpParameter;
    wchar_t* wsWxId = (WCHAR*)rp->wxid;
    wchar_t* wsTextMsg = (WCHAR*)rp->wxmsg;
    SendText(wsWxId, wsTextMsg);
}

void __stdcall SendText(wchar_t* wsWxId, wchar_t* wsTextMsg) {
    WxBaseStruct wxWxid(wsWxId);
    WxBaseStruct wxTextMsg(wsTextMsg);
    wchar_t** pWxmsg = &wxTextMsg.buffer;
    char buffer[0x3A8] = { 0 };

    WxString wxNull = { 0 };
    DWORD dllBaseAddress = GetWeChatWinBase();
    DWORD callAddress = dllBaseAddress + SendTextCallOffset;

    __asm {
        lea eax, wxNull;
        push 0x1;
        push eax;
        mov edi, pWxmsg;
        push edi;
        lea edx, wxWxid;
        lea ecx, buffer;
        call callAddress;
        add esp, 0xC;
    }
}