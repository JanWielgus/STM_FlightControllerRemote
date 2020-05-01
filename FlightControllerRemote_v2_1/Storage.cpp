#include "Storage.h"
#include "config.h"


namespace Storage
{
	// Objects
	FC_ObjectTasker tasker(config::MaxAmtOfTaskerTasks);
	FC_TaskPlanner taskPlanner(config::MaxAmtOfTaskPlannerTasks);
	FC_CommunicationHandler comm(&Serial, config::MaxCommPacketBytes);
	FC_ExternalADC extADC(&taskPlanner);
	LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
	LcdHandler display;
	GestureRecognizer gestureRecognizer;
	AndroidCommunication& androidComm = *AndroidCommunication::getInstance();


	// control sticks
	FC_ControlStick thrStick(new ThrottleStickAnalogSource);
	FC_ControlStick rotStick(new RotateStickAnalogSource);
	FC_ControlStick TB_Stick(new TB_StickAnalogSource);
	FC_ControlStick LR_Stick(new LR_StickAnalogSource);


	// Other types
	armStateType armState = disarmed;
}

