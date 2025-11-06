/**
 * @file message_test.c / source code for message object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include <stdio.h>

#include "../src/cache.h"
#include "../src/message.h"
#include "helpers.h"

int main() {
    bool actual;

    PRINT_HEADER("init empty page");
    cache_page_t* page1 = init_page();
    print_page(page1);

    PRINT_HEADER("load message into page");
    message_t* msg1 = create_msg_from_str("0,obi-wan,grievous,2025-10-31 20:40:55,0,hello there");
    actual = fill_page(page1, msg1);
    print_page(page1);
    PRINT_TEST_RESULTS(actual == true, "");

    PRINT_HEADER("clear message from the page");
    actual = clear_page(page1);
    print_page(page1);
    PRINT_TEST_RESULTS(actual == true, "");

    PRINT_HEADER("load message into page again");
    message_t* msg2 = create_msg_from_str("2,alexander,angelica,2025-10-31 21:40:55,0,my dearest, angelica");
    actual = fill_page(page1, msg2);
    print_page(page1);
    PRINT_TEST_RESULTS(actual == true, "");

    PRINT_HEADER("edge case of trying to fill already filled page");
    actual = fill_page(page1, msg1);
    PRINT_TEST_RESULTS(actual == false, "expected the function to prevent unauthorized overwrite of filled page");

    PRINT_HEADER("edge case of trying to fill page with NULL message");
    clear_page(page1);
    actual = fill_page(page1, NULL);
    PRINT_TEST_RESULTS(actual == false, "expected the function to prevent filling the page with NULL message object");

    PRINT_HEADER("edge case of trying to fill NULL page");
    actual = fill_page(NULL, msg1);
    PRINT_TEST_RESULTS(actual == false, "expected the function to catch if page is NULL");

    PRINT_HEADER("create message from page");
    fill_page(page1, msg1);
    message_t* actual_msg = create_msg_from_page(page1);
    PRINT_COMPARE_MESSAGES(msg1, actual_msg);

    PRINT_HEADER("edge case of try to create message from empty page");
    clear_page(page1);
    actual_msg = create_msg_from_page(page1);
    PRINT_TEST_RESULTS(actual_msg == NULL, "expected NULL return if page is empty");

    PRINT_HEADER("edge case of trying to create message from a NULL page");
    actual_msg = create_msg_from_page(NULL);
    PRINT_TEST_RESULTS(actual_msg == NULL, "expected NULL return if page is NULL");

    free_message(msg1);
    free_message(msg2);
    free(page1);
}