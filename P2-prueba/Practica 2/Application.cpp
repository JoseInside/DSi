#include "HIDXBox.h"
#include "HIDWii.h"
#include <math.h>

#define T 10 //ms para actualizar

void CALLBACK TimerCallBack();

#define TARGET_WII
//#define TARGET_XBOX360
#define MAX_CONTROLLERS 4



#ifdef TARGET_XBOX360
HIDXBox Control(T);
#elif defined(TARGET_PS3)
HIDPs Control(T);
#elif defined(TARGET_WII)
HIDWii Control(T);
#endif

void GeneraEfectos(BaseHID* Control)
{
	POINT pt, pt0;
	GetCursorPos(&pt);
	//HWND hWnd = GetActiveWindow();

	if ((fabs(Control->gLJX()) > 0.05) | (fabs(Control->gLJY()) > 0.05))
	{
		pt.x += 20 * Control->gLJX();
		pt.y -= 20 * Control->gLJY();
		SetCursorPos(pt.x, pt.y);
	}



	if ((fabs(Control->gRJXf()) > 0.05) | (fabs(Control->gRJYf()) > 0.05))
	{
		pt.x += 20 * Control->gRJXf();
		pt.y -= 20 * Control->gRJYf();
		SetCursorPos(pt.x, pt.y);
	}

	// raton
	if (Control->BD(XINPUT_GAMEPAD_LEFT_SHOULDER))
		mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_LEFT_SHOULDER))
		mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, NULL);
	if (Control->BD(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, NULL);
	if (Control->gLT() != 0)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, (DWORD)(40 * Control->gLT()), NULL);
	if (Control->gRT() != 0)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, (DWORD)(-40 * Control->gRT()), NULL);



	// teclado
	if (Control->BD(XINPUT_GAMEPAD_BACK))
		PostQuitMessage(0);

	if (Control->BD(XINPUT_GAMEPAD_DPAD_UP))
		keybd_event(VK_UP, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_UP))
		keybd_event(VK_UP, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_DPAD_DOWN))
		keybd_event(VK_DOWN, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_DOWN))
		keybd_event(VK_DOWN, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_DPAD_LEFT))
		keybd_event(VK_LEFT, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_LEFT))
		keybd_event(VK_LEFT, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_DPAD_RIGHT))
		keybd_event(VK_RIGHT, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_RIGHT))
		keybd_event(VK_RIGHT, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_Y))
		keybd_event(VK_PRIOR, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_Y))
		keybd_event(VK_PRIOR, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_A))
		keybd_event(VK_NEXT, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_A))
		keybd_event(VK_NEXT, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_X))
		keybd_event(VK_HOME, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_X))
		keybd_event(VK_HOME, 0x0, KEYEVENTF_KEYUP, 0);

	if (Control->BD(XINPUT_GAMEPAD_B))
		keybd_event(VK_END, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_B))
		keybd_event(VK_END, 0x0, KEYEVENTF_KEYUP, 0);
}

VOID CALLBACK TimerCallBack()
{
	Control.Actualiza(); //Actualiza HID
	
	GeneraEfectos(&Control);//Genera los efectos en la aplicación 
}

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int)
{
	//Control.Calibra();
	UINT_PTR pTimerxbox;
	pTimerxbox = SetTimer(NULL, NULL, T, (TIMERPROC)TimerCallBack);

	MSG msg;
	
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Translate and dispatch the message
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(NULL, pTimerxbox);

	return 0;
}

