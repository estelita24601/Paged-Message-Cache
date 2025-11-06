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
    PRINT_HEADER("create cache");
    cache_t* cache = create_cache();
    print_cache_metadata(cache);
    print_cache_contents(cache);

    PRINT_HEADER("free cache");
    free_cache(cache);

    return 0;
}