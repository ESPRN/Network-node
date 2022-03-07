/*
    @ Name: ESP-NOW mashed network
    @ Author: Tevž Beškovnik
    @ Date: 28. 1. 2022
    @ Description: This class is the manager that connects the routing software and manages the node software for the network relay
    @ Notes:
*/

#ifndef _MANAGER_
#define _MANAGER_

#ifndef EXTRA_PIN
#define EXTRA_PIN 33
#endif

// default relay coms channel is 7 (from 0 - 14)
#ifndef CHANNEL
    #define CHANNEL 7
#endif

// default encryption will be turned off
#ifndef ENCRYPTION
	#define ENCRYPTION 0
#endif

#include "../NODE/NODE.h"
#include "../CACHE/CACHE.h"

void send_function(const uint8_t *mac_addr, esp_now_send_status_t status);

void recieve_function(const uint8_t *mac_addr, const uint8_t *data, int data_len);

class MANAGER
{
    public:
        MANAGER();

        bool send_message(const uint8_t* message, uint8_t len);
};

#endif