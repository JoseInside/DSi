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
		wButtonsDown = (~wLastButtons) & (wButtons);
		wButtonsUp = (wLastButtons) & (~wButtons);

		fThumbRXf = (1 - a)*fThumbRXf + a * fThumbRX;
		fThumbRYf = (1 - a)*fThumbRYf + a * fThumbRY;


		if ((fThumbLX > 0.8) | (fThumbLX < -0.8))
			fVelX = fVelX + aV * fThumbLX;
		else if ((fThumbLX > 0.01) | (fThumbLX < -0.01))
			fVelX = (fVelX + fThumbLX) / 2;
		else
			fVelX = fVelX * (1 - aV);

		if ((fThumbLY > 0.8) | (fThumbLY < -0.8))
			fVelY = fVelY + aV * fThumbLY;
		else if ((fThumbLY > 0.01) | (fThumbLY < -0.01))
			fVelY = (fVelY + fThumbLY) / 2;
		else
			fVelY = fVelY * (1 - aV);

		if (fVelX > 2.0) fVelX = 2.0;
		if (fVelX < -2.0) fVelX = -2.0;
		if (fVelY > 2.0) fVelY = 2.0;
		if (fVelY < -2.0) fVelY = -2.0;

		if ((fThumbLX > 0) &(fThumbLY > 0))
		{
			Ro == CUADRANTE1;
			tRo = 1.0;
		}
		if (tRo > 0) {
			tRo = tRo - T;
			if ((fThumbLX > 0) & (fThumbLY <= 0) &(Ro == CUADRANTE1)) Ro = CUADRANTE4;
			if ((fThumbLX <= 0) & (fThumbLY < 0) &(Ro == CUADRANTE4)) Ro = CUADRANTE3;
			if ((fThumbLX < 0) & (fThumbLY >= 0) &(Ro == CUADRANTE3)) Ro = CUADRANTE2;
		}
		else Ro = CUADRANTE1;

		if (wButtonsDown) fRightVibration = 1;
		fRightVibration = fRightVibration * (1 - a);
		if (wButtonsUp) fLeftVibration = 1;
		fLeftVibration = fLeftVibration * (1 - a);

		EscribeMando();

	}
}
