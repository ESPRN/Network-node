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

#include <stdio.h>

struct stack_node_s
{
    // the stack node after the current one
    stack_node_s* next;
    // length of the message (included for no other reason then increasing the speed of the search)
    uint8_t len;
    // value of the stored message to the stack
    char value[250];
    // the stack node proceding the current one
    stack_node_s* prev;
};

struct push_stack_s
{
    // amount of stack_nodes allocated
    uint8_t count = 0;
    //first in stack
    stack_node_s* first = NULL;
    //last in stack
    stack_node_s* last = NULL;

    // push a new entry to the stack
    void push(const char* value, uint8_t len)
    {
        if(count == 100)
        {
            stack_node_s* snd_last = last->next;
            free(last);
            last = snd_last;
            count--;
        }

        if(first == NULL)
        {
            first = (stack_node_s*)malloc(sizeof(stack_node_s));
            first->next = NULL;
            first->prev = NULL;
            first->len = len;
            strncpy(first->value, value, len);
            last = first;
            count++;
            return;
        }

        stack_node_s* second = first;
        first = (stack_node_s*)malloc(sizeof(stack_node_s));
        first->next = NULL;
        first->prev = second;
        second->next = first;
        first->len = len;
        strncpy(first->value, value, len);
        count++;
    }

    // search the stack for a entry
    void search_stack(const char* value, uint8_t len)
    {
        
    }
};

#endif