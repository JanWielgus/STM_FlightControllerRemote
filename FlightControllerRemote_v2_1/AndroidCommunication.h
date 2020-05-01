// AndroidCommunication.h

#ifndef _ANDROIDCOMMUNICATION_h
#define _ANDROIDCOMMUNICATION_h

#include <Arduino.h>
#include <FC_Task.h>
#include <FC_TaskPlanner.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


class AndroidCommunication : public FC_Task
{
	// singleton
private:
	static AndroidCommunication* instance;
	AndroidCommunication(); // constructor
public:
	static AndroidCommunication* getInstance()
	{
		if (instance == nullptr)
			instance = new AndroidCommunication();
		return instance;
	}

	// the rest of the class

public:
	void connectWithWiFiAsync(FC_TaskPlanner* taskPlanner); // asynchronically connects with wifi and add this to tasker
	void execute() override; // receiving
	bool isConnected();
	// TODO: add sending method if will be a need (example in esp arduino wifi udp example)

private:
	int uint8ArrayToInt(uint8_t* arr);

	uint32_t connectingAttemptStartTime; // in millis
	FC_TaskPlanner* taskPlannerPtr; // for async connecting process
	bool connectingPending = false; // prevent from starting connection several time simultaneously

	// sending and receiving buffers
	static const uint16_t MAX_PACKET_SIZE = 100;
	char receiveBuffer[MAX_PACKET_SIZE + 1];
	char sendBuffer[MAX_PACKET_SIZE + 1];

	WiFiUDP udp;
	IPAddress localIPAddress;
	friend void wifiAsyncConnecting(); // friend function used for asyns connecting
};


#endif

