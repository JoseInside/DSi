#pragma once
#include "UsbHID.h"
#include "HID.h"

const int VENDER_ID = 0x057E;
const int PRODUCT_ID = 0x0306;


class HIDWii : public BaseHID
{
private:
	UsbHID *wiimote;
	bool LeeMando();
	void EscribeMando();
	void Mando2HID();
	void Calibra();
	bool SetLEDS(bool led1, bool led2, bool led3, bool led4);

public:
	HIDWii(float t) :BaseHID(t) {
		
		wiimote = new UsbHID();
		wiimote->open(VENDER_ID, PRODUCT_ID);

		bool result = SetLEDS(false, true, false, false);

		unsigned char *outBuf = wiimote->getOutputByteBuffer();

		wiimote->clearOutputByteBuffer();
		outBuf[0] = 0x12;
		outBuf[1] = 0x00;
		outBuf[2] = 0x33;
		wiimote->write(outBuf, 3);

		

	};
	~HIDWii(){};
};

