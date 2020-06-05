// 
// 
// 

#include "CommRecDataPackets.h"


namespace ReceiveData
{
	// add voltage
	int8_t tilt_TB; // top/back
	int8_t tilt_LR; // left/right
	int16Byte heading;
	int16Byte altitude;
	int32Byte longitude;
	int32Byte latitude;
	bitByte errorBits1;
	bitByte errorBits2;
	bitByte bitArray1;
	uint8_t receivingConnectionStability;

	// from android device
	uint8_t andrTunedControllerID; // which controller is tuned
	androidPidCommPack AndrTunedPID_values; // PID values of that controller


	// Packet only instances
	PacketClasses::Basic DP_basic;
	PacketClasses::Full DP_full;
	PacketClasses::AndroidPID_Tuning DP_androidPID_tuning;
}

