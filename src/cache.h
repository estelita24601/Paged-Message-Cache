/**
 * @file cache.h
 * @authors Estelita Chen & Lori Kim
 * @brief
 * @date 2025-10-31
 *
 */
#ifndef CACHE_H
#define CACHE_H

#include "message.h"

typedef struct message_cache {
    int num_pages;
    int page_size_bytes;

    // fixme: what data type should this be?
    void* cache_data;
} msg_cache_t;

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