#include "HIDXBox.h"



HIDXBox::HIDXBox(float f) :
	HID(f)
{
}


HIDXBox::~HIDXBox()
{
}

bool LeeMando(){
	Xbox.dwResult = XInputGetState(0, &Xbox.State);

	if (Xbox.dwResult == ERROR_SUCCESS)
		return true;
	else return false;
}

void EscribeMando() {

	Xbox.vibration.wLeftMotorSpeed = (ushort)(fLeftVibration = MAXUINT16);
	Xbox.vibration.wRightMotorSpeed = (ushort)(fRightVibration = MAXUINT16);
	XInputGetState(0, &Xbox.vibration);

}
void Mando2HID(){

	wButtons = Xbox.State.Gamepad.wButtons;
	fLeftTrigger = (float)Xbox.State.Gamepad.bLeftTrigger / (float)MAXBYTE;
	fRightTrigger = (float)Xbox.State.Gamepad.bRightTrigger / (float)MAXBYTE;

	/*
	Xbox.State.Gamepad.sThumbLX = Xbox.OffState.Gamepad.sThumbLX;
	Xbox.State.Gamepad.sThumbRX = Xbox.OffState.Gamepad.sThumbRX;
	Xbox.State.Gamepad.sThumbLY = Xbox.OffState.Gamepad.sThumbLY;
	Xbox.State.Gamepad.sThumbRY = Xbox.OffState.Gamepad.sThumbRY;
	*/

	//Nueva zona muerta joys
	if (Xbox.State.Gamepad.sThumbLX > INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbLX -= INPUT_DEADZONE;
	else if (Xbox.State.Gamepad.sThumbLX < -INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else Xbox.State.Gamepad.sThumbLX = 0;

	if (Xbox.State.Gamepad.sThumbLY > INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbLY -= INPUT_DEADZONE;
	else if (Xbox.State.Gamepad.sThumbLY < -INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbLY += INPUT_DEADZONE;
	else Xbox.State.Gamepad.sThumbLY = 0;

	if (Xbox.State.Gamepad.sThumbRX > INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbRX -= INPUT_DEADZONE;
	else if (Xbox.State.Gamepad.sThumbRX < -INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else Xbox.State.Gamepad.sThumbLX = 0;

	if (Xbox.State.Gamepad.sThumbRY > INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbRY -= INPUT_DEADZONE;
	else if (Xbox.State.Gamepad.sThumbRY < -INPUT_DEADZONE)
		Xbox.State.Gamepad.sThumbRY += INPUT_DEADZONE;
	else Xbox.State.Gamepad.sThumbRY = 0;
}
