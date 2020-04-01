#include "Storage.h"
#include "config.h"


namespace Storage
{
	// Objects
	FC_SimpleTasker tasker;
	FC_TaskPlanner taskPlanner(config::MaxAmtOfTaskPlannerTasks);
	FC_MainCommunication com(&Serial, 100);
	FC_ExternalADC extADC(&taskPlanner);
	LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
	LcdHandler display;
	AndroidCommunication androidCom;
	GestureRecognizer gestureRecognizer;


	// control sticks
	FC_ControlStick thrStick(new ThrottleStickAnalogSource);
	FC_ControlStick rotStick(new RotateStickAnalogSource);
	FC_ControlStick TB_Stick(new TB_StickAnalogSource);
	FC_ControlStick LR_Stick(new LR_StickAnalogSource);


	// Other types
	armStateType armState = disarmed;
}

