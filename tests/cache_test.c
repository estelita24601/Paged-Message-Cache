/**
 * @file cache_test.c / source code for cache object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

#include "../src/disk.h"
#include "../src/message.h"
#include "../src/cache.h"

/**
 * @brief Main function for tests of multiprocess cipher programs to encrypt words.
 *
 * @param void - no parameters
 * @return int - value for if successful or not
 */
int main() {

    PRINT_HEADER("create cache");
    cache_t* null_cache = NULL;
    cache_t* lifo_cache = create_cache(LIFO);
    cache_t* random_cache = create_cache(RANDOM);
    print_cache_contents(lifo_cache);

    // create objects and values for testing
    message_t* null_msg = NULL;
    message_t* msg0 = disk_find(0);
    message_t* msg1 = disk_find(1);
    message_t* msg2 = disk_find(2);
    message_t* msg3 = disk_find(3);
    message_t* msg4 = disk_find(4);
    message_t* msg5 = disk_find(5);
    message_t* msg6 = disk_find(6);

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
    cache_page_t* page = cache_find(lifo_cache, 2);
    message_t* actual_msg = create_msg_from_page(page);
    PRINT_COMPARE_MESSAGES(msg2, actual_msg);

    
    PRINT_HEADER("print page metadata");
    print_page_metadata(page);
    PRINT_HEADER("print page contents");
    print_page(page);

    free_message(actual_msg);

    PRINT_HEADER("try to find message that isn't in the cache");
    page = cache_find(lifo_cache, 5);
    actual_msg = create_msg_from_page(page);
    PRINT_TEST_RESULTS(actual_msg == NULL, "");

    // TEST - (edge case) add a NULL msg and invalid id to cache_find(..)
    PRINT_HEADER("try to find message in a cache that doesn't exist");
    page = cache_find(null_cache, 2);
    PRINT_TEST_RESULTS(page == NULL, "");

    PRINT_HEADER("try to find message with an invalid id");
    page = cache_find(random_cache, -5);
    PRINT_TEST_RESULTS(page == NULL, "");
    

    srand(time(NULL)); // Seed with current time - need to place outside of cache.c to fully randomize
    // TEST - fill RANDOM cache in initial fill phase
    PRINT_HEADER("add 1st message to the RANDOM cache");
    status = cache_add(random_cache, msg1);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 2nd message to the RANDOM cache");
    status = cache_add(random_cache, msg2);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 3rd message to the RANDOM cache");
    status = cache_add(random_cache, msg3);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 4th message to the RANDOM cache");
    status = cache_add(random_cache, msg4);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    // TEST - out-of-memory detection RANDOM replacement strategy
    PRINT_HEADER("add 5th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg5);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 6th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg6);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    // TEST - (edge case) add a NULL message to the cache
    PRINT_HEADER("add 6th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, null_msg);
    PRINT_TEST_RESULTS(status == false, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    // TEST - (edge case) add a NULL cache to the cache
    PRINT_HEADER("add 1st message to a non-exist cache");
    print_page(page);


    // TEST - out-of-disk detection

    // TEST - other
    PRINT_HEADER("check what happens when printing a NULL cache");
    printf("\n");
    print_cache_metadata(null_cache);
    printf("\n");
    print_cache_contents(null_cache);
    printf("\n");

    PRINT_HEADER("check what happens when printing a NULL page");
    cache_page_t* null_page = NULL;
    printf("\n");
    print_page_metadata(null_page);
    printf("\n");
    print_page(null_page);
    printf("\n");


    free_message(actual_msg);
    free_message(msg0);
    free_message(msg1);
    free_message(msg2);
    free_message(msg3);
    free_message(msg4);
    free_message(msg5);
    free_message(msg6);
    free_cache(lifo_cache);
    free_cache(random_cache);

    return 0;
}