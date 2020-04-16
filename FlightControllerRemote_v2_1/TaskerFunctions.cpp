#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"

using namespace Storage;



void addTaskerFunctionsToTasker()
{
	using namespace TaskerFunction;


	// Communication
	tasker.addTask(new UpdateSteeringSending, 6000L, 0); // 167Hz
	tasker.addTask(new UpdateOtherSending, 200000L, 0); // 5Hz
	tasker.addTask(new UpdateReceiving, 100000L, 0); // 10Hz

	// Sticks
	tasker.addTask(new ReadControlSticksValues, 4651L, 0); // 215Hz
	tasker.addTask(new GestureRecognition, 100000L, 0); // 10Hz

	// Display
	tasker.addTask(new UpdateDisplayFast, 100000L, 0); // 10Hz
	tasker.addTask(new UpdateDisplaySlow, 333333L, 0); // 3Hz

	// Others
	tasker.addTask(new UpdateControlDiode, 1000000L, 0); // 1Hz
}




namespace TaskerFunction
{
	// Communication

	void UpdateSteeringSending::execute()
	{
		com.toSend.steer.throttle = thrStick.getValue();
		com.toSend.steer.rotate = rotStick.getValue();
		com.toSend.steer.TB = TB_Stick.getValue();
		com.toSend.steer.LR = LR_Stick.getValue();

		com.packAndSendData(com.sendPacketTypes.TYPE4_ID, com.sendPacketTypes.TYPE4_SIZE);
	}


	void UpdateOtherSending::execute()
	{
		// Update sent values before sending

		com.toSend.arming = armState == armed ? 1 : 0;

		// Set the flight modes  ( FLIGHT MODES SYSTEM WILL CHANGE SO DO THIS )
		if (digitalRead(config::pin.rightSwitch) == LOW)
			com.toSend.flightMode = 0; // stabilize
		else
			com.toSend.flightMode = 1; // altHold


		// OTHER DATA !!!


		// send packed data
		//com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE);
		com.packAndSendData(com.sendPacketTypes.TYPE1_ID, com.sendPacketTypes.TYPE1_SIZE);


		// check if need to send PID tuning values
		if (flags.btNeedToUpdatePID)
		{
			flags.btNeedToUpdatePID = false;

			com.toSend.PIDcontrollerID = androidData.controllerID;
			com.toSend.PIDvalues.P = androidData.PID_P;
			com.toSend.PIDvalues.I = androidData.PID_I;
			com.toSend.PIDvalues.I_max = androidData.PID_Imax;
			com.toSend.PIDvalues.D = androidData.PID_D;

			com.packAndSendData(com.sendPacketTypes.TYPE3_ID, com.sendPacketTypes.TYPE3_SIZE);
		}
	}


	void UpdateReceiving::execute()
	{
		com.receiveAndUnpackData();
	}




	// Sticks

	void ReadControlSticksValues::execute()
	{
		thrStick.readValue();
		rotStick.readValue();
		TB_Stick.readValue();
		LR_Stick.readValue();
	}


	void GestureRecognition::execute()
	{
		gestureRecognizer.recognizeArmingAndDisarmingGesture();
	}




	// Display

	void UpdateDisplayFast::execute()
	{
		display.updateFastParts();
	}


	void UpdateDisplaySlow::execute()
	{
		display.updateSlowParts();
	}




	// Other

	void UpdateControlDiode::execute()
	{
		digitalWrite(LED_BUILTIN, ledState);
		ledState = !ledState;
	}

}

