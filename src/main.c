/**
 * @file main.c / source code for Practicum I.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cache.h"
#include "config.h"
#include "message.h"

/**
 * @brief Helper function to implement random access evaluation of LIFO and RANDOM
 *
 * @param cache cache_t* - either LIFO or RANDOM strategy set cache
 */
void cache_evaluate(cache_t* cache, int n) {
    if (cache == NULL) {
        printf("WARNING: trying to print contents of a cache that doesn't exist\n");
        return;
    }
    
    int next_id = get_next_id();

    // fill cache in initial phase
    message_t* msg = NULL;
    for (int i = 1; i <= cache->total_pages; i++) {
        msg = retrieve_msg(i, cache);
        free_message(msg);
    }

    // EVALUATION - choosing random index to be requested
    int random_index = -1;
    for (int i = 1; i <= n; i++) {
        if (next_id > -1) {
            random_index = rand() % (next_id-1) + 1;
        }

        // printf("%d\n", random_index);
        msg = retrieve_msg(random_index, cache);
        free_message(msg);
    }
    printf("Cache Hts: %d/%d\n", cache->hits, n);
    printf("Cache Miss: %d/%d\n", cache->miss, n);
}


int main(int argc, char* argv[]) {

    srand(time(NULL));  // Seed with current time
    
    cache_t* lifo_cache = create_cache(LIFO);
    cache_t* random_cache = create_cache(RANDOM);

    printf("EVALUATE LIFO REPLACEMENT\n");
    cache_evaluate(lifo_cache, 10000);

    printf("\nEVALUATE RANDOM REPLACEMENT\n");
    cache_evaluate(random_cache, 10000);

    free_cache(lifo_cache);
    free_cache(random_cache);
    
    
}