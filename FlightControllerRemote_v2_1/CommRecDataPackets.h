// CommRecDataPackets.h

#ifndef _COMMRECDATAPACKETS_h
#define _COMMRECDATAPACKETS_h

#include "arduino.h"
#include <DataPacketBase.h>
#include <FC_CustomDataTypes.h>

namespace ReceiveData
{
	struct androidPidCommPack
	{
		int32Byte P;
		int32Byte I;
		int32Byte D;
		int32Byte I_max;
	};


	extern int8_t tilt_TB; // top/back
	extern int8_t tilt_LR; // left/right
	extern int16Byte heading;
	extern int16Byte altitude;
	extern int32Byte longitude;
	extern int32Byte latitude;
	extern bitByte errorBits1;
	extern bitByte errorBits2;
	extern bitByte bitArray1;
	extern uint8_t receivingConnectionStability;

	// from android device
	extern uint8_t andrTunedControllerID; // which controller is tuned
	extern androidPidCommPack AndrTunedPID_values; // PID values of that controller



	namespace PacketClasses
	{
		class Basic : public DataPacketBase
		{
		public:
			Basic()
				: DataPacketBase(0)
			{
				// byte types
				addByteType(errorBits1);
				addByteType(errorBits2);
				addByteType(bitArray1);
				add_uint8_t(receivingConnectionStability);
			}
		};


		class Full : public DataPacketBase
		{
		public:
			Full()
				: DataPacketBase(1)
			{
				// add voltage

				// byte types
				addByteType(heading);
				addByteType(altitude);
				addByteType(longitude);
				addByteType(latitude);
				addByteType(errorBits1);
				addByteType(errorBits2);
				addByteType(bitArray1);

				// other types
				add_uint8_t(receivingConnectionStability);
				add_int8_t(tilt_TB);
				add_int8_t(tilt_LR);
			}
		};



		// from android

		class AndroidPID_Tuning : public DataPacketBase
		{
		public:
			AndroidPID_Tuning()
				: DataPacketBase(100)
			{
				add_uint8_t(andrTunedControllerID);
				addByteType(AndrTunedPID_values.P);
				addByteType(AndrTunedPID_values.I);
				addByteType(AndrTunedPID_values.D);
				addByteType(AndrTunedPID_values.I_max);
			}
		};
	}


	// Packet only instances
	extern PacketClasses::Basic DP_basic;
	extern PacketClasses::Full DP_full;
	extern PacketClasses::AndroidPID_Tuning DP_androidPID_tuning;
}



#endif

