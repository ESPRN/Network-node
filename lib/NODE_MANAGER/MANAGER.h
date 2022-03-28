/*
    @ Name: ESP-NOW mashed network
    @ Author: Tevž Beškovnik
    @ Date: 28. 1. 2022
    @ Description: This class is the manager that connects the routing software and manages the node software for the network relay
    @ Notes:
*/

#ifndef _MANAGER_
#define _MANAGER_

#ifdef DEBUG_MODE

#ifndef DEBUG_PIN_1
#define DEBUG_PIN_1 33
#endif

#ifndef DEBUG_PIN_2
#define DEBUG_PIN_2 18
#endif

#ifndef DEBUG_PIN_3
#define DEBUG_PIN_3 2
#endif

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

extern NODE* node;

extern push_heap_s traffic_cache;

bool manager_send(const uint8_t* message, uint8_t len);

void manager_init(uint8_t channel, bool encryption);

#endif