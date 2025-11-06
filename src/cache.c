/**
 * @file cache.c / source code for Cache.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include "cache.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Create a cache object on the heap
 *
 * @return cache_t*
 */
cache_t* create_cache(message_t* msg) {}

bool cache_add(cache_t* cache, message_t* msg, replacement_strategy strategy) {}

message_t* cache_find(cache_t* cache, int id) {}

void free_cache(cache_t* cache) {}

cache_page_t* init_page() {
    cache_page_t* new_page = malloc(sizeof(cache_page_t));
    new_page->occupied = false;

    return new_page;
}

bool fill_page(cache_page_t* page, const message_t* msg) {
    // make sure objects aren't null
    if (page == NULL) {
        printf("ERROR: trying to fill page that doesn't exist\n");
        return false;
    } else if (msg == NULL) {
        printf("ERROR: trying to fill page with a NULL message object\n");
    }
    // make sure strings inside of message aren't null
    if (msg->sender == NULL || msg->receiver == NULL || msg->content == NULL) {
        printf("ERROR: trying to fill page with a message object that contains NULL string fields");
        return false;
    }

    // todo: if time check strlen of sender, receiver and content isn't too big

    page->id = msg->id;
    page->sent_time = msg->sentTime;
    page->sent_flag = msg->sentFlag;

    return true;  // placeholder
}

bool clear_page(cache_page_t* page) {}

message_t* create_msg_from_page(const cache_page_t* page) {}

void free_page(cache_page_t* page) {}
