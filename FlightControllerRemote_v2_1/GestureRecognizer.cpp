#include "GestureRecognizer.h"
#include "Storage.h"
#include "config.h"


using Storage::armState;
using Storage::thrStick;
using Storage::rotStick;
using Storage::TB_Stick;
using Storage::LR_Stick;



GestureRecognizer::GestureRecognizer()
{
	// by now do nothing
}


// 10Hz
void GestureRecognizer::recognizeArmingAndDisarmingGesture()
{
	/*
	if (digitalRead(config::pin.rightSwitch) == HIGH)
		armState = armStateType::disarmed;
	else
		armState = armStateType::armed;
		*/


	thr = thrStick.getValue();
	rot = rotStick.getValue();
	tb = TB_Stick.getValue();
	lr = LR_Stick.getValue();
	static bool stage1Flag = false;

	if (thr == 0 && tb == 0 && lr == 0)
	{
		if (stage1Flag == false && rot > 460)
		{
			// change arm state to the opposite
			if (armState == armStateType::armed)
				armState = armStateType::disarmed;
			else armState = armStateType::armed;
			stage1Flag = true;
		}

		if (rot <= 100)
			stage1Flag = false;
	}
	else
		stage1Flag = false;
	




	/*
	thr = thrStick.getValue();
	rot = rotStick.getValue();
	tb = TB_Stick.getValue();
	lr = LR_Stick.getValue();

	static bool state1Flag = false;
	static uint32_t state1Time;

	// Arming / Disarming
	{
		// Check if sticks are in position to change state
		if (thr == 0 && tb == 0 && lr == 0)
		{
			if (armState == disarmed)
			{
				//////////////////
				// Arming

				if (rot == 0)
				{
					state1Flag = true;
					state1Time = tasker.getCurrentTime();
				}
				// ...

				// Arming end
				//////////////////
			}

			else if (armState == armed)
			{
				//////////////////
				// Disarming

				// Disarming end
				//////////////////
			}
		}
		else
			state1Flag = false;
	}
	*/
}

