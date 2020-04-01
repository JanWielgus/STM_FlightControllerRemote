// FC_ExternalADC.h
/*
 * Created: 27/03/2020
 *  Author: Jan Wielgus
 *
 */

#ifndef _FC_EXTERNALADC_h
#define _FC_EXTERNALADC_h

#include "arduino.h"

#include "externalLibraries/I2Cdev.h"
#include "externalLibraries/ADS1115.h"
#include <FC_TaskPlanner.h>


class FC_ExternalADC
{
public:
    FC_ExternalADC(FC_TaskPlanner* taskPlannerPtr);
    bool initialize(bool needToBeginWire_flag = true);
    void setFastClock();
    uint16_t getReading(uint8_t pin); // pin values are from 0 to 3


private:
    void triggerContinuousADC_Mesasurement(); // begin to measure new values
    void takeADC_Measurement(); // get new value from the device and trigger the next

    friend void takeADC_MeasurementWrapper();

private:
    ADS1115 adc = ADS1115(ADS1115_DEFAULT_ADDRESS);
    FC_TaskPlanner* taskPlanner;

    uint16_t measurementArr[4] = {};
    uint8_t currentReadPin = 0; // used to change shots on next pins round and round

    static const uint16_t CONVERSTION_TIME = 1164; // 1000000 / RATE (check rate in initialize() )
};


#endif

