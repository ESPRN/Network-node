/*
    @ Name: ESP-NOW mashed network
    @ Author: Tevž Beškovnik
    @ Date: 28. 1. 2022
    @ Description: This class is the manager that connects the routing software and manages the node software for the network relay
    @ Notes:
*/

#ifndef _MANAGER_
#define _MANAGER_

#include "../NODE/NODE.h"
#include "../CACHE/CACHE.h"

class MANAGER
{
    public:
        ~MANAGER();

        void init(uint8_t* primaryKey, const uint8_t channel, bool encrypt);

        void init(const uint8_t channel, bool encrypt);

    private:
        void send_function(const uint8_t *mac_addr, esp_now_send_status_t status);

        void recieve_function(const uint8_t *mac_addr, const uint8_t *data, int data_len);

        push_heap_s traffic_cache;
        NODE node;
};

#endif