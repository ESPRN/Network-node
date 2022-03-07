#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <MANAGER.h>

/*
	TODO:
		- pPort project to ESP-IDF so i can actualy see the backtraces,
		- write the router code,
		- write the node manager code.
*/

// led pin on message recieve
#define LED_PIN_MESSAGE_RCV 18

// led pin on message send
#define LED_PIN_MESSAGE_SND 22

uint8_t data[4] = { 1, 2, 3, 4 };

MANAGER* node_manager;

void setup() 
{
	Serial.begin(115200);

	delay(1000);

	pinMode(LED_PIN_MESSAGE_RCV, OUTPUT);
	pinMode(LED_PIN_MESSAGE_SND, OUTPUT);
	pinMode(EXTRA_PIN, OUTPUT);

	digitalWrite(LED_PIN_MESSAGE_RCV, LOW);
	digitalWrite(LED_PIN_MESSAGE_SND, LOW);
	digitalWrite(EXTRA_PIN, LOW);

	node_manager = new MANAGER();

	node_manager->init(CHANNEL, ENCRYPTION);

	#ifdef DEBUG_MODE
	Serial.print("Relay node running.\n");
	Serial.printf("Channel: %3d\n", CHANNEL);
	Serial.printf("Encryption: %3d\n", ENCRYPTION);
	#endif
}

void loop() 
{
	const uint8_t message[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	bool sent = node_manager->send_message((const uint8_t*)message, 8);
	#ifdef DEBUG_MODE
	if(sent)
		printf("Sent\n");
	else
		printf("Not sent\n");
	#endif

	delay(1000);
}