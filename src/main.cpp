#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include "../lib/NODE/NODE.h"

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

node_info_s pairedNode;

bool paired = false;

NODE node_relay((uint8_t)CHANNEL, (char*)NODE_ID, (char*)PASS, (bool)ENCRYPTION, (uint8_t)MAX_CONNECTIONS);

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

void updateConnections(node_info_s nodeInfo)
{
	pairedNode = nodeInfo;
	Serial.print("Paired with new node: ");
	Serial.println(nodeInfo.peerName);
}

void setup() 
{
	pinMode(LED_PIN_MESSAGE_RCV, OUTPUT);
	pinMode(LED_PIN_MESSAGE_SND, OUTPUT);

	digitalWrite(LED_PIN_MESSAGE_RCV, LOW);
	digitalWrite(LED_PIN_MESSAGE_SND, LOW);

	Serial.begin(115200);

	Serial.print("Relay node \"");
	Serial.print(NODE_ID);
	Serial.println("\" running...");
	Serial.printf("Channel: %3d\n", CHANNEL);
	Serial.printf("Encryption: %3d\n", ENCRYPTION);
	Serial.printf("Max connections: %3d\n", MAX_CONNECTIONS);

	node_relay.register_send_cb(&sendFunc);
	Serial.println("Registered send callback");

	node_relay.register_recieve_cb(&recieveFunc);
	Serial.println("Registered recieve callback");

	node_relay.update_connections_cb(&updateConnections);
	Serial.println("Registered new connection callback");
}

void loop() 
{
	if(!paired)
		paired = node_relay.dynamic_pair();
	if(paired)
		node_relay.sendData(pairedNode.peerInfo.peer_addr, data, 4);
}