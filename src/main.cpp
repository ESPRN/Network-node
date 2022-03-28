#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <MANAGER.h>

/*
	TODO:
		- Port project to ESP-IDF so i can actualy see the backtraces,
		- write the router code,
		- write the node manager code.
*/

uint8_t data[4] = { 1, 2, 3, 4 };

void setup() 
{
	Serial.begin(115200);

	delay(1000);

	manager_init(CHANNEL, ENCRYPTION);

	#ifdef DEBUG_MODE
	Serial.print("Relay node running.\n");
	Serial.printf("Channel: %3d\n", CHANNEL);
	Serial.printf("Encryption: %3d\n", ENCRYPTION);
	#endif
}

void loop()
{
	#ifdef DEBUG_SENDER
	const uint8_t message[8] = {1, 2, 3, 2, 5, 6, 10, 8};
	bool sent = manager_send((const uint8_t*)message, 8);
	#ifdef DEBUG_MODE
	if(sent)
		printf("Sent\n");
	else
		printf("Not sent\n");
	#endif

	delay(1000);
	#endif
}