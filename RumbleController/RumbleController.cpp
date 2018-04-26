//-----------------------------------------------------------------------------
// File: RumbleController.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <commdlg.h>
#include <XInput.h>
#include <basetsd.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "resource.h"

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
HRESULT UpdateControllerState();
void RenderFrame();

bool flag = false;
//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define MAX_CONTROLLERS     4


//-----------------------------------------------------------------------------
// Struct to hold xinput state
//-----------------------------------------------------------------------------
struct CONTROLER_STATE
{
    XINPUT_STATE lastState;
    XINPUT_STATE state;
    DWORD dwResult;
    bool bLockVibration;
    XINPUT_VIBRATION vibration;
};

CONTROLER_STATE g_Controllers[MAX_CONTROLLERS];
WCHAR g_szMessage[4][1024] = {0};
HWND g_hWnd;


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application.  Since we use a simple dialog for 
//       user interaction we don't need to pump messages.
//-----------------------------------------------------------------------------
int APIENTRY wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, int )
{
    // Register the window class
    HBRUSH hBrush = CreateSolidBrush( 0xFF0000 );
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), 0, MsgProc, 0L, 0L, hInst, NULL,
        LoadCursor( NULL, IDC_ARROW ), hBrush,
        NULL, L"XInputSample", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    g_hWnd = CreateWindow( L"XInputSample", L"XInput Sample: RumbleController",
                           WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                           CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
                           NULL, NULL, hInst, NULL );

    // Init state
    ZeroMemory( g_Controllers, sizeof( CONTROLER_STATE ) * MAX_CONTROLLERS );

    // Enter the message loop
    bool bGotMsg;
    MSG msg;
    msg.message = WM_NULL;

    while( WM_QUIT != msg.message )
    {
        // Use PeekMessage() so we can use idle time to render the scene and call pEngine->DoWork()
        bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

        if( bGotMsg )
        {
            // Translate and dispatch the message
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            UpdateControllerState();
            RenderFrame();
        }
    }

    // Clean up 
    UnregisterClass( L"XInputSample", NULL );

    return 0;
}


//-----------------------------------------------------------------------------
HRESULT UpdateControllerState()
{
    for( DWORD i = 0; i < MAX_CONTROLLERS; i++ )
    {
        g_Controllers[i].lastState = g_Controllers[i].state;
        g_Controllers[i].dwResult = XInputGetState( i, &g_Controllers[i].state );
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
void RenderFrame()
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
		if (g_Controllers[i].dwResult == ERROR_SUCCESS)
		{

			//*********************************************************
			//	INPUT DEL LEFT STICK PARA EMULAR MOVIMIENTO DE RATÓN
			//*********************************************************
			GetCursorPos(&pt);

			if (g_Controllers[i].state.Gamepad.sThumbLX < 1000 && g_Controllers[i].state.Gamepad.sThumbLX > -1000 ||
				g_Controllers[i].state.Gamepad.sThumbLY < 1000 && g_Controllers[i].state.Gamepad.sThumbLY > -1000)
			{
				repeat = 1;            // Clear repeat count. 
			}

			if (g_Controllers[i].state.Gamepad.sThumbLX < -10000)
			{
				pt.x -= repeat;
			}

			if (g_Controllers[i].state.Gamepad.sThumbLX > 10000)
			{
				pt.x += repeat;
			}

			if (g_Controllers[i].state.Gamepad.sThumbLY > 10000)
			{
				pt.y -= repeat;
			}

			if (g_Controllers[i].state.Gamepad.sThumbLY < -10000)
			{
				pt.y += repeat;
			}

			repeat++;           // Increment repeat count. 
			SetCursorPos(pt.x, pt.y);


			//*********************************************************
			//						INPUT MOUSE
			//*********************************************************

			if (g_Controllers[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, 0);

			}

			if (g_Controllers[i].state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, 0);

			}

			if (g_Controllers[i].state.Gamepad.bLeftTrigger > 0)
			{
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, ((g_Controllers[i].state.Gamepad.bLeftTrigger +
					1) * 256) - 1, 0);

			}
			else {
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, 0, 0);
			}

			if (g_Controllers[i].state.Gamepad.bRightTrigger > 0)
			{
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, -((g_Controllers[i].state.Gamepad.bRightTrigger +
					1) * 256) - 1, 0);

			}
			else {
				mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, 0, 0);
			}

			//Se ha levantado la tecla
			if (g_Controllers[i].lastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB
				&& g_Controllers[i].state.Gamepad.wButtons != XINPUT_GAMEPAD_LEFT_THUMB){
				mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0);

			}

			//Se ha levantado la tecla
			if (g_Controllers[i].lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB &&
				g_Controllers[i].state.Gamepad.wButtons != XINPUT_GAMEPAD_RIGHT_THUMB){
				mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, 0);
			}

			//*********************************************************
			//					INPUT KEYBOARD
			//*********************************************************

			switch (g_Controllers[i].lastState.Gamepad.wButtons)
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

		
			if (!g_Controllers[i].bLockVibration)
			{

				// Map bLeftTrigger's 0-255 to wLeftMotorSpeed's 0-65535
				if (g_Controllers[i].state.Gamepad.bLeftTrigger > 0)
					g_Controllers[i].vibration.wLeftMotorSpeed = ((g_Controllers[i].state.Gamepad.bLeftTrigger +
					1) * 256) - 1;
				else
					g_Controllers[i].vibration.wLeftMotorSpeed = 0;

				// Map bRightTrigger's 0-255 to wRightMotorSpeed's 0-65535
				if (g_Controllers[i].state.Gamepad.bRightTrigger > 0)
					g_Controllers[i].vibration.wRightMotorSpeed = ((g_Controllers[i].state.Gamepad.bRightTrigger +
					1) * 256) - 1;
				else
					g_Controllers[i].vibration.wRightMotorSpeed = 0;
			}

			if ((g_Controllers[i].state.Gamepad.wButtons) &&
				(g_Controllers[i].lastState.Gamepad.wButtons == 0))
			{
				if (!(!g_Controllers[i].bLockVibration && g_Controllers[i].vibration.wRightMotorSpeed == 0 &&
					g_Controllers[i].vibration.wLeftMotorSpeed == 0))
					g_Controllers[i].bLockVibration = !g_Controllers[i].bLockVibration;
			}

			XInputSetState(i, &g_Controllers[i].vibration);

			StringCchPrintfW(sz[i], 1024,
				L"Controller %d: Connected\n"
				L"  Left Motor Speed: %d\n"
				L"  Right Motor Speed: %d\n"
				L"  Rumble Lock: %d\n", i,
				g_Controllers[i].vibration.wLeftMotorSpeed,
				g_Controllers[i].vibration.wRightMotorSpeed,
				g_Controllers[i].bLockVibration);

		}
		else if (g_Controllers[i].dwResult == ERROR_DEVICE_NOT_CONNECTED)
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
		}

	}

    if( bRepaint )
    {
        // Repaint the window if needed 
        InvalidateRect( g_hWnd, NULL, TRUE );
        UpdateWindow( g_hWnd );
    }

    // This sample doesn't use Direct3D.  Instead, it just yields CPU time to other 
    // apps but this is not typically done when rendering
    Sleep( 10 );
}


