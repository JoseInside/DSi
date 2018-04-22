#include "HIDXBox.h"
#include <math.h>
#define T 10 //ms para actualizar
#define TARGET_XBOX360
#ifdef TARGET_XBOX360
HIDXBox Control(T);
#elif defined(TARGET_PS3)
HIDPs Control(T);
#elif defined(TARGET_WII)
HIDWii Control(T);
#endif

void GeneraEfectos(HIDXBox* Control)
{
	POINT pt, pt0;
	GetCursorPos(&pt);
	HWND hwnd = GetActiveWindow();

	if ((fabs(Control->gLJX()) > 0.05) || (fabs(Control->gLJY() > 0.05))) 
	{
		pt.x += 20 * Control->gLJX();
		pt.y -= 20 * Control->gLJY();
		SetCursorPos(pt.x, pt.y);
	}

	if ((fabs(Control->gLJXf()) > 0.05) || (fabs(Control->gLJYf() > 0.05)))
	{
		pt.x += 20 * Control->gLJXf();
		pt.y -= 20 * Control->gLJYf();
		SetCursorPos(pt.x, pt.y);
	}

}
VOID CALLBACK TimerCallBack()
{
	Control.Actualiza(); //Actualiza nuestro HID
	GeneraEfectos(&Control);//Genera los efectos en la aplicación en función de los gestos del control
}