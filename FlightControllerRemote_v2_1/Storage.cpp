#include "Storage.h"
#include "config.h"


namespace Storage
{
	// Objects
	FC_ObjectTasker tasker(config::MaxAmtOfTaskerTasks);
	FC_TaskPlanner taskPlanner(config::MaxAmtOfTaskPlannerTasks);
	FC_SerialCommBase serialCommBase(&Serial, config::MaxCommPacketBytes); // can be replaced with other class that implements IPacketTransceiver interface
	FC_ESP8266_WiFiComm wifiCommBase(config::wifiCommConfig.SSID, config::wifiCommConfig.PASS,
		config::wifiCommConfig.port, config::MaxCommPacketBytes);
	FC_CommunicationHandler comm(&wifiCommBase); // <--- Set there communication way (something that implement IPacketTransceiver)
	FC_ExternalADC extADC(&taskPlanner);
	LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
	LcdHandler display;
	GestureRecognizer gestureRecognizer;


	// control sticks
	FC_ControlStick thrStick(new ThrottleStickAnalogSource);
	FC_ControlStick rotStick(new RotateStickAnalogSource);
	FC_ControlStick TB_Stick(new TB_StickAnalogSource);
	FC_ControlStick LR_Stick(new LR_StickAnalogSource);


	// Other types
	armStateType armState = disarmed;
}

