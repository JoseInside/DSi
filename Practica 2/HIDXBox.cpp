#include "HIDXBox.h"



HIDXBox::HIDXBox(float f) :
	HID(f)
{
}


HIDXBox::~HIDXBox()
{
}

bool HIDXBox::LeeMando(){
	XBox.dwResult = XInputGetState(0, &XBox.State);

	if (XBox.dwResult == ERROR_SUCCESS)
		return true;
	else return false;
}

void HIDXBox::EscribeMando() {

	XBox.vibration.wLeftMotorSpeed = (ushort)(fLeftVibration = MAXUINT16);
	XBox.vibration.wRightMotorSpeed = (ushort)(fRightVibration = MAXUINT16);
	XInputSetState(0, &XBox.vibration);

}
void HIDXBox::Mando2HID(){

	wButtons = XBox.State.Gamepad.wButtons;
	fLeftTrigger = (float)XBox.State.Gamepad.bLeftTrigger / (float)MAXBYTE;
	fRightTrigger = (float)XBox.State.Gamepad.bRightTrigger / (float)MAXBYTE;

	/*
	XBox.State.Gamepad.sThumbLX = XBox.OffState.Gamepad.sThumbLX;
	XBox.State.Gamepad.sThumbRX = XBox.OffState.Gamepad.sThumbRX;
	XBox.State.Gamepad.sThumbLY = XBox.OffState.Gamepad.sThumbLY;
	XBox.State.Gamepad.sThumbRY = XBox.OffState.Gamepad.sThumbRY;
	*/

	//Nueva zona muerta joys
	if (XBox.State.Gamepad.sThumbLX > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLX < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else XBox.State.Gamepad.sThumbLX = 0;

	if (XBox.State.Gamepad.sThumbLY > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLY < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLY += INPUT_DEADZONE;
	else XBox.State.Gamepad.sThumbLY = 0;

	if (XBox.State.Gamepad.sThumbRX > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRX < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else XBox.State.Gamepad.sThumbLX = 0;

	if (XBox.State.Gamepad.sThumbRY > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRY < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRY += INPUT_DEADZONE;
	else XBox.State.Gamepad.sThumbRY = 0;
}
