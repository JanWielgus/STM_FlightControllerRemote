#ifndef TASKERFUNCTIONS_H_
#define TASKERFUNCTIONS_H_

#include <FC_Task.h>

void addTaskerFunctionsToTasker();

namespace TaskerFunction
{
// Communication

	class UpdateSteeringSending : public FC_Task
	{
		void execute() override;
	};

	class UpdateOtherSending : public FC_Task
	{
		void execute() override;
	};

	class BasicReceivedUpdate : public FC_Task
	{
	protected:
		void execute() override;
	};

	class FullReceivedUpdate : public BasicReceivedUpdate
	{
		void execute() override;
	};


	class AndroidPID_TuningReceivedUpdate : public FC_Task
	{
		void execute() override;
	};


// Sticks

	class ReadControlSticksValues : public FC_Task
	{
		void execute() override;
	};

	class GestureRecognition : public FC_Task
	{
		void execute() override;
	};


// Display

	class UpdateDisplayFast : public FC_Task
	{
		void execute() override;
	};

	class UpdateDisplaySlow : public FC_Task
	{
		void execute() override;
	};


// Other

	class UpdateControlDiode : public FC_Task
	{
		bool ledState = LOW;
		void execute() override;
	};
	
}


#endif /* TASKERFUNCTIONS_H_ */