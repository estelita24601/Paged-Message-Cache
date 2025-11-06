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
cache_t* create_cache() {
    cache_t* cache = (cache_t*) calloc(CACHE_SIZE, sizeof(cache_t*));
    if (cache == NULL) {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }

    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->page_array[i] = init_page();
    }
    cache->page_size_bytes = MAX_MESSAGE_SIZE;
    cache->total_pages = CACHE_SIZE;
    cache->pages_occupied = 0;
    cache->last_added = -1;

    return cache;
}

bool cache_add(cache_t* cache, message_t* msg, replacement_strategy strategy) {}

message_t* cache_find(cache_t* cache, int id) {}

void free_cache(cache_t* cache) {
    if (cache == NULL) {
        fprintf(stderr, "ERROR: cache is NULL");
        exit(1);
    }

    for(int i = 0; i < cache->total_pages; i++) {
        free(cache->page_array [i]);
    }
    
    free(cache);

}

cache_page_t* init_page() {
    cache_page_t* new_page = malloc(sizeof(cache_page_t));
    if (new_page == NULL) {
        fprintf(stderr, "ERROR: memory was not able to be allocated\n");
        exit(1);
    }
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

    // update page status
    page->occupied = true;

    // copy message values over
    page->id = msg->id;
    page->sent_time = msg->sentTime;
    page->sent_flag = msg->sentFlag;

    // copy strings over
    strncpy(page->sender, msg->sender, MAX_SENDER_SIZE - 1);
    strncpy(page->receiver, msg->receiver, MAX_RECEIVER_SIZE - 1);
    strncpy(page->content, msg->content, MAX_CONTENT_SIZE - 1);

    // make sure strings are null terminated in case msg strings were truncated
    page->sender[MAX_SENDER_SIZE - 1] = '\0';
    page->receiver[MAX_RECEIVER_SIZE - 1] = '\0';
    page->content[MAX_CONTENT_SIZE - 1] = '\0';

    return true;  // placeholder
}

bool clear_page(cache_page_t* page) {
    if (page == NULL) {
        printf("ERROR: trying to clear page that doesn't exist\n");
        return false;
    }
    // update page status
    page->occupied = false;

    // reset values and empty strings
    page->id = -1;
    page->sent_time = -1;
    page->sent_flag = false;
    page->sender[0] = '\0';
    page->receiver[0] = '\0';
    page->content[0] = '\0';
    return true;
}

message_t* create_msg_from_page(const cache_page_t* page) {}

void free_page(cache_page_t* page) {}
