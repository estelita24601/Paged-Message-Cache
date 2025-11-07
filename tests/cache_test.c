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
#include <math.h>

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

    // create objects and values for testing
    message_t* msg0 = retrieve_msg(0);
    message_t* msg1 = retrieve_msg(1);
    message_t* msg2 = retrieve_msg(2);
    message_t* msg3 = retrieve_msg(3);
    message_t* msg4 = retrieve_msg(4);

    replacement_strategy lifo = LIFO;
    replacement_strategy random = RANDOM;

    PRINT_HEADER("add 1st message to the cache");
    bool status = cache_add(cache, msg0, lifo);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(cache);
    print_cache_contents(cache);

    PRINT_HEADER("add 2nd message to the cache");
    status = cache_add(cache, msg1, lifo);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(cache);
    print_cache_contents(cache);

    PRINT_HEADER("add 3rd message to the cache should trigger a LIFO replacement");
    status = cache_add(cache, msg2, lifo);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(cache);
    print_cache_contents(cache);

    /*
    PRINT_HEADER("try to find message in the cache");
    status = cache_find(cache, 2);
    PRINT_TEST_RESULTS(status == true, "");

    PRINT_HEADER("try to find message that isn't in the cache");
    status = cache_find(cache, 5);
    PRINT_TEST_RESULTS(status == false, "");
    */

    PRINT_HEADER("add 4th message to the cache should trigger a RANDOM replacement");
    status = cache_add(cache, msg2, lifo);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(cache);
    print_cache_contents(cache);

    PRINT_HEADER("add 4th message to the cache should trigger a RANDOM replacement");
    status = cache_add(cache, msg4, random);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(cache);
    print_cache_contents(cache);

    
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
    free_cache(cache);
    
    return 0;
}