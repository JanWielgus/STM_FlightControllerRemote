/*
 * ControlStick.h
 *
 * Created: 01/07/2019
 *  Author: Jan Wielgus
 */ 


#ifndef _FC_CONTROLSTICK_h
#define _FC_CONTROLSTICK_h

#include "arduino.h"
#include <FC_EVA_Filter.h>
#include "config.h"

class IStickAnaglogSource;


class FC_ControlStick
{
public:
	FC_ControlStick(IStickAnaglogSource* analogSource);
	void setProperties(uint16_t rawMinVal, uint16_t rawCenterVal, uint16_t rawMaxVal,
						int16_t finalMinVal, int16_t finalCenVal, int16_t finalMaxVal,
						uint16_t deadZone = 0);
		// min/maxRawVal - readings of the extreme stick tilts
		// rawCenterValue - raw value in the center
		// min/cen/maxFinalVal - range of the calculated output value ( getValue() )
		// reverseFlag - true -> need to reverse the value
		// deadZone - range where despite stick movement result value is finalCenterValue
		
	
	void readValue(); // need to be called in the specific frequency
	uint16_t getRawValue(); // Use this for calibration
	int16_t getValue(); // calculated stick value
	void setFilterIntensity(uint8_t intensity); // 0-99, 0 - no filtering, 99- huge filtering
	//void setRawBorderValues(uint16_t minVal, uint16_t cenVal, uint16_t maxVal);
	void setDeadZone(uint16_t deadZone);

 
private:
	IStickAnaglogSource* analogSource; // passed inside the contructor, use this for reading the analog values

	FC_EVA_Filter filter;
	
	int16_t rawValue;
	int16_t value;
	
	struct
	{
		uint16_t rawMinValue;
		uint16_t rawCenterValue;
		uint16_t rawMaxValue;
	} calib;
	
	struct
	{
		int16_t finalMinValue;
		int16_t finalCenterValue;
		int16_t finalMaxValue;
		uint16_t deadZone;
	} outputProperties;
	
};





///////////////////// \/   \/   \/
//
// ANALOG SOURCE CHANGE INSIDE THIS CLASSES
// 
///////////////////// \/   \/   \/

class IStickAnaglogSource
{
public:
	virtual uint16_t getValue() = 0;
protected:
	uint16_t getReversed(uint16_t val)
	{
		int16_t temp = val - int16_t(config::stickAnalogReverse.minReadingValue + config::stickAnalogReverse.maxReadingValue);
		return abs(temp);
	}
};


class ThrottleStickAnalogSource : public IStickAnaglogSource
{
	uint16_t getValue() override;
};

class RotateStickAnalogSource : public IStickAnaglogSource
{
	uint16_t getValue() override;
};

class TB_StickAnalogSource : public IStickAnaglogSource
{
	uint16_t getValue() override;
};

class LR_StickAnalogSource : public IStickAnaglogSource
{
	uint16_t getValue() override;
};



#endif

