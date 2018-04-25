#include "HIDXBox.h"
#include <math.h>
#define T 10 //ms para actualizar
#define TARGET_XBOX360
#ifdef TARGET_XBOX360
HIDXBox Control(T);
#elif defined(TARGET_PS3)
HIDPs Control(T);
#elif defined(TARGET_WII)
HIDWii Control(T);
#endif

void GeneraEfectos(HIDXBox* Control)
{
	POINT pt, pt0;
	GetCursorPos(&pt);
	HWND hWnd = GetActiveWindow();

	if ((Control->gLJX() != 0) | (Control->gLJY() != 0))
	{
		pt.x += 10 * Control->gLJXf();
		pt.y += 10 * Control->gLJYf();
		SetCursorPos(pt.x, pt.y);
	}

	RECT Rect = { 0, 0, 1024, 600 };
	HWND Desk = GetDesktopWindow();
	GetWindowRect(Desk, &Rect);

	if ((Control->gRJX() != 0) | (Control->gRJY() != 0))
	{
		pt.x = Rect.right / 2 + Rect.right / 2 * Control->gRJXf();
		pt.y = Rect.bottom / 2 - Rect.bottom / 2 * Control->gRJYf();
		SetCursorPos(pt.x, pt.y);
	}

	//eventos de raton
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
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, -(DWORD)(40 * Control->gRT()), NULL);


	//eventos de teclado
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

}
VOID CALLBACK TimerCallBack()
{
	Control.Actualiza(); //Actualiza nuestro HID
	GeneraEfectos(&Control);//Genera los efectos en la aplicación en función de los gestos del control
}