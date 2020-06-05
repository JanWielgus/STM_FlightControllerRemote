#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"

using namespace Storage;



void addTaskerFunctionsToTasker()
{
	using namespace TaskerFunction;


	// Communication
	tasker.addTask(new UpdateSteeringSending, 4000L, 0); // 250Hz
	tasker.addTask(new UpdateOtherSending, 200000L, 0); // 5Hz
	tasker.addTask(&comm, 22000L, 0); // 45Hz - receiving

	// add receive data packets
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_basic, 4);
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_full, 4);
	// android (attached to main communication as long as it is over wifi)
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_androidPID_tuning, 2);

	// received packet events
	ReceiveData::DP_basic.setPacketEvent(new BasicReceivedUpdate);
	ReceiveData::DP_full.setPacketEvent(new FullReceivedUpdate);
	// android
	ReceiveData::DP_androidPID_tuning.setPacketEvent(new AndroidPID_TuningReceivedUpdate);

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
			if (digitalRead(config::pin.rightSwitch) == HIGH)
				SendData::flightMode = FlightModeType::STABILIZE;
			else
				SendData::flightMode = FlightModeType::ALT_HOLD;
		}



		// OTHER DATA !!!


		// send packed data
		//comm.sendDataPacket(&SendData::DP_basicBackground);
		comm.sendDataPacket(&SendData::DP_fullBackground);
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


	void AndroidPID_TuningReceivedUpdate::execute()
	{
		SendData::tunedControllerID = ReceiveData::andrTunedControllerID;
		SendData::tunedPID_values.P = ReceiveData::AndrTunedPID_values.P / 100.f;
		SendData::tunedPID_values.I = ReceiveData::AndrTunedPID_values.I / 100.f;
		SendData::tunedPID_values.I_max = (uint16_t)ReceiveData::AndrTunedPID_values.I_max;
		SendData::tunedPID_values.D = ReceiveData::AndrTunedPID_values.D / 100.f;

		// send new values
		Storage::comm.sendDataPacket(&SendData::DP_PID_params);
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

