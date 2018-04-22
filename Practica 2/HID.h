#ifndef _H_HID_H_
#define _H_HID_H_

typedef unsigned short ushort;

class HID
{

	enum EstadosRotacion
	{
		CUADRANTE1,
		CUADRANTE2,
		CUADRANTE3,
		CUADRANTE4
	};
public:
	bool bConected; //Mando Conectado
	//Gets & Sets
	bool gBU(ushort bit); //Estado del Boton codificado en bit
	float gLT() { return fLeftTrigger; }//Left Triger [0,1]
	float gRT() { return fRightTrigger; } //Right Triger [0,1]
	float gLJX() { return fThumbLX; } //LeftJoyX [-1,1]
	float gLJY() { return fThumbLY; } //LeftJoyY [-1,1]
	float gRJX() { return fThumbRX; } //RightJoyX [-1,1]
	float gRJY() { return fThumbRY; } //RightJoyY [-1,1]
	float gLJXf() { return fThumbLXf; } //LeftJoyXfiltered [-1,1]
	float gLJYf() { return fThumbLYf; } //LeftJoyYfiltered [-1,1]
	float gRJXf() { return fThumbRXf; } //RightJoyXfiltered [-1,1]
	float gRJYf() { return fThumbRYf; } //RigthJoyYfiltered [-1,1]
	void sLR(float cantidad, float tiempo) { cantidad = fLeftVibration; tiempo = tLR; } //LeftRumble [0,1]: cantidad [0,1], tiempo [0,inf]
	void sRR(float cantidad, float tiempo) { cantidad = fRightVibration; tiempo = tRR; } //RightRumble [0,1]: cantidad [0,1], tiempo [0,inf]
	//Gestos
	bool BD(ushort Bit); //Boton Down codificado en Bit
	bool BU(ushort Bit); //Boton Up codificado en Bit
	bool GRLJ(); //Gesto de Rotación del LeftJoy
	HID(float t) //Constructor que recoge el periodo de muestreo
	{
		T = t / 1000; //Periodo de muestreo
		a = T / (0.1 + T); //Cte. de tiempo para filtros (depende de T)
	};
	~HID() {};
	void Actualiza(); //Actualiza Mando2HID y HID2Mando.
protected:
	//Entradas
	ushort wButtons; //Botones (Utilizo Codificación Xbox360)
	ushort wLastButtons; //Botones anteriores (Utilizo Codificación Xbox360)
	float fLeftTrigger, fRightTrigger; //[0.0,1.0]
	float fThumbLX, fThumbLY, fThumbRX, fThumbRY; //[-1.0,1.0]
	float fThumbLXf, fThumbLYf, fThumbRXf, fThumbRYf; //[-1.0,1.0] Filtrado
	float T; //Perido de actualización
	float a; //Cte.Tiempo Filtro		const
				   //Salidas
	float fLeftVibration, fRightVibration; //[0.0,1.0] Salida
	float tLR = 0.0; //Tiempo que queda de vibración en LR
	float tRR = 0.0; //Tiempo que queda de vibración en RR
					 //Gestos
	ushort wButtonsDown; //EventosDown Botones (Codificación Xbox360?)
	ushort wButtonsUp; //EventosUp Botones (Codificación Xbox360?)
	EstadosRotacion Ro; //Estado del gesto de rotación
	float tRo = 0.0; //Tiempo que queda para el gesto de rotación



	//Funciones virtuales que se deben reimplementar para cada mando.
	virtual bool LeeMando() = 0; //Lee estado del mando
	virtual void EscribeMando() = 0; //Escribe Feeback en mando
	virtual void Mando2HID() = 0; //Vuelca el estado del mando al HID
};
#endif // !_H_HID_H_

