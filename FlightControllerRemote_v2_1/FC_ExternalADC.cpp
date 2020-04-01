/*
 * Created: 27/03/2020
 *  Author: Jan Wielgus
 *
 */

#include "FC_ExternalADC.h"

// Pointer for the friend function
// Set up in the constructor
FC_ExternalADC* extADC_ptr;

void takeADC_MeasurementWrapper();



FC_ExternalADC::FC_ExternalADC(FC_TaskPlanner* taskPlannerPtr)
{
    this->taskPlanner = taskPlannerPtr;

    // Set up pointer for the friend function
    extADC_ptr = this;
}


bool FC_ExternalADC::initialize(bool needToBeginWire_flag)
{
    if (needToBeginWire_flag)
        Wire.begin();

    adc.initialize();

    // Setup device

    adc.setMode(ADS1115_MODE_SINGLESHOT);
    adc.setRate(ADS1115_RATE_860); // TEST !!!
    // Set the gain (PGA) +/- 6.144V
    // Note that any analog input must be higher than -0.3V and less than VDD +0.3
    adc.setGain(ADS1115_PGA_6P144);


    // !!!
    // ADDITIONAL PIN CAN BE USED TO INDICATE CONVERSION READY
    /*
    pinMode(alertReadyPin, INPUT_PULLUP);
    adc.setConversionReadyPinMode();
    */


    // begin to continuously
    triggerContinuousADC_Mesasurement();


    return true;
}


void FC_ExternalADC::setFastClock()
{
    Wire.setClock(400000L);
}


uint16_t FC_ExternalADC::getReading(uint8_t pin)
{
    if (pin >= 4)
        return 0;

    return measurementArr[pin];
}


void FC_ExternalADC::triggerContinuousADC_Mesasurement()
{
    adc.setMultiplexer(ADS1115_MUX_P0_NG); // first pin
    currentReadPin = 0;
    adc.triggerConversion();

    // plan first takeADC_MeasurementWrapper() funciton execution
    taskPlanner->scheduleTaskMicroseconds(takeADC_MeasurementWrapper, CONVERSTION_TIME);
}


void FC_ExternalADC::takeADC_Measurement()
{
    // get the value
    //measurementArr[currentReadPin] = adc.getMilliVolts(false);
    measurementArr[currentReadPin] = adc.getConversion();

    // plan next reading
    currentReadPin++;
    if (currentReadPin >= 4)
        currentReadPin -= 4;
    switch (currentReadPin)
    {
    case 0:
        adc.setMultiplexer(ADS1115_MUX_P0_NG);
        break;
    case 1:
        adc.setMultiplexer(ADS1115_MUX_P1_NG);
        break;
    case 2:
        adc.setMultiplexer(ADS1115_MUX_P2_NG);
        break;
    case 3:
        adc.setMultiplexer(ADS1115_MUX_P3_NG);
        break;
    }
    adc.triggerConversion();
    taskPlanner->scheduleTaskMicroseconds(takeADC_MeasurementWrapper, FC_ExternalADC::CONVERSTION_TIME);
}




void takeADC_MeasurementWrapper()
{
    extADC_ptr->takeADC_Measurement();
}




