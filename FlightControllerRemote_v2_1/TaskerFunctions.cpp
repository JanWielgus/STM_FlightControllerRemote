#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"

using namespace Storage;



void addTaskerFunctionsToTasker()
{
	using namespace TaskerFunction;


	// Communication
	tasker.addTask(new UpdateSteeringSending, 3703L, 0); // 270Hz (!!! TO CHECK ! I don't know why some packets are lost and don't arrive)
	tasker.addTask(new UpdateOtherSending, 200000L, 0); // 5Hz
	tasker.addTask(&comm, 22000L, 0); // 45Hz

	// Android communication task is automatically added after successful connection with wifi

	// add receive data packets
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_basic, 4);
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_full, 4);

	// received packet events
	ReceiveData::DP_basic.setPacketEvent(new BasicReceivedUpdate);
	ReceiveData::DP_full.setPacketEvent(new FullReceivedUpdate);

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
		SendData::throttle = thrStick.getValue();
		SendData::rot_stick = rotStick.getValue();
		SendData::TB_stick = TB_Stick.getValue();
		SendData::LR_stick = LR_Stick.getValue();

		comm.sendDataPacket(&SendData::DP_steering);
	}


	void UpdateOtherSending::execute()
	{
		// Update sent values before sending

		// Set flight mode
		if (armState != armStateType::armed)
			SendData::flightMode = FlightModeType::UNARMED;
		else
		{
			//if (digitalRead(config::pin.rightSwitch) == LOW)
				SendData::flightMode = FlightModeType::STABILIZE;
			//else
				//SendData::flightMode = FlightModeType::ALT_HOLD;
		}



		// OTHER DATA !!!


		// send packed data
		//comm.sendDataPacket(&SendData::DP_basicBackground);
		comm.sendDataPacket(&SendData::DP_fullBackground);


		// !!!
		//
		// THIS HAVE TO BE REBUILT !!!
		/*
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
		}*/
	}


	void BasicReceivedUpdate::execute()
	{
		// When basic data packet was received
		// ...

	}


	void FullReceivedUpdate::execute()
	{
		BasicReceivedUpdate::execute();

		// When full data packet was received
		// ...

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

