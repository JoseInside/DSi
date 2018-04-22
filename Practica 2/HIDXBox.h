#ifndef _H_HIDXBOX_H_
#define _H_HIDXBOX_H_
#include <Windows.h>
#include <XInput.h>
#include "HID.h"

class HIDXBox : 
	public HID
{
private:
	CONTROLLER_STATE XBox;
public:
	HIDXBox(float t);

	bool LeeMando();
	void EscribeMando();
	void Mando2HID();
};
#endif //!_H_HIDXBOX_H_
