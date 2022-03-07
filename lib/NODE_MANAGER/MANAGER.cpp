#include "./MANAGER.h"

NODE* node;

push_heap_s traffic_cache;

void send_function(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    #ifdef DEBUG_MODE
    if(status == esp_now_send_status_t::ESP_NOW_SEND_FAIL)
        Serial.println("Message failed to send");
    if(status == esp_now_send_status_t::ESP_NOW_SEND_SUCCESS)
        Serial.println("Message sent successfuly");
    #endif
}

void recieve_function(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    #ifdef DEBUG_MODE
    digitalWrite(2, !digitalRead(2));
    Serial.printf("sender mac address: %3d:%3d:%3d:%3d:%3d:%3d\nData: ", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.printf((const char*)data);
    Serial.printf("\n");
    #endif

    if(traffic_cache.find((const char*)data, data_len) != true)
    {
        bool a = node->sendData(data, data_len);
        #ifdef DEBUG_MODE
        if(a)
            Serial.println("Sent successfuly");
        else
            Serial.println("Sent unsuccessfuly");
        #endif
        traffic_cache.push((const char*)data, data_len);
    }
    else
    {
        #ifdef DEBUG_MODE
        Serial.println("Message found in cache not sending");
        #endif
    }
}

MANAGER::MANAGER()
{
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
}

void MANAGER::init(uint8_t channel, bool encryption)
{
    node = new NODE((uint8_t)channel, (bool)encryption);

    #ifdef DEBUG_MODE
    Serial.printf("Node channel: %3d\n", node->get_channel());
    #endif

    node->register_recieve_cb(recieve_function);

    node->register_send_cb(send_function);
}

bool MANAGER::send_message(const uint8_t* message, uint8_t len)
{
    if(traffic_cache.find((const char*)message, len) != true)
    {
        traffic_cache.push((const char*)message, len);
        return node->sendData(message, len);
    }
    return false;
}