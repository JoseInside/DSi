#include "Wiimote.h"
#include "HID.h"


#pragma once
class HIDWii : public BaseHID
{
private:
	Wiimote *wiimote;
	bool LeeMando();
	void EscribeMando();
	void Mando2HID();
	void Calibra();
public:
	HIDWii(float t):BaseHID(t) { wiimote = new Wiimote(); wiimote->open(); };
	~HIDWii(){};
};

