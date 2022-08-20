#include "./MANAGER.h"

NODE* node;

push_heap_s traffic_cache;

int j = 0;

bool manager_send(const uint8_t* message, uint8_t len)
{
    if(traffic_cache.find((const char*)message, len) != true)
    {
        return node->sendData((const uint8_t*)message, len);
        traffic_cache.push((const char*)message, len);
    }
    return false;
}

void send_function(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    #ifdef DEBUG_MODE
    if(status == esp_now_send_status_t::ESP_NOW_SEND_FAIL)
    {
        Serial.println("Message failed to send");
    }
    if(status == esp_now_send_status_t::ESP_NOW_SEND_SUCCESS)
    {
        digitalWrite(DEBUG_PIN_1, HIGH);
        delay(500);
        Serial.println("Message sent successfuly");
    }
    digitalWrite(DEBUG_PIN_1, LOW);
    #endif
}

void recieve_function(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    #ifdef DEBUG_MODE
    Serial.printf("sender mac address: %3d:%3d:%3d:%3d:%3d:%3d\nData: ", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.printf((const char*)data);
    Serial.printf("\n");
    #endif

    if(traffic_cache.find((const char*)data, data_len) != true)
    {
        bool a = manager_send((const uint8_t*)data, data_len);
        #ifdef DEBUG_MODE
        if(a)
        {
            digitalWrite(DEBUG_PIN_2, HIGH);
            Serial.println("Message forwarded");
            j++;
        }
        else
            Serial.println("Sent unsuccessfuly");
        delay(500);
        digitalWrite(DEBUG_PIN_2, LOW);
        #endif

        traffic_cache.push((const char*)data, data_len);
    }
    #ifdef DEBUG_MODE
    else
    {
        digitalWrite(DEBUG_PIN_3, HIGH);
        Serial.println("Message found in cache not sending");
        delay(500);
        digitalWrite(DEBUG_PIN_3, LOW);
    }
    printf("Sent: %d\n", j);
    #endif
}

void manager_init(uint8_t channel, bool encryption)
{
    #ifdef DEBUG_MODE
    pinMode(DEBUG_PIN_1, OUTPUT);
    digitalWrite(DEBUG_PIN_1, LOW);
    pinMode(DEBUG_PIN_2, OUTPUT);
    digitalWrite(DEBUG_PIN_2, LOW);
    pinMode(DEBUG_PIN_3, OUTPUT);
    digitalWrite(DEBUG_PIN_3, LOW);
    #endif

    node = new NODE((uint8_t)channel, (bool)encryption);

    #ifdef DEBUG_MODE
    Serial.printf("Node channel: %3d\n", node->get_channel());
    #endif

    node->register_recieve_cb(recieve_function);

    node->register_send_cb(send_function);
}