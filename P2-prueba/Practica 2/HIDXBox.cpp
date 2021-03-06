#include "HIDXBox.h"


HIDXBox::HIDXBox(float f) :
	BaseHID(f)
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

	if (wButtonsDown) 
	{
	fLeftVibration = 1;
	XBox.vibration.wLeftMotorSpeed = (ushort)(fLeftVibration = MAXUINT16);
	fLeftVibration = fLeftVibration * (1 - a);

	}
	else if(wButtonsUp){
		fLeftVibration = 0;
		XBox.vibration.wLeftMotorSpeed = 0;
		
	}
	
	XInputSetState(0, &XBox.vibration);
	
}
void HIDXBox::Mando2HID(){

	wButtons = XBox.State.Gamepad.wButtons;
	fLeftTrigger = (float)XBox.State.Gamepad.bLeftTrigger / (float)MAXBYTE;
	fRightTrigger = (float)XBox.State.Gamepad.bRightTrigger / (float)MAXBYTE;	

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

	fThumbLX = (float)XBox.State.Gamepad.sThumbLX / (float)(MAXINT16 - INPUT_DEADZONE);
	fThumbLY = (float)XBox.State.Gamepad.sThumbLY / (float)(MAXINT16 - INPUT_DEADZONE);
	fThumbRX = (float)XBox.State.Gamepad.sThumbRX / (float)(MAXINT16 - INPUT_DEADZONE);
	fThumbRY = (float)XBox.State.Gamepad.sThumbRY / (float)(MAXINT16 - INPUT_DEADZONE);
}

void HIDXBox::Calibra()
{
	if (LeeMando()) XBox.OffState = XBox.State;
}
