/*
 * config.h
 *
 * Created: 01/07/2019 12:03:17
 *  Author: Jan Wielgus
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


namespace config
{
	const uint8_t MaxAmtOfTaskerTasks = 25; // tasks array size inside tasker
	const uint8_t MaxAmtOfTaskPlannerTasks = 10; // max amount of tasks planned at once

	const uint8_t MaxCommPacketBytes = 100; // communication buffer size


	// Pins
	struct
	{
		uint8_t redDiode = D3;
		uint8_t greenDiode = D4;
		
		// steering (pins on the external ADC)
		uint8_t throttle = 0;
		uint8_t rotate = 1;
		uint8_t tiltTB = 2;
		uint8_t tiltLR = 3;
		
		// switches
		uint8_t leftSwitch = D8;
		uint8_t rightSwitch = D7;
		
		
		// radio module
		uint8_t m0pin = D5;
		uint8_t m1pin = D6;
		uint8_t aux = D0;
		
	} const pin;
	
	
	// Addresses
	const uint8_t LCD_ADDRESS = 0x3F;


	// Analog reading reverse flags
	// Change before sticks calibration
	struct
	{
		int16_t maxReadingValue = 30000;
		int16_t minReadingValue = 0;
		bool thr = true;
		bool rot = true;
		bool tb = true;
		bool lr = true;
	} const stickAnalogReverse;
	
	
	// Stick default border values (maximum tilts)
	struct
	{
		// throttle
		uint16_t thrMin = 13600; // mapped to final min
		uint16_t thrCen = 13600; // mapped to final center
		uint16_t thrMax = 28870; // mapped to final max
		// rotate
		uint16_t rotMin = 13500; // mapped to final min
		uint16_t rotCen = 21255; // mapped to final center
		uint16_t rotMax = 28900; // mapped to final max
		// TB tilt
		uint16_t TB_Min = 14220; // mapped to final min
		uint16_t TB_Cen = 22360; // mapped to final center
		uint16_t TB_Max = 29000; // mapped to final max
		// LR tilt
		uint16_t LR_Min = 13080; // mapped to final min
		uint16_t LR_Cen = 20910; // mapped to final center
		uint16_t LR_Max = 28650; // mapped to final max


		// Note: eg. if final center have to be the same as final minimum,
		//			set raw center to the same value as raw minimum

	} sticksCalibVal;
	
	const uint16_t stickDeadZone = 450; // 450
	const uint8_t stickFilterIntensity = 50;
}





#endif /* CONFIG_H_ */

