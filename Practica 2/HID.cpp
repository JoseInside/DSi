#include "HID.h"



HID::HID(float t)
{
}


HID::~HID()
{
}

bool HID::gBU(ushort bit)
{
	return false;
}

bool HID::BD(ushort Bit)
{
	return false;
}

bool HID::BU(ushort Bit)
{
	return false;
}

bool HID::GRLJ()
{
	return false;
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
