#include "HID.h"



HID::HID(float t)
{
	T = t / 1000; //Periodo de muestreo
	a = T / (0.1 + T); //Cte. de tiempo para filtros (depende de T)
}

HID::~HID()
{
}

void HID::Actualiza()
{
	wLastButtons = wButtons; //Copia estado de botones
	bConected = LeeMando(); //Leo Mando
	if (bConected == true)
	{
		Mando2HID(); //Vuelco de Mando a HID normalizando
		 //Actualizo Gestos de entrada genéricos (entradas)
		
			//Genero Gesto de feedback (salida)
			
			EscribeMando(); //Escribo en Mando el feedback
	}
}
