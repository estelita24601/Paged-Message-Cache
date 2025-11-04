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
#include "message.h"

typedef struct cache_page {
    bool occupied;  // is this page of the cache occupied?

    int id;           // id of the message
    time_t sentTime;  // time the message was sent
    bool sentFlag;    // flag indicating if the message has been delivered
    char[] sender;    // sender of the message
    char[] receiver;  // receiver of the message
    char[] content;   // content of the message

} cache_page_t;

typedef struct cache {
    // cache meta-data
    int num_pages;
    int page_size_bytes;
    bst_t* avl_lookup;  // use binary search to search by message id #
    // todo: add a stack for LIFO

    // actual messages we're saving in the cache
    cache_page_t* pages;
} cache_t;

/**
 * @brief Create a cache object
 *
 * @param num_pages
 * @param page_size
 * @return msg_cache_t*
 */
msg_cache_t* create_cache(int num_pages, int page_size);

/**
 * @brief
 *
 * @param cache
 */
void free_cache(msg_cache_t* cache);

#endif