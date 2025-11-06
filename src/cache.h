/**
 * @file cache.h / header file for Cache.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

#include "config.h"
#include "message.h"

typedef enum { LIFO, RANDOM } replacement_strategy;

typedef struct cache_page {
    bool occupied;  // flag for if this page is occupied

    // actual data for the message stored in this page
    int id;                            // id of the message
    time_t sentTime;                   // time the message was sent
    bool sentFlag;                     // flag indicating if the message has been delivered
    char sender[MAX_SENDER_SIZE];      // sender of the message
    char receiver[MAX_RECEIVER_SIZE];  // receiver of the message
    char content[MAX_CONTENT_SIZE];    // content of the message
} cache_page_t;

typedef struct cache {
    cache_page_t** page_array;  // array of cache pages

    // cache meta-data
    int page_size_bytes;  // max size of a page that stores a message
    int total_pages;      // max messages this cache can hold
    int pages_occupied;   // how many messages currently in the cache
    int last_added;       // index of last page added to cache
} cache_t;

///////////////////////////////////////////////////////////////////////

/**
 * @brief Create a cache object on the heap
 *
 * @return cache_t*
 */
cache_t* create_cache();

/**
 * @brief - add a message to the cache. if the cache is full then use the replacement strategy specified
 *
 * @param cache
 * @param msg
 * @param strategy
 * @return true
 * @return false
 */
bool cache_add(cache_t* cache, message_t* msg, replacement_strategy strategy);

/**
 * @brief - helper function for when cache_add needs to replace a page
 *
 * @param cache cache_t* -
 * @param i int - index of the page to clear out
 * @return true
 * @return false
 */
bool cache_clear_page_(cache_t* cache, int i);

/**
 * @brief - use linear search to find a message in the cache
 *
 * @param cache cach_t* - cache we're searching
 * @param id int - id of the message we want to find
 * @return message_t* - found message or NULL if not found
 */
message_t* cache_find(cache_t* cache, int id);

/**
 * @brief free the cache object
 *
 * @param cache
 */
void free_cache(cache_t* cache);

/**
 * @brief - initialize empty cache page on the heap
 *
 * @return cache_page_t*
 */
cache_page_t* init_page();

/**
 * @brief put data for the message into the page
 *
 * @param msg
 * @return true
 * @return false
 */
bool fill_page(const message_t* msg);

/**
 * @brief Create a msg from page object
 *
 * @param page
 * @return message_t*
 */
message_t* create_msg_from_page(const cache_page_t* page);

/**
 * @brief
 *
 * @param page
 */
void free_page(cache_page_t* page);

#endif