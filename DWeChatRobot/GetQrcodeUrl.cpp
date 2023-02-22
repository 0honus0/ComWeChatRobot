#include "pch.h"
#define GetQrcodeUrlOffset 0x23A10C8

wstring GetQrcodeUrl()
{
    string url = "http://weixin.qq.com/x/";
    DWORD addr = GetWeChatWinBase() + GetQrcodeUrlOffset;
    url += string((char *)(*(DWORD *)addr), *(int *)(addr + 0x10));
    return utf8_to_unicode(url.c_str());
}
