/**
 * @file message_test.c / source code for message object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include "../src/message.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/cache.h"
#include "helpers.h"

bool msg_compare(message_t* msgA, message_t* msgB) {
    // first do null checks
    if (msgA == NULL || msgB == NULL) {
        return false;
    }

    if (msgA->id != msgB->id) {
        return false;
    }

    return true;
}

/**
 * @brief Tests conversion of a message object to CSV string format
 *
 * @param msg_obj The message object to convert
 * @param expected The expected CSV string output
 */
void test_msg_to_csv(message_t* msg_obj, char* expected) {
    char* actual = msg_to_csv(msg_obj);

    if (expected == NULL) {
        if (actual != NULL) {
            PRINT_FAILURE();
        }
    }
    PRINT_COMPARISON(expected, actual);
    free(actual);
}

/**
 * @brief Tests creation of a message object from a CSV string
 *
 * @param csv_str The CSV string to parse
 * @param expected The expected message object (NULL for invalid inputs)
 */
void test_csv_to_msg(char* csv_str, message_t* expected) {
    message_t* actual = create_msg_from_str(csv_str);

    if (actual == NULL && expected != NULL) {
        PRINT_FAILURE();
    }

    char* actual_string = msg_to_csv(actual);
    char* expected_string = msg_to_csv(expected);
    PRINT_COMPARISON(expected_string, actual_string);

    if (actual_string) free(actual_string);
    if (expected_string) free(expected_string);

    free_message(actual);
}

/**
 * @brief Tests roundtrip conversion: message -> CSV -> message
 *
 * Converts a message to CSV format and back, verifying that the
 * resulting message matches the original.
 *
 * @param original_msg The message to test roundtrip conversion
 */
void test_msg_csv_roundtrip(message_t* original_msg) {
    char* original_string = msg_to_csv(original_msg);
    message_t* translated_msg = create_msg_from_str(original_string);

    PRINT_COMPARE_MESSAGES(original_msg, translated_msg);

    free(original_string);
    free_message(translated_msg);
}

/**
 * @brief Main function for tests of multiprocess cipher programs to encrypt words.
 *
 * @param void - no parameters
 * @return int - value for if successful or not
 */
int main() {
    // save current value of _NEXT_ID so we can put it back to normal after we're done
    int original_next_id = get_next_id();

    PRINT_HEADER("create_msg");
    PRINT_SUBHEADER("happy path");
    message_t* msg0 = create_msg("obi-wan", "grievous", "hello there");
    DISPLAY_MESSAGE(msg0);
    message_t* msg1 = create_msg("navi", "link", "hey listen!");
    DISPLAY_MESSAGE(msg1);

    PRINT_SUBHEADER("allow messages with empty or null content");
    message_t* msg2 = create_msg("link", "navi", "");
    DISPLAY_MESSAGE(msg2);
    message_t* msg3 = create_msg("link", "zelda", NULL);
    DISPLAY_MESSAGE(msg3);

    // instead of program crash just send back NULL object
    PRINT_SUBHEADER("don't allow messages with empty or null sender");
    message_t* msg_empty_sender = create_msg("", "recipient2", "content2");
    assert(msg_empty_sender == NULL);
    message_t* msg_null_sender = create_msg(NULL, "recipient2", "content2");
    assert(msg_null_sender == NULL);

    PRINT_SUBHEADER("don't allow messages with empty or null receiver");
    message_t* msg_empty_receiver = create_msg("sender 3", "", "content3");
    assert(msg_empty_receiver == NULL);
    message_t* msg_null_receiver = create_msg("sender3", NULL, "content3");
    assert(msg_null_receiver == NULL);

    PRINT_SUBHEADER("don't allow messages with empty or null sender AND receiver");
    message_t* msg_all_empty = create_msg("", "", "hello");
    assert(msg_all_empty == NULL);
    message_t* msg_all_null = create_msg(NULL, NULL, "hello");
    assert(msg_all_null == NULL);

    // tests message by checking its csv format
    PRINT_HEADER("msg_to_csv");
    // NEW EXPECTED FORMAT: id,sender,receiver,time_sent,sent_flag,content
    // TIME_FORMAT = "%Y-%m-%d %H:%M:%S"
    // "YYYY-mm-dd HH:MM:SS"

    PRINT_SUBHEADER("happy path test I");
    test_msg_to_csv(msg0, "0,obi-wan,grievous,YYYY-mm-dd HH:MM:SS,0,hello there");

    PRINT_SUBHEADER("happy path test II");
    test_msg_to_csv(msg1, "1,navi,link,YYYY-mm-dd HH:MM:SS,0,hey listen!");

    PRINT_SUBHEADER("message with no content I");
    test_msg_to_csv(msg2, "2,link,navi,YYYY-mm-dd HH:MM:SS,0,N/A");

    PRINT_SUBHEADER("message with no content II");
    test_msg_to_csv(msg3, "3,link,zelda,YYYY-mm-dd HH:MM:SS,0,N/A");

    PRINT_SUBHEADER("NULL message object");
    test_msg_to_csv(NULL, NULL);

    PRINT_HEADER("create_msg_from_str");
    PRINT_SUBHEADER("happy path I");
    test_msg_csv_roundtrip(msg0);

    PRINT_SUBHEADER("happy path II");
    test_msg_csv_roundtrip(msg1);

    PRINT_SUBHEADER("no content");
    test_msg_csv_roundtrip(msg2);

    PRINT_SUBHEADER("commas in the message content");
    message_t* msg4 = create_msg("alexander", "angelica", "my dearest, angelica");
    test_msg_csv_roundtrip(msg4);

    // empty sender
    char* test_case = "csv with no sender: '3,,receiver,2025-10-26 19:20:49,0,content'";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty receiver
    test_case = "csv with no receiver: '4,sender,,2025-10-26 19:20:49,0,content'";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty sender and receiver
    test_case = "csv with no sender, receiver or content: '5,,,2025-10-26 19:20:49,0,'";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty everything except id number and delivered flag
    test_case = "csv with most fields empty: '6,,,,0,'";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty id number
    test_case = "csv with no id number: ',sender,receiver,2025-10-26 19:20:49,0,content'";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    free_message(msg4);
    free_message(msg3);
    free_message(msg2);
    free_message(msg1);
    free_message(msg0);

    // reset next_id since none of these messages were stored to disk
    FILE* id_file = fopen(NEXT_ID_PATH, "w");
    if (id_file) {
        fprintf(id_file, "%d", original_next_id);
        fclose(id_file);
    } else {
        printf("WARNING: unable to reset next_id\n");
    }

    return 0;
}