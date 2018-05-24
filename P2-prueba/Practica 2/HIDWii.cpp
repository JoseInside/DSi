#include "HIDWii.h"


bool HIDWii::LeeMando()
{
	return wiimote->read();
}

void HIDWii::EscribeMando()
{
	unsigned char *outBuf = wiimote->getOutputByteBuffer();

	if (wButtonsDown)
	{
		wiimote->clearOutputByteBuffer();
		outBuf[0] = 21;
		outBuf[1] = 1;
		wiimote->write(outBuf, 2);

	}
	else if (wButtonsUp) {
		wiimote->clearOutputByteBuffer();
		outBuf[0] = 21;
		outBuf[1] = 0;
		wiimote->write(outBuf, 2);

	}
	
	

	
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
	if (wiimote->IsButtonPressed(2, 0x80))	wButtons |= XINPUT_GAMEPAD_BACK;
	else 	wButtons &= ~XINPUT_GAMEPAD_BACK;
	
	if (wiimote->IsButtonPressed(2, 0x08))		wButtons |= XINPUT_GAMEPAD_A;
	else  	wButtons &= ~XINPUT_GAMEPAD_A;
	
	if (wiimote->IsButtonPressed(2, 0x04)) wButtons |= XINPUT_GAMEPAD_B;
	else wButtons &= ~XINPUT_GAMEPAD_B;
	
	if (wiimote->IsButtonPressed(2, 0x02)) wButtons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
	else wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;

	if (wiimote->IsButtonPressed(2, 0x01)) wButtons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
	else wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;
	
	wiimote->IsButtonPressed(1, 0x08) ? wButtons |= XINPUT_GAMEPAD_DPAD_UP : wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
	wiimote->IsButtonPressed(1, 0x04) ? wButtons |= XINPUT_GAMEPAD_DPAD_DOWN : wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
	wiimote->IsButtonPressed(1, 0x01) ? wButtons |= XINPUT_GAMEPAD_DPAD_LEFT : wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
	wiimote->IsButtonPressed(1, 0x02) ? wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT : wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;

	
	//Normalizamos Joys
	fThumbLX = (float)(wiimote->getInputByte(3) - 0x80) / 25.0;  // +g=24/127 [-1.0,1.0]
	fThumbLY = -(float)(wiimote->getInputByte(4) - 0x80) / 25.0; //[-1.0,1.0]
		
	
}

void HIDWii::Calibra()
{
}

bool HIDWii::SetLEDS(bool led1, bool led2, bool led3, bool led4) {
	
	unsigned char *outBuf = wiimote->getOutputByteBuffer();

	wiimote->clearOutputByteBuffer();
	outBuf[0] = 0x21;
	outBuf[1] = 0x00;
	if (led1)
		outBuf[1] |= 0x10;
	if (led2)
		outBuf[1] |= 0x20;
	if (led3)
		outBuf[1] |= 0x40;
	if (led4)
		outBuf[1] |= 0x80;
	if (wiimote->write(outBuf, 2) <= 0)
		return true;

	return false;
}