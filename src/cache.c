 /**
 * @file cache.c / source code for Cache.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include "cache.h"

/**
 * @brief Create a cache object on the heap
 *
 * @return cache_t*
 */
cache_t* create_cache() {

    return NULL;

}

 bool cache_add(cache_t* cache, message_t* msg, replacement_strategy strategy) {}

 bool cache_clear_page_(cache_t* cache, int i) {}

 message_t* cache_find(cache_t* cache, int id) {}

 void free_cache(cache_t* cache) {}

 cache_page_t* init_page() {}

 bool fill_page(const message_t* msg) {}

 message_t* create_msg_from_page(const cache_page_t* page) {}

 void free_page(cache_page_t* page) {}
