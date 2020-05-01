#include "AndroidCommunication.h"
#include "Storage.h"
#include "config.h"


AndroidCommunication* AndroidCommunication::instance = nullptr;

// Async connecting function definition
void wifiAsyncConnecting();


AndroidCommunication::AndroidCommunication()
{

}


void AndroidCommunication::connectWithWiFiAsync(FC_TaskPlanner* taskPlanner)
{
	if (connectingPending)
		return;

	WiFi.mode(WIFI_STA);
	WiFi.begin(config::androidComm.SSID, config::androidComm.PASS);

	// begin async connecting
	connectingAttemptStartTime = millis();
	taskPlannerPtr = taskPlanner;
	connectingPending = true;
	wifiAsyncConnecting();
}


void wifiAsyncConnecting()
{
	AndroidCommunication* andCommInst = AndroidCommunication::getInstance();
	if (WiFi.status() == WL_CONNECTED)
	{
		// wifi connection established
		andCommInst->localIPAddress = WiFi.localIP();
		andCommInst->udp.begin(config::androidComm.port);

		// add new tasker task
		Storage::tasker.addTask(andCommInst, config::androidComm.receivingInterval, 0);

		andCommInst->connectingPending = false;
	}
	else
	{
		// not connected, check if not timeout and try again
		if (millis() < andCommInst->connectingAttemptStartTime + ((uint32_t)config::androidComm.connectingTimeTimeout * 1000))
			andCommInst->taskPlannerPtr->scheduleTask(wifiAsyncConnecting, 500); // try to reconnect in 500 ms
		else
			andCommInst->connectingPending = false;
	}
}


void AndroidCommunication::execute()
{
	// TODO: check if work with available() before (but should work as it is)
	int packetSize = udp.parsePacket();

	if (packetSize)
	{
		int recPacketSize = udp.read(receiveBuffer, MAX_PACKET_SIZE);
		// receiveBuffer[recPacketSize] = 0; // i don't know what is it for in the example

		// !!!
		// TODO: Do something with received data
		if (receiveBuffer[0] == 0 && recPacketSize == 18) // if packet ID == 0 and size match
		{
			SendData::tunedControllerID = receiveBuffer[1];
			uint8_t* temp = (uint8_t*)receiveBuffer; // the same array but of uint8_t type
			SendData::tunedPID_values.P = uint8ArrayToInt(temp + 2) / 100.f;
			SendData::tunedPID_values.I = uint8ArrayToInt(temp + 6) / 100.f;
			SendData::tunedPID_values.D = uint8ArrayToInt(temp + 10) / 100.f;
			SendData::tunedPID_values.I_max = uint8ArrayToInt(temp + 14);

			// send new PID values to drone
			Storage::comm.sendDataPacket(&SendData::DP_PID_params);
		}
	}
}


bool AndroidCommunication::isConnected()
{
	return WiFi.status() == WL_CONNECTED;
}


// Merge ints divided in the Android app to 4 x uint8
int AndroidCommunication::uint8ArrayToInt(uint8_t* arr)
{
	int result = arr[0] << 24 | (arr[1] & 0xFF) << 16 | (arr[2] & 0xFF) << 8 | (arr[3] & 0xFF);
	return result;
}

