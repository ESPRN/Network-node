/*
    @ Name: ESP-NOW mashed network
    @ Author: Tevž Beškovnik
    @ Date: 17. 1. 2022
    @ Description: Meshed network of ESP32 devices over the connectionless ESP-NOW network this file is network node
    @ Notes:
        - This uses wifi routers to auto discover other devices and pair with them while this is 
          handy and good this is not the best way to discover other devices, there could be 3rd. party devices
          protending to be part of the network
        - The data that can be recieved in one message as far as i can understand can have a length from anywhere
          from 0 to 250 bytes, so we should have a very optimised way to send messages or some sort of packet method
        - Theres a function called eps_now_set_wake_window that controlls something to do with the esp low power sleep 
          mode need to look into that a bit further

*/
#ifndef _NODE_
#define _NODE_

#include <Arduino.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <esp_err.h>
#include <string>
#include <WiFi.h>

struct node_info_s
{
    String peerName;
    esp_now_peer_info_t peerInfo;
};

class NODE{
    public:
        // this initialises all the things for esp-now, access point stuff, etc...
        NODE(uint8_t* primaryKey, const uint8_t channel, const char* SSID, const char* pass, bool encrypt, uint8_t maxConnections);

        // this also initis just doesnt take the primary key.
        NODE(const uint8_t channel, const char* SSID, const char* pass, bool encrypt, uint8_t maxConnections);

        ~NODE();

        // get the current primary key for esp-now
        uint8_t get_pmk();

        // get current channel
        uint8_t get_channel();

        // register a callback function for sending
        void register_send_cb(esp_now_send_cb_t sendFunc);

        // register a callback function for revieving messages
        void register_recieve_cb(esp_now_recv_cb_t recieveFunc);

        // send data
        bool sendData(const uint8_t* data, uint8_t dataLen);

    private:
        uint8_t channel; // message channel for ESP-NOW
        uint8_t* pmk; // primary master key
        String macAddress; // this devices mac address
        bool encrypt; // if transmition is encrypted
        const char* ssid; // ssid
        const char* pass; // password
        const uint8_t maxConnections;
        const uint8_t brodcastAddress[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        esp_now_peer_info_t brodcastPeer;
};
#endif