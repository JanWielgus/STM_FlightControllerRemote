/*
 * Storage.h
 *
 * Created: 20/07/2019 19:27:18
 *  Author: Jan Wielgus
 *
 *
 * All global variables and objects are created here
 * (everything needed by multiple files)
 */ 


#ifndef STORAGE_H_
#define STORAGE_H_

#include <FC_ObjectTasker.h>
#include <FC_Task.h>
#include <FC_TaskPlanner.h>
#include <FC_CommunicationHandler.h>
#include <FC_SerialCommBase.h>
#include "FC_ESP8266_WiFiComm.h"
#include "FC_ExternalADC.h"
#include "FC_ControlStick.h"
#include <LiquidCrystal_I2C.h>
#include "LCDhandler.h"
#include "GestureRecognizer.h"
#include "CommRecDataPackets.h"
#include "CommSendDataPackets.h"


// New data types
enum armStateType { disarmed, armed };

// list of all flight modes
enum FlightModeType
{
    UNARMED, // used when drone have to be unarmed
    STABILIZE, // keeps drone level
    ALT_HOLD, // extend stabilize, take control of throttle and keep altitude
    POS_HOLD, // extend altHold, take control of position
};


namespace Storage
{
    // All global flags
    struct
    {
        // all flags
        bool testFlagToDelete = true;
    } flags;


    // Objects
    extern FC_ObjectTasker tasker;
    extern FC_TaskPlanner taskPlanner;
    extern FC_SerialCommBase serialCommBase;
    extern FC_ESP8266_WiFiComm wifiCommBase;
    extern FC_CommunicationHandler comm;
    extern FC_ExternalADC extADC;
    extern LiquidCrystal_I2C lcd;
    extern LcdHandler display;
    extern GestureRecognizer gestureRecognizer;


    // control sticks
    extern FC_ControlStick thrStick;
    extern FC_ControlStick rotStick;
    extern FC_ControlStick TB_Stick;
    extern FC_ControlStick LR_Stick;


    // Other types
    extern armStateType armState; // old "state"
}



#endif /* STORAGE_H_ */