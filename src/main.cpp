#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <NODE.h>

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

//extra
#define EXTRA_PIN 33

// default relay coms channel is 7 (from 0 - 14)
#ifndef CHANNEL
    #define CHANNEL 7
#endif

// default relay node id should start with "RELAY_NODE" to get recognised by network
#ifndef NODE_ID
	#define NODE_ID "RELAY_NODE_1"
#endif

// default password for the relay network
#ifndef PASS
	#define PASS "DEFAULT_PASS"
#endif

// default encryption will be turned off
#ifndef ENCRYPTION
	#define ENCRYPTION 0
#endif

// max connections the node can have default is 20 can be lowered if necessary
#ifndef MAX_CONNECTIONS
	#define MAX_CONNECTIONS 20
#endif

NODE* node_relay;

uint8_t data[4] = { 1, 2, 3, 4 };

void sendFunc(const uint8_t *mac_addr, esp_now_send_status_t status)
{
	if(status == esp_now_send_status_t::ESP_NOW_SEND_SUCCESS)
		Serial.println("send successful");  
	else
		Serial.println("send not successful"); 
	digitalWrite(LED_PIN_MESSAGE_SND, !digitalRead(LED_PIN_MESSAGE_SND));
}

void recieveFunc(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
	Serial.println("recieved a message");
	digitalWrite(LED_PIN_MESSAGE_RCV, !digitalRead(LED_PIN_MESSAGE_RCV));
}

void setup() 
{
	Serial.begin(115200);

	delay(1000);

	node_relay = new NODE ((uint8_t)CHANNEL, (char*)NODE_ID, (char*)PASS, (bool)ENCRYPTION, (uint8_t)MAX_CONNECTIONS);

	pinMode(LED_PIN_MESSAGE_RCV, OUTPUT);
	pinMode(LED_PIN_MESSAGE_SND, OUTPUT);
	pinMode(EXTRA_PIN, OUTPUT);

	digitalWrite(LED_PIN_MESSAGE_RCV, LOW);
	digitalWrite(LED_PIN_MESSAGE_SND, LOW);
	digitalWrite(EXTRA_PIN, LOW);

	Serial.print("Relay node \"");
	Serial.print(NODE_ID);
	Serial.println("\" running...");
	Serial.printf("Channel: %3d\n", CHANNEL);
	Serial.printf("Encryption: %3d\n", ENCRYPTION);
	Serial.printf("Max connections: %3d\n", MAX_CONNECTIONS);

	node_relay->register_send_cb(sendFunc);

	node_relay->register_recieve_cb(recieveFunc);
	
}

void loop() 
{
	digitalWrite(EXTRA_PIN, !digitalRead(EXTRA_PIN));
	//bool sent = node_relay->sendData(data, 4);
	//if(sent != true)
	//	Serial.println("Error happened while sending");
	delay(1000);
}