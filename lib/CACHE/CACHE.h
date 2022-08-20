/*
    @ Name: ESP-NOW network node: message caching code
    @ Author: Tevž Beškovnik
    @ Date: 12. 2. 2022
    @ Description: Meshed network of ESP32 devices over the connectionless ESP-NOW network, this header file contains the code
    is for the trafic caching sistem to prevent messages being infinatly brodcasted
    @ Notes:
        - Caches the used to cache the incoming trafic from other surounding nodes,
        - this is needed becouse if not checked messages will get infinatly brodcasted to the
          network becouse of the nature of how it works,
        - this file should get revieved by someone else then me, becouse I have no clue if it could couse memory problems :shrug:
*/

#ifndef _CACHE_
#define _CACHE_

// the cahcke stack limit defines how many messages should be cached at once before the start geting deleted default is 100
#ifndef CACHE_STACK_LIMIT
    #define CACHE_STACK_LIMIT 100
#endif

#ifdef DEBUG_MODE
#include <Arduino.h>
#endif

#include <stdlib.h>
#include <string.h>

struct node_s
{
    // the stack node after the current one
    struct node_s* next;
    // length of the message (included for no other reason then increasing the speed of the search)
    uint8_t len;
    // value of the stored message to the stack
    char value[250];
    // the stack node proceding the current one
    struct node_s* prev;
};

struct push_heap_s
{
    // amount of stack_nodes allocated
    uint8_t count;
    //first in stack
    node_s* first;
    //last in stack
    node_s* last;

    push_heap_s()
        : count(0), first(NULL), last(NULL)
    {
        ;
    }

    // search the heap for a entry
    bool find(const char* value, uint8_t len)
    {
        node_s* front = first;
        node_s* back = last;
        for(int i = 0; i < count/2; i++)
        {
            if(front->len != len && back->len != len)
            {
                front = front->prev;
                back = back->next;
                #ifdef DEBUG_MODE
                Serial.printf("CACHE: compared lengths do not mach up; value: %s, len: %d\n", value, len);
                #endif
                continue;
            }

            if(strcmp(value, front->value) == 0 || strcmp(value, back->value) == 0)
            {
                #ifdef DEBUG_MODE
                Serial.printf("CACHE: compared values match; value: %s, len: %d\n", value, len);
                #endif
                return true;
            }

            front = front->prev;
            back = back->next;
        }

        #ifdef DEBUG_MODE
        Serial.printf("CACHE: didnt find searched value in cache; value: %s, len: %d\n", value, len);
        #endif
        return false;
    }

    // push a new entry to the stack
    void push(const char* value, uint8_t len)
    {
        if(find(value, len) == true)
        {
            #ifdef DEBUG_MODE
            Serial.printf("CACHE: found value that was trying to be pushed\n");
            #endif
            return;
        }

        if(count == CACHE_STACK_LIMIT)
        {
            node_s* snd_last = last->next;
            snd_last->prev = NULL;
            last->next = NULL;
            free(last);
            last = snd_last;
            count--;
            #ifdef DEBUG_MODE
            Serial.printf("CACHE: entry limit reached; C: %d value: %s, len: %d\n", count, value, len);
            #endif
        }

        if(first == NULL)
        {
            first = (node_s*)malloc(sizeof(node_s));
            first->next = NULL;
            first->prev = NULL;
            first->len = len;
            strncpy(first->value, value, len);
            last = first;
            count++;
            #ifdef DEBUG_MODE
            Serial.printf("CACHE: first entry; C: %d, value: %s, len %d\n", count, value, len);
            #endif
            return;
        }
        
        node_s* second = first;
        first = (node_s*)malloc(sizeof(node_s));
        first->next = NULL;
        first->prev = second;
        second->next = first;
        first->len = len;
        strncpy(first->value, value, len);
        count++;
        #ifdef DEBUG_MODE
        Serial.printf("CACHE: entry added; C: %d value: %s, len: %d\n", count, value, len);
        #endif
    }

    ~push_heap_s()
    {
        node_s* current = last->next;
        last = NULL;
        first = NULL;
        while(current->next != NULL)
        {
            node_s* to_clear = current->prev;
            current->prev = NULL;
            free(to_clear);
            current = current->next;
        }
        free(current);
        count = 0;
    }
};

#endif