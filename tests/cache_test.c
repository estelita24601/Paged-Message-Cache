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
    cache_t* lifo_cache = create_cache(LIFO);
    cache_t* random_cache = create_cache(RANDOM);
    print_cache_contents(lifo_cache);

    // create objects and values for testing
    message_t* msg0 = disk_find(20);
    message_t* msg1 = disk_find(1);
    message_t* msg2 = disk_find(2);
    message_t* msg3 = disk_find(3);
    message_t* msg4 = disk_find(4);
    message_t* msg5 = disk_find(5);
    message_t* msg6 = disk_find(6);
    message_t* msg7 = disk_find(7);
    message_t* msg8 = disk_find(8);
    message_t* msg9 = disk_find(9);
    message_t* msg10 = disk_find(10);

    PRINT_HEADER("add 1st message to the LIFO cache");
    bool status = cache_add(lifo_cache, msg0);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 2nd message to the LIFO cache");
    status = cache_add(lifo_cache, msg1);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 3rd message to the LIFO cache");
    status = cache_add(lifo_cache, msg2);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    // TEST - out-of-memory detection RANDOM replacement strategy
    PRINT_HEADER("add 4th message to the LIFO cache");
    status = cache_add(lifo_cache, msg3);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 5th message to the LIFO cache should trigger a LIFO replacement");
    status = cache_add(lifo_cache, msg4);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 6th message to the LIFO cache should trigger a LIFO replacement");
    status = cache_add(lifo_cache, msg6);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("add 7th message to the LIFO cache should trigger a LIFO replacement");
    status = cache_add(lifo_cache, msg7);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(lifo_cache);
    print_cache_contents(lifo_cache);

    PRINT_HEADER("try to find message in the cache");
    cache_page_t* page = cache_find(lifo_cache, 2);
    message_t* actual_msg = create_msg_from_page(page);
    PRINT_COMPARE_MESSAGES(msg2, actual_msg);
    free_message(actual_msg);

    PRINT_HEADER("try to find message that isn't in the cache");
    page = cache_find(lifo_cache, 5);
    actual_msg = create_msg_from_page(page);
    PRINT_TEST_RESULTS(actual_msg == NULL, "expected false return if message is NULL");
    free_message(actual_msg);

    // TEST - (edge case) add a NULL msg and invalid id to cache_find(..)
    PRINT_HEADER("try to find message in a cache that doesn't exist");
    page = cache_find(NULL, 2);
    PRINT_TEST_RESULTS(page == NULL, "expected false return if cache is NULL");

    PRINT_HEADER("try to find message with an invalid id");
    page = cache_find(random_cache, -5);
    PRINT_TEST_RESULTS(page == NULL, "expected false return if id is invalid");
    

    srand(time(NULL)); // Seed with current time - need to place outside of cache.c to fully randomize
    // TEST - fill RANDOM cache in initial fill phase
    PRINT_HEADER("add 1st message to the RANDOM cache");
    status = cache_add(random_cache, msg1);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 2nd message to the RANDOM cache");
    status = cache_add(random_cache, msg2);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 3rd message to the RANDOM cache");
    status = cache_add(random_cache, msg3);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 4th message to the RANDOM cache");
    status = cache_add(random_cache, msg4);
    PRINT_TEST_RESULTS(status == true, "");
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

    PRINT_HEADER("add 7th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg7);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 8th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg8);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 9th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg9);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    PRINT_HEADER("add 10th message to the RANDOM cache should trigger a RANDOM replacement");
    status = cache_add(random_cache, msg10);
    PRINT_TEST_RESULTS(status == true, "");
    print_cache_metadata(random_cache);
    print_cache_contents(random_cache);

    // TEST - (edge case) add a NULL message to the cache
    PRINT_HEADER("add empty message to the RANDOM cache");
    status = cache_add(random_cache, NULL);
    PRINT_TEST_RESULTS(status == false, "expected false return if message is NULL");

    // TEST - (edge case) add a msg1 to a NULL cache
    PRINT_HEADER("add 1st message to a non-exist cache");
    status = cache_add(NULL, msg1);
    PRINT_TEST_RESULTS(status == false, "expected false return if cache is NULL");

    // PRINT_HEADER("store_msg"); // TODO
    // TEST - (edge case) store null msg to disk and LIFO cache
    PRINT_HEADER("store a NULL message to disk and cache");
    status = store_msg(NULL, lifo_cache);
    PRINT_TEST_RESULTS(status == false, "expected false return if mesage is NULL");

    // TEST - (edge case) store msg to NULL cache
    PRINT_HEADER("store 1st message to disk and NULL cache");
    status = store_msg(msg1, NULL);
    PRINT_TEST_RESULTS(status == false, "expected false return if cache is NULL");
    
    // TEST: test store message and next_id - commented out since don't want to always create a new message with the
    /*
    PRINT_HEADER("store new message to disk and LIFO cache and compare with retrieved");
    message_t* msg = create_msg("Jasmine", "Peter", "Thanksgiving is around the corner, we have a special holiday rate for our Alaskan cruise! Call us at 888-888-8888.");
    status = store_msg(msg, lifo_cache);
    PRINT_TEST_RESULTS(status == true, "");
    actual_msg = retrieve_msg(get_next_id()-1, lifo_cache);
    PRINT_COMPARE_MESSAGES(msg, actual_msg);
    free_message(msg);
    free_message(actual_msg);
    */

    // PRINT_HEADER("retrieve_msg"); // TODO
    // TEST - retrieve msg from disk and cache
    PRINT_HEADER("retrieve 2nd message to disk and LIFO cache");
    actual_msg = retrieve_msg(2, lifo_cache);
    PRINT_COMPARE_MESSAGES(msg2, actual_msg);
    free_message(actual_msg);

    // TEST - (edge case) retrieve msg from disk and NULL cache
    PRINT_HEADER("retrieve invalid message id to disk and LIFO cache");
    actual_msg = retrieve_msg(-2, lifo_cache);
    PRINT_TEST_RESULTS(actual_msg == NULL, "expected NULL return if id is invalid");
    free_message(actual_msg);


    // TEST - (edge case) retrieve msg from disk and NULL cache TODO - need to catch an EXIT status or scrap this test
    /*
    PRINT_HEADER("store 2nd message to disk and NULL cache");
    actual_msg = retrieve_msg(2, NULL);
    PRINT_TEST_RESULTS(actual_msg == NULL, "");
    free_message(actual_msg);
    */

    // TEST - out-of-disk detection

    // TEST - other
    PRINT_HEADER("check what happens when printing a NULL cache");
    printf("\n");
    print_cache_metadata(NULL);
    printf("\n");
    print_cache_contents(NULL);
    printf("\n");


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