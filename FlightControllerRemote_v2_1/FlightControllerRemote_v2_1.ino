// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       FlightControllerRemote_v2_1.ino
    Created:	06/02/2020
    Author:     Jan Wielgus
*/


/*
	!!!

	IF CODE DOES NOT COMPILE
	ADD FILES FROM THE externalLibraries FOLDER
	TO THE SOLUTION

	!!!
*/


#include <FC_Communication_Base.h>
#include <FC_CommunicationHandler.h>
#include "CommSendDataPackets.h"
#include "CommRecDataPackets.h"
#include <FC_ObjectTasker.h>
#include <FC_Task.h>
#include <FC_TaskPlanner.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FC_GrowingArray.h>
#include <FC_CustomDataTypes.h>
#include <FC_EVA_Filter.h>
#include "externalLibraries/I2Cdev.h"
#include "externalLibraries/ADS1115.h" // 16 bit ADC library
#include "FC_ExternalADC.h"
#include "Storage.h"
#include "config.h"
#include "LCDhandler.h"
#include "TaskerFunctions.h"
#include "AndroidCommunication.h"
#include "GestureRecognizer.h"

using namespace Storage;




void setup()
{
	// Communication serial
	Serial.begin(19200);
	delay(300);

	// bluetooth software serial
	////////////////////androidCom.init();

	pinMode(LED_BUILTIN, OUTPUT);

	//switches
	pinMode(config::pin.leftSwitch, INPUT_PULLUP);
	pinMode(config::pin.rightSwitch, INPUT_PULLUP);


	// TEMPORARY !!!  for the radio module
	pinMode(config::pin.m0pin, OUTPUT);
	pinMode(config::pin.m1pin, OUTPUT);
	pinMode(config::pin.aux, INPUT);
	// set full-duplex
	digitalWrite(config::pin.m0pin, HIGH);
	digitalWrite(config::pin.m1pin, LOW);


	// initialize the external adc
	while (!extADC.initialize())
	{
		delay(500);
	}



	// Add all takser functions
	addTaskerFunctionsToTasker();

	// Makes that time has not influence connection stability value
	// Use after adding tasks to tasker
	comm.adaptConStabFilterToInterval();



	// init the control sticks
	thrStick.setProperties(config::sticksCalibVal.thrMin, config::sticksCalibVal.thrCen, config::sticksCalibVal.thrMax,
		0, 0, 1000, config::stickDeadZone);
	rotStick.setProperties(config::sticksCalibVal.rotMin, config::sticksCalibVal.rotCen, config::sticksCalibVal.rotMax,
		-500, 0, 500, config::stickDeadZone);
	TB_Stick.setProperties(config::sticksCalibVal.TB_Min, config::sticksCalibVal.TB_Cen, config::sticksCalibVal.TB_Max,
		-500, 0, 500, config::stickDeadZone);
	LR_Stick.setProperties(config::sticksCalibVal.LR_Min, config::sticksCalibVal.LR_Cen, config::sticksCalibVal.LR_Max,
		-500, 0, 500, config::stickDeadZone);

	// set stick filtering
	thrStick.setFilterIntensity(config::filterIntensity);
	rotStick.setFilterIntensity(config::filterIntensity);
	TB_Stick.setFilterIntensity(config::filterIntensity);
	LR_Stick.setFilterIntensity(config::filterIntensity);


	// display
	display.initLCD();
	display.showStaticParts();


	// Necessary for LCD to keep up
	Wire.setClock(400000L);
}

// Add the main program code into the continuous loop() function
void loop()
{
	// This are the only two thins inside the loop()
	// All other stuff have to be called using this objects
	tasker.run();
	taskPlanner.runPlanner();
}
