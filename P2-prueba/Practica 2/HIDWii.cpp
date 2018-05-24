#include "HIDWii.h"


bool HIDWii::LeeMando()
{
	return wiimote->read();
}

void HIDWii::EscribeMando()
{
}


#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000


void HIDWii::Mando2HID()
{
	if (wiimote->IsHomePressed()) 		wButtons |= XINPUT_GAMEPAD_BACK;
	else 	wButtons &= ~XINPUT_GAMEPAD_BACK;
	
	if (wiimote->IsAPressed())		wButtons |= XINPUT_GAMEPAD_A;
	else  	wButtons &= ~XINPUT_GAMEPAD_A;
	
	if (wiimote->IsBPressed()) wButtons |= XINPUT_GAMEPAD_B;
	else wButtons &= ~XINPUT_GAMEPAD_B;
	
	if (wiimote->IsOnePressed()) wButtons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
	else wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;

	if (wiimote->IsTwoPressed()) wButtons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
	else wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;
	
	wiimote->IsUpPressed() ? wButtons |= XINPUT_GAMEPAD_DPAD_UP : wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
	wiimote->IsDownPressed() ? wButtons |= XINPUT_GAMEPAD_DPAD_DOWN : wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
	wiimote->IsLeftPressed() ? wButtons |= XINPUT_GAMEPAD_DPAD_LEFT : wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
	wiimote->IsRightPressed() ? wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT : wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;

	
	//Normalizamos Joys
	fThumbLX = (float)(wiimote->getXMotion() - 0x80) / 25.0;  // +g=24/127 [-1.0,1.0]
	fThumbLY = -(float)(wiimote->getYMotion() - 0x80) / 25.0; //[-1.0,1.0]
		
	
}

void HIDWii::Calibra()
{
}