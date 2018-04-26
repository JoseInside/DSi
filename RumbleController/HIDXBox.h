#ifndef _H_HIDXBOX_H_
#define _H_HIDXBOX_H_

#include <Windows.h>
#include <XInput.h>
#include "HID.h"
#define INPUT_DEADZONE (0.05f * FLOAT(0x7FFF))


struct CONTROLLER_STATE
{
	XINPUT_STATE State;
	XINPUT_STATE OffState;

	DWORD dwResult;
	XINPUT_VIBRATION vibration;
};

class HIDXBox : 
	public HID
{
private:
	CONTROLLER_STATE XBox;
public:
	HIDXBox(float t);
	~HIDXBox();

	bool LeeMando();
	void EscribeMando();
	void Mando2HID();
};
#endif //!_H_HIDXBOX_H_
