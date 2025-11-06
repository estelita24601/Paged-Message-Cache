 /**
 * @file cache_test.c / source code for cache object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../src/message.h"
#include "../src/cache.h"

/**
 * @brief prints if a test was a success or failure
 *
 * @param result bool - result of the test
 * @param message char * - describes the test that was run
 */
void print_test_results(bool result, char *message)
{
    if (result)
    {
        printf("SUCCESS: %s\n\n", message);
    }
    else
    {
        printf("FAILURE: %s\n\n", message);
    }
}


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
    free_cache(cache); // FIXME: free() is aborting here

    return 0;
}