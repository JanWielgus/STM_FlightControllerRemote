// 
// 
// 

#include "FC_ControlStick.h"
#include "Storage.h"

FC_ControlStick::FC_ControlStick(IStickAnaglogSource* analogSource)
	:filter(0.3)
{
	this->analogSource = analogSource;
}


void FC_ControlStick::setProperties(uint16_t rawMinVal, uint16_t rawCenterVal, uint16_t rawMaxVal,
									int16_t finalMinVal, int16_t finalCenVal, int16_t finalMaxVal,
									uint16_t deadZone)
{
	calib.rawMinValue = rawMinVal;
	calib.rawCenterValue = rawCenterVal;
	calib.rawMaxValue = rawMaxVal;

	outputProperties.finalMinValue = finalMinVal;
	outputProperties.finalCenterValue = finalCenVal;
	outputProperties.finalMaxValue = finalMaxVal;

	setDeadZone(deadZone);
}


void FC_ControlStick::readValue()
{
	// Get the raw value (!! MUST FIT in int16_t type)
	rawValue = (*analogSource).getValue();
	

	// filter
	float temp = rawValue;
	value = (int16_t)(filter.updateFilter(temp) + 0.5);

	
	// calculate the output values
	if (value > calib.rawCenterValue + outputProperties.deadZone)
	{
		value = map(value,
			calib.rawCenterValue + outputProperties.deadZone,	calib.rawMaxValue,
			outputProperties.finalCenterValue,					outputProperties.finalMaxValue	);

		value = constrain(value, outputProperties.finalCenterValue, outputProperties.finalMaxValue);
	}
	else if (value < calib.rawCenterValue - outputProperties.deadZone)
	{
		value = map(value,
			calib.rawMinValue,				calib.rawCenterValue - outputProperties.deadZone,
			outputProperties.finalMinValue,	outputProperties.finalCenterValue					);

		value = constrain(value, outputProperties.finalMinValue, outputProperties.finalCenterValue);
	}
	else
		value = outputProperties.finalCenterValue;
}


uint16_t FC_ControlStick::getRawValue()
{
	return rawValue;
}


int16_t FC_ControlStick::getValue()
{
	return value;
}


void FC_ControlStick::setFilterIntensity(uint8_t intensity)
{
	intensity = constrain(intensity, 0, 99);
	filter.setFilterBeta((float)intensity / 100);
}


void FC_ControlStick::setDeadZone(uint16_t deadZone)
{
	outputProperties.deadZone = deadZone;
}



///////////////////// \/   \/   \/
//
// ANALOG SOURCE CHANGE INSIDE THIS CLASSES
// 
///////////////////// \/   \/   \/


uint16_t ThrottleStickAnalogSource::getValue()
{
	uint16_t value = Storage::extADC.getReading(config::pin.throttle);
	if (config::stickAnalogReverse.thr)
		return getReversed(value);
	return value;
}

uint16_t RotateStickAnalogSource::getValue()
{
	uint16_t value = Storage::extADC.getReading(config::pin.rotate);
	if (config::stickAnalogReverse.rot)
		return getReversed(value);
	return value;
}

uint16_t TB_StickAnalogSource::getValue()
{
	uint16_t value = Storage::extADC.getReading(config::pin.tiltTB);
	if (config::stickAnalogReverse.tb)
		return getReversed(value);
	return value;
}

uint16_t LR_StickAnalogSource::getValue()
{
	uint16_t value = Storage::extADC.getReading(config::pin.tiltLR);
	if (config::stickAnalogReverse.lr)
		return getReversed(value);
	return value;
}
