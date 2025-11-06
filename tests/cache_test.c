/**
 * @file cache_test.c / source code for cache object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include "../src/cache.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/message.h"
#include "helpers.h"

/**
 * @brief Main function for tests of multiprocess cipher programs to encrypt words.
 *
 * @param void - no parameters
 * @return int - value for if successful or not
 */
int main() {
    // TEST: create_cache();
    cache_t* cache = create_cache();
    printf("Cache Total Pages: %d\n", cache->total_pages);
    free_cache(cache);  // FIXME: free() is aborting here

    return 0;
}