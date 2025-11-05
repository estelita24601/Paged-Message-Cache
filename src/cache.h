/**
 * @file cache.h / header file for Cache.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#ifndef CACHE_H
#define CACHE_H
#include "bst.h"
#include "config.h"
#include "message.h"

// todo: turn this into a union
typedef struct cache_page {
    // meta-data
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
    // cache meta-data
    int num_pages;
    int page_size_bytes;

    cache_page_t* page_array;
    int last_added;  // index for last page added to cache

} cache_t;

/**
 * @brief - given the max message size in bytes calculate how much memory to allocate for the different fields such as
 * sender, receiver, content, etc.
 *
 */
void calculate_memory_sizes();

/**
 * @brief Create a cache object
 *
 * @return cache_t*
 */
cache_t* create_cache();

/**
 * @brief free the cache object
 *
 * @param cache
 */
void free_cache(cache_t* cache);

#endif