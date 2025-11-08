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
    time_t sent_time;                  // time the message was sent
    bool sent_flag;                    // flag indicating if the message has been delivered
    char sender[MAX_SENDER_SIZE];      // sender of the message
    char receiver[MAX_RECEIVER_SIZE];  // receiver of the message
    char content[MAX_CONTENT_SIZE];    // content of the message
} cache_page_t;

typedef struct cache {
    cache_page_t* page_array[CACHE_SIZE];  // array of cache pages

    // cache meta-data
    replacement_strategy strategy;
    int page_size_bytes;  // max size of a page that stores a message
    int total_pages;      // max messages this cache can hold
    int pages_occupied;   // how many messages currently in the cache
    int last_added;       // index of last page added to cache
    int total_accesses;   // total number of requests sent to cache
    int hits;             // number of messages found in cache
    int miss;             // number of messages not found in cache
} cache_t;

///////////////////////////////////////////////////////////////////////

/**
 * @brief Create a cache object on the heap where its members are contiguous blocks of memory.
 *
 * @return cache_t* - return the initialized cache object
 */
cache_t* create_cache(replacement_strategy strategy);

/**
 * @brief - Add a message to the cache. If the cache is full then use the replacement strategy specified.
 *
 * @param cache cache_t* - the pointer to the cache object to add a message to
 * @param msg message_t* - the pointer to the message object to add to the cache
 * @return bool - return true if a message has been successfully added into the cache otherwise false
 */
bool cache_add(cache_t* cache, message_t* msg);

/**
 * @brief - use linear search to find a message in the cache
 *
 * @param cache cach_t* - cache we're searching
 * @param id int - id of the message we want to find
 * @return cache_page_t* - found cache page or NULL if not found
 */
cache_page_t* cache_find(cache_t* cache, int id);

/**
 * @brief free the page_array inside the cache and the cache object itself.
 *
 * @param cache cache_t* - the pointer to the cache object
 */
void free_cache(cache_t* cache);

/**
 * @brief prints the cache object metadata.
 *
 * @param cache cache_t* - the pointer to the cache object
 */
void print_cache_metadata(cache_t* cache);

/**
 * @brief prints the cache object's member values
 *
 * @param cache cache_t* - the pointer to the cache object
 */
void print_cache_contents(cache_t* cache);

/**
 * @brief - initialize empty cache page on the heap
 *
 * @return cache_page_t*
 */
cache_page_t* init_page();

/**
 * @brief - sets the initialized cache page with the provided message
 *
 * @param page cache_page_t* - the pointer to the cache page object
 * @param msg message_t* - the pointer to the message object
 *
 * @return bool - true if page is set otherwise false
 */
bool set_page(cache_page_t* page, const message_t* msg);

/**
 * @brief - clears the cache page to prepare to set a new message input
 *
 * @param page cache_page_t* - the pointer to the cache page object
 *
 * @return bool - true if page is cleared otherwise false
 */
bool clear_page(cache_page_t* page);

/**
 *
 * @param page
 * @param msg
 * @return
 */
bool replace_page(cache_page_t* page, const message_t* msg);

/**
 * @brief - Creates a new message object with the provided cache page.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 * @return message_t*
 */
message_t* create_msg_from_page(const cache_page_t* page);

/**
 * @brief - Prints the members of the input cache page.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 */
void print_page(cache_page_t* page);

/**
 * @brief - Prints the cache page metadata.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 */
void print_page_metadata(cache_page_t* page);

/**
 * @brief - Frees the memory allocated for a cache page.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 */
void free_page(cache_page_t* page);

#endif