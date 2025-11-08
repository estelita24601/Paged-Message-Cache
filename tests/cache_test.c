/**
 * @file cache_test.c / source code for cache object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include "../src/cache.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/disk.h"
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
    cache_t* lifo_cache = create_cache(LIFO);
    cache_t* random_cache = create_cache(RANDOM);
    print_cache_contents(lifo_cache);

    // create objects and values for testing
    message_t* msg0 = disk_find(0);
    message_t* msg1 = disk_find(1);
    message_t* msg2 = disk_find(2);
    message_t* msg3 = disk_find(3);
    message_t* msg4 = disk_find(4);

    replacement_strategy lifo = LIFO;
    replacement_strategy random = RANDOM;

    PRINT_HEADER("add 1st message to the cache");
    bool status = cache_add(lifo_cache, msg0);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 2nd message to the cache");
    status = cache_add(lifo_cache, msg1);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 3rd message to the cache should trigger a LIFO replacement");
    status = cache_add(lifo_cache, msg2);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("try to find message in the cache");
    message_t* actual_msg = create_msg_from_page(cache_find(lifo_cache, 2));
    PRINT_COMPARE_MESSAGES(msg2, actual_msg);

    PRINT_HEADER("try to find message that isn't in the cache");
    actual_msg = create_msg_from_page(cache_find(lifo_cache, 5));
    PRINT_TEST_RESULTS(actual_msg == NULL, "");

    // TEST - out-of-memory detection
    PRINT_HEADER("add 4th message to the cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg2);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 4th message to the cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg4);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    // TEST - out-of-disk detection

    // TEST - other
    PRINT_HEADER("check what happens when printing a NULL cache");
    cache_t* cache1 = NULL;
    printf("\n");
    print_cache_contents(cache1);
    printf("\n");

    free_message(msg0);
    free_message(msg1);
    free_message(msg2);
    free_message(msg3);
    free_message(msg4);
    free_cache(lifo_cache);
    free_cache(random_cache);

    return 0;
}