#include <iostream>
#include <Windows.h>
#include<thread>

using namespace std;

HHOOK hookProc;


LRESULT CALLBACK waitForKeys(const int code, const WPARAM wParam, const LPARAM lParam) {
	if (wParam == WM_KEYDOWN) { //tipul eventului de keyboard (keydown , keyup, etc...)
		KBDLLHOOKSTRUCT* kbdStruct = (KBDLLHOOKSTRUCT*)lParam; //convertim lparam in pointer de KBDLLHOOKSTRUCT ca sa iau date despre keyboard event
		int virtualCode = kbdStruct->vkCode; //virtual OS code 
		int scanCode = kbdStruct->scanCode; //keyboard hardware code 

		BYTE keyState[256]; //256 key codes 
		GetKeyboardState(keyState); //preluam state-ul keyboard-ului

		char result; //stocam caracterul apasat
		ToAscii(virtualCode, scanCode, keyState, (unsigned short*)&result, 0);
		cout << result << "\n";
	}
	return CallNextHookEx(hookProc, code, wParam, lParam);
}

void StartHook() {
	cout << "key hook started \n";
	hookProc = SetWindowsHookEx(WH_KEYBOARD_LL, waitForKeys, NULL, 0);
	GetMessage(NULL, NULL, 0, 0); //activam hook chain-ul 
}

int main() {
	std::thread getDataThread(StartHook);
	getDataThread.join();
	cout << "process ended\n";
	return 0;
}