void SetNumLock(BOOL bState)
{
	BYTE keyState[256];

	GetKeyboardState((LPBYTE)&keyState);
	if ((bState && !(keyState[VK_NUMLOCK] & 1)) ||
		(!bState && (keyState[VK_NUMLOCK] & 1)))
	{
		// Simulate a key press
		keybd_event(VK_NUMLOCK,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);

		// Simulate a key release
		keybd_event(VK_NUMLOCK,
			0x45,
			KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
			0);
	}
}

void main()
{
	SetNumLock(TRUE);
}


//-----------------------------------------------------------------------------
// Window message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

	HCURSOR hCurs1, hCurs2;    // cursor handles 

	POINT pt;                  // cursor location  
	RECT rc;                   // client area coordinates 
	static int repeat = 1;     // repeat key counter 

	

    switch( msg )
    {
        case WM_ACTIVATEAPP:
        {
            if( wParam == TRUE )
            {
                // App is now active, so re-enable XInput
                XInputEnable( true );
            }
            else
            {
                // App is now inactive, so disable XInput to prevent
                // user input from effecting application and to 
                // disable rumble. 
                //XInputEnable( false );
            }
            break;
        }

        case WM_PAINT:
        {
            // Paint some simple explanation text
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint( hWnd, &ps );
            SetBkColor( hDC, 0xFF0000 );
            SetTextColor( hDC, 0xFFFFFF );
            RECT rect;
            GetClientRect( hWnd, &rect );

            rect.top = 20;
            rect.left = 20;
            DrawText( hDC,
                      L"Use the controller's left/right trigger to adjust the speed of the left/right rumble motor.\n"
                      L"Press any controller button to lock or unlock at the current rumble speed.\n",
                      -1, &rect, 0 );

            for( DWORD i = 0; i < MAX_CONTROLLERS; i++ )
            {
                rect.top = i * 80 + 90;
                rect.left = 20;
                DrawText( hDC, g_szMessage[i], -1, &rect, 0 );
            }

            EndPaint( hWnd, &ps );
            return 0;
        }

		// 
		// Process other messages. 
		// 
		
		case WM_KEYDOWN:

			if (wParam != VK_LEFT && wParam != VK_RIGHT &&
				wParam != VK_UP && wParam != VK_DOWN)
			{
				break;
			}

			//hWnd = GetActiveWindow();

			GetCursorPos(&pt);

			// Convert screen coordinates to client coordinates. 

			ScreenToClient(hWnd, &pt);

			switch (wParam)
			{
				// Move the cursor to reflect which 
				// arrow keys are pressed. 

				case VK_LEFT:               // left arrow 
				pt.x -= repeat;
				break;

				case VK_RIGHT:              // right arrow 
				pt.x += repeat;
				break;

				case VK_UP:                 // up arrow 
				pt.y -= repeat;
				break;

				case VK_DOWN:               // down arrow 
				pt.y += repeat;
				break;

				case VK_PAD_BACK:
					keybd_event(VK_ESCAPE,
						0x1B,
						KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
						0);
					break;
				case VK_INSERT:
					keybd_event(VK_RETURN,
						0x45,
						KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
						0);
					break;

			default:
				return 0;
			}

			repeat++;           // Increment repeat count. 

			// Keep the cursor in the client area. 

			GetClientRect(hWnd, &rc);

			if (pt.x >= rc.right)
			{
				pt.x = rc.right - 1;
			}
			else
			{
				if (pt.x < rc.left)
				{
					pt.x = rc.left;
				}
			}

			if (pt.y >= rc.bottom)
				pt.y = rc.bottom - 1;
			else
				if (pt.y < rc.top)
					pt.y = rc.top;

			// Convert client coordinates to screen coordinates. 

			ClientToScreen(hWnd, &pt);
			SetCursorPos(pt.x, pt.y);
			return 0;


		case WM_KEYUP:

			repeat = 1;            // Clear repeat count. 
			return 0;

        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            break;
        }
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}



