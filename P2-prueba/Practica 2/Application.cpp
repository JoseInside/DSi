#include "HIDXBox.h"
#include <math.h>

#define T 10 //ms para actualizar

void CALLBACK TimerCallBack();

#define TARGET_XBOX360
#define MAX_CONTROLLERS 4

#ifdef TARGET_XBOX360
HIDXBox Control(T);
#elif defined(TARGET_PS3)
HIDPs Control(T);
#elif defined(TARGET_WII)
HIDWii Control(T);
#endif

/*struct CONTROLLER_STATE
{
	XINPUT_STATE State;
	XINPUT_STATE OffState;

	DWORD dwResult;
	XINPUT_VIBRATION vibration;
};*/

CONTROLLER_STATE Mandos[MAX_CONTROLLERS];

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
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, (DWORD)(-40 * Control->gRT()), NULL);


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

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int)
{

	UINT_PTR pTimerxbox;
	pTimerxbox = SetTimer(NULL, NULL, T, (TIMERPROC)TimerCallBack);

	MSG msg;
	
	ZeroMemory(Mandos, sizeof(CONTROLLER_STATE) * MAX_CONTROLLERS);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Translate and dispatch the message
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(NULL, pTimerxbox);

	return 0;
}

/*void RenderFrame()
{
	HCURSOR hCurs1, hCurs2;    // cursor handles 

	POINT pt;                  // cursor location  
	RECT rc;                   // client area coordinates 
	static int repeat = 1;     // repeat key counter 


	bool bRepaint = false;

	WCHAR sz[4][1024];

	//HWND hWnd = GetActiveWindow();

	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		if (Mandos[i].dwResult = ERROR_SUCCESS)
		{

			//*********************************************************
			//	INPUT DEL LEFT STICK PARA EMULAR MOVIMIENTO DE RATÓN
			//*********************************************************
			GetCursorPos(&pt);
			
			if (Mandos[i].State.Gamepad.sThumbLX < 1000 && Mandos[i].State.Gamepad.sThumbLX > -1000 ||
				Mandos[i].State.Gamepad.sThumbLY < 1000 && Mandos[i].State.Gamepad.sThumbLY > -1000)
			{
				repeat = 1;            // Clear repeat count. 
			}

			if (Mandos[i].State.Gamepad.sThumbLX < -10000)
			{
				pt.x -= repeat;
			}

			if (Mandos[i].State.Gamepad.sThumbLX > 10000)
			{
				pt.x += repeat;
			}

			if (Mandos[i].State.Gamepad.sThumbLY > 10000)
			{
				pt.y -= repeat;
			}

			if (Mandos[i].State.Gamepad.sThumbLY < -10000)
			{
				pt.y += repeat;
			}

			repeat++;           // Increment repeat count. 
			SetCursorPos(pt.x, pt.y);


			//*********************************************************
			//						INPUT MOUSE
			//*********************************************************

			if (Mandos[i].State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, 0);

			}

			if (Mandos[i].State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, 0);

			}

			if (Mandos[i].State.Gamepad.bLeftTrigger > 0)
			{
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, ((Mandos[i].State.Gamepad.bLeftTrigger +
					1) * 256) - 1, 0);

			}
			else {
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, 0, 0);
			}

			if (Mandos[i].State.Gamepad.bRightTrigger > 0)
			{
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, -((Mandos[i].State.Gamepad.bRightTrigger +
					1) * 256) - 1, 0);

			}
			else {
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, 0, 0);
			}

			//Se ha levantado la tecla
			if (Mandos[i].wlastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB
				&& Mandos[i].State.Gamepad.wButtons != XINPUT_GAMEPAD_LEFT_THUMB){
				mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0);

			}

			//Se ha levantado la tecla
			if (Mandos[i].lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB &&
				Mandos[i].State.Gamepad.wButtons != XINPUT_GAMEPAD_RIGHT_THUMB){
				mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, 0);
			}
			
			//*********************************************************
			//					INPUT KEYBOARD
			//*********************************************************

			switch (Mandos[i].lastState.Gamepad.wButtons)
			{
				// Move the cursor to reflect which 
				// arrow keys are pressed. 

			case XINPUT_GAMEPAD_DPAD_LEFT:               // left arrow 
				keybd_event(VK_LEFT,
					0,
					0,
					0);
				break;

			case XINPUT_GAMEPAD_DPAD_RIGHT:              // right arrow 
				keybd_event(VK_RIGHT,
					0,
					0,
					0);
				break;

			case XINPUT_GAMEPAD_DPAD_UP:                 // up arrow 
				keybd_event(VK_UP,
					0,
					0,
					0);
				break;

			case XINPUT_GAMEPAD_DPAD_DOWN:               // down arrow 
				keybd_event(VK_DOWN,
					0,
					0,
					0);
				break;

			case XINPUT_GAMEPAD_BACK:
				keybd_event(VK_ESCAPE,
					0,
					0,
					0);
				break;
			case XINPUT_GAMEPAD_START:
				keybd_event(VK_RETURN,
					0,
					0,
					0);
				break;
			case XINPUT_GAMEPAD_A:
				keybd_event(VK_PRIOR,
					0,
					0,
					0);
				break;
			case XINPUT_GAMEPAD_B:
				keybd_event(VK_END,
					0,
					0,
					0);
				break;
			case XINPUT_GAMEPAD_X:
				keybd_event(VK_HOME,
					0,
					0,
					0);
				break;
			case XINPUT_GAMEPAD_Y:
				keybd_event(VK_NEXT,
					0,
					0,
					0);
				break;
			default:
				break;
			}


			/*if (!Mandos[i].bLockVibration)
			{

				// Map bLeftTrigger's 0-255 to wLeftMotorSpeed's 0-65535
				if (Mandos[i].State.Gamepad.bLeftTrigger > 0)
					Mandos[i].vibration.wLeftMotorSpeed = ((Mandos[i].State.Gamepad.bLeftTrigger +
					1) * 256) - 1;
				else
					Mandos[i].vibration.wLeftMotorSpeed = 0;

				// Map bRightTrigger's 0-255 to wRightMotorSpeed's 0-65535
				if (Mandos[i].State.Gamepad.bRightTrigger > 0)
					Mandos[i].vibration.wRightMotorSpeed = ((Mandos[i].State.Gamepad.bRightTrigger +
					1) * 256) - 1;
				else
					Mandos[i].vibration.wRightMotorSpeed = 0;
			}

			if ((Mandos[i].State.Gamepad.wButtons) &&
				(Mandos[i].lastState.Gamepad.wButtons == 0))
			{
				if (!(!Mandos[i].bLockVibration && Mandos[i].vibration.wRightMotorSpeed == 0 &&
					Mandos[i].vibration.wLeftMotorSpeed == 0))
					Mandos[i].bLockVibration = !Mandos[i].bLockVibration;
			}

			XInputSetState(i, &Mandos[i].vibration);


		}
		else if (Mandos[i].dwResult == ERROR_DEVICE_NOT_CONNECTED)
		{
			StringCchPrintf(sz[i], 1024,
				L"Controller %d: Not connected", i);
		}
		else
		{
			StringCchPrintf(sz[i], 1024,
				L"Controller %d: Generic error", i);
		}

		if (wcscmp(sz[i], g_szMessage[i]) != 0)
		{
			StringCchCopy(g_szMessage[i], 1024, sz[i]);
			bRepaint = true;
		}*/

	//}

//}
