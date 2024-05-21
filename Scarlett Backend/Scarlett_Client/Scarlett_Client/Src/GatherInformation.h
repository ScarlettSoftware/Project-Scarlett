#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <stdio.h>
#include <intrin.h>
#include <windows.h>
#include <sddl.h>

std::string GetCurrentUserSID()
{
    HANDLE token = nullptr;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
    {
        std::cerr << "Failed to open process token. Error: " << GetLastError() << std::endl;
        return "";
    }

    DWORD tokenInfoLength = 0;
    GetTokenInformation(token, TokenUser, nullptr, 0, &tokenInfoLength);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        std::cerr << "Failed to get token information length. Error: " << GetLastError() << std::endl;
        CloseHandle(token);
        return "";
    }

    TOKEN_USER* tokenUser = (TOKEN_USER*)malloc(tokenInfoLength);
    if (!GetTokenInformation(token, TokenUser, tokenUser, tokenInfoLength, &tokenInfoLength))
    {
        std::cerr << "Failed to get token information. Error: " << GetLastError() << std::endl;
        CloseHandle(token);
        free(tokenUser);
        return "";
    }

    LPSTR sidString = nullptr;
    if (!ConvertSidToStringSidA(tokenUser->User.Sid, &sidString))
    {
        std::cerr << "Failed to convert SID to string. Error: " << GetLastError() << std::endl;
        CloseHandle(token);
        free(tokenUser);
        return "";
    }

    std::string sid(sidString);
    LocalFree(sidString);
    CloseHandle(token);
    free(tokenUser);
    return sid;
}