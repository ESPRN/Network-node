#include "NODE.h"

NODE::NODE()
    :channel(CHANNEL), pmk(0), encrypt(ENCRYPTION)
{
    init();
}

NODE::NODE(uint8_t* pmk, const uint8_t channel, bool encrypt = false)
    :channel(channel), pmk(pmk), encrypt(encrypt)
{
    init();
}

NODE::NODE(const uint8_t channel, bool encrypt = false)
    :channel(channel), pmk(0), encrypt(encrypt)
{
    init();
}

NODE::~NODE()
{
    ;
}

void NODE::init()
{
    WiFi.mode(WIFI_STA);

    printf("Device MAC: %s\n", WiFi.macAddress().c_str());

    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

    if(esp_now_init() == ESP_OK)
    {
        printf("EPS-NOW initialised\n");
    }
    else
    {
        printf("ESP-NOW initialisation failed... Restarting ESP...\n");

        ESP.restart();
    }

    // esp_now_set_pmk(pmk);

    memcpy(&brodcastPeer.peer_addr, brodcastAddress, 6);

    brodcastPeer.encrypt = encrypt;

    brodcastPeer.channel = channel;

    brodcastPeer.ifidx = wifi_interface_t::ESP_IF_WIFI_STA;

    if(esp_now_add_peer((const esp_now_peer_info_t*)&brodcastPeer) == ESP_OK)
    {
        #ifdef DEBUG_MODE
        printf("Peer added\n");
        #endif
    }
}

void NODE::register_send_cb(esp_now_send_cb_t sendFunc)
{
    esp_err_t response = esp_now_register_send_cb(sendFunc);

    if(response == ESP_OK)
    {
        Serial.println("Registered send callback");
    }
    else if(response == ESP_ERR_ESPNOW_NOT_INIT)
    {
        Serial.println("Could not register send callback becouse ESP-NOW is not initialised");
    }
    else if(response == ESP_ERR_ESPNOW_INTERNAL)
    {
        Serial.println("Could not register send callback due to internal error");
    }
}

void NODE::register_recieve_cb(esp_now_recv_cb_t recieveFunc)
{
    esp_err_t response = esp_now_register_recv_cb(recieveFunc);

    if(response == ESP_OK)
    {
        Serial.println("Registered recieve callback");
    }
    else if(response == ESP_ERR_ESPNOW_NOT_INIT)
    {
        Serial.println("Could not register recieve callback becouse ESP-NOW is not initialised");
    }
    else if(response == ESP_ERR_ESPNOW_INTERNAL)
    {
        Serial.println("Could not register recieve callback due to internal error");
    }
}

uint8_t NODE::get_pmk()
{
    return *pmk;
}

uint8_t NODE::get_channel()
{
    return channel;
}

bool NODE::sendData(const uint8_t* data, uint8_t dataLen)
{
    if(!esp_now_is_peer_exist(brodcastAddress))
    {
        #ifdef DEBUG_MODE
        printf("Peer was not added\n");
        #endif
        esp_now_add_peer((const esp_now_peer_info_t*)&brodcastPeer);
    }

    if(dataLen > ESP_NOW_MAX_DATA_LEN) return false;

    uint8_t* cutData = (uint8_t*)malloc(dataLen*sizeof(uint8_t));

    memcpy(cutData, data, dataLen * sizeof(uint8_t));

    esp_err_t result = esp_now_send(brodcastAddress, cutData, dataLen);

    free(cutData);

    if(result == ESP_OK)
        return true;
    else if(result == ESP_ERR_ESPNOW_NOT_FOUND)
    {
        Serial.println("Not found");
        return false;
    }
    else if(result == ESP_ERR_ESPNOW_IF)
    {
        Serial.println("WiFi interface does not match");
        return false;
    }
    else return false;
}