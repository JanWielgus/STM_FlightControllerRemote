#include "LCDhandler.h"
#include "Storage.h"
#include "config.h"

using Storage::lcd;




LcdHandler::LcdHandler()
{
	// do nothing
}


void LcdHandler::initLCD()
{
	lcd.init(); // Wire.begin() is here
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("FC Pilot");
	lcd.setCursor(0, 1);
	lcd.print("v 2.0");
	delay(400);
	lcd.clear();

	showStaticParts();
}


void LcdHandler::showStaticParts()
{
	lcd.clear();

	lcd.setCursor(0, 1);
	lcd.print("S:");
}


void LcdHandler::updateNonStaticParts()
{
	updateFastParts();
	updateSlowParts();
}


void LcdHandler::updateFastParts()
{
	// Throttle
	intToSizedString(fastShowLine, Storage::thrStick.getValue(), 4);
	print(fastShowLine, 0, 0);

	// Other sticks
	lcd.setCursor(4, 0);
	lcd.print(stickValueToSymbolHorizontal(Storage::rotStick.getValue()));
	lcd.print(stickValueToSymbolVertical(Storage::TB_Stick.getValue()));
	lcd.print(stickValueToSymbolHorizontal(Storage::LR_Stick.getValue()));
	
	// Altitude
	//print(ReceiveData::altitude, 8, 0); // same place as tilt
}


void LcdHandler::updateSlowParts()
{
	// print the state
	lcd.setCursor(2, 1);
	switch (Storage::armState)
	{
	case disarmed:
		lcd.print("dsmd");
		break;

	case armed:
		lcd.print("arm ");
		break;
	}


	// Other slow parts

	// Drone angle
	print(intToSizedString(ReceiveData::tilt_TB, 3), 8, 0);

	// P PID value from the android
	print(SendData::tunedPID_values.P, 12, 0);

	print(Storage::wifiCommBase.isConnected(), 14, 1);

	// Right switch
	bool swState = digitalRead(config::pin.rightSwitch);
	print(swState, 15, 1);

	// temp
	//print(intToSizedString(Storage::comm.getConnectionStability(), 3), 8, 1);
	//print(intToSizedString(ReceiveData::receivingConnectionStability, 3), 8, 1);
	print(intToSizedString(ReceiveData::altitude, 5), 8, 1);
}

/*
void LcdHandler::print(const String& str, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print(str);
}


void LcdHandler::print(char symbol, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print
}*/


template <class T>
void LcdHandler::print(T toPrint, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print(toPrint);
}


char LcdHandler::stickValueToSymbolHorizontal(int16_t value)
{
	if (value > 0)
		return '>';
	if (value < 0)
		return '<';
	return '|';
}


char LcdHandler::stickValueToSymbolVertical(int16_t value)
{
	if (value > 0)
		return '^';
	if (value < 0)
		return 'v';
	return '-';
}


// Int to right aligned string
void LcdHandler::intToSizedString(String& outString, int16_t value, uint8_t length)
{
	outString = String(value);
	while ((outString).length() < length)
		(outString) = " " + (outString);
}

String LcdHandler::intToSizedString(int16_t value, uint8_t length)
{
	String result(value);
	while ((result).length() < length)
		(result) = " " + (result);
	return result;
}
