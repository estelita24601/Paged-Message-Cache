#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"

int NUM_FAILURES = 0;

/**
 * @brief Prints a test failure message and increments the failure counter
 */
void PRINT_FAILURE() {
    printf("---- FAILED TEST\n");
    NUM_FAILURES++;
}

/**
 * @brief Prints a comparison between expected and actual test results
 *
 * @param expected The expected result string
 * @param actual The actual result string
 */
void PRINT_COMPARISON(const char* expected, const char* actual) {
    printf("\texpected: %s\n", expected);
    printf("\t  actual: %s\n", actual);
}

/**
 * @brief Prints a test section header with a given title
 *
 * @param str The title of the test section
 */
void PRINT_HEADER(char* str) {
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\nTEST: %s\n", str);
}

/**
 * @brief Prints a test subsection header
 *
 * @param str The title of the test subsection
 */
void PRINT_SUBHEADER(char* str) { printf("\n-- %s\n", str); }

/**
 * @brief Tests conversion of a message object to CSV string format
 *
 * @param msg_obj The message object to convert
 * @param expected The expected CSV string output
 */
void test_msg_to_csv(message_t* msg_obj, char* expected) {
    char* actual = msg_to_str(msg_obj);

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

    char* actual_string = msg_to_str(actual);
    char* expected_string = msg_to_str(expected);
    PRINT_COMPARISON(expected_string, actual_string);

    if (actual_string) free(actual_string);
    if (expected_string) free(expected_string);

    free_msg(actual);
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
    char* original_string = msg_to_str(original_msg);

    message_t* translated_msg = create_msg_from_str(original_string);

    if (msg_compare(original_msg, translated_msg) != true) {
        PRINT_FAILURE();
        char* translated_string = msg_to_str(translated_msg);
        PRINT_COMPARISON(original_string, translated_string);
        free(translated_string);
    }

    free(original_string);
    free_msg(translated_msg);
}

/**
 * @brief Tests message comparison functionality
 *
 * Compares two messages and verifies that the comparison result
 * matches the expected outcome. Prints detailed failure information
 * if the comparison doesn't match expectations.
 *
 * @param msgA First message to compare
 * @param msgB Second message to compare
 * @param expected The expected comparison result (true if messages should be equal)
 */
void test_msg_equals(message_t* msgA, message_t* msgB, bool expected) {
    bool actual = msg_compare(msgA, msgB);
    char* msgA_string = msg_to_str(msgA);
    char* msgB_string = msg_to_str(msgB);

    if (actual != expected) {
        PRINT_FAILURE();
        printf("\tA: %s\n", msgA_string);
        printf("\tvs\n");
        printf("\tB: %s\n", msgB_string);

        if (expected == true && actual == false) {
            PRINT_COMPARISON("A == B", "A != B");
        } else {
            PRINT_COMPARISON("A != B", "A == B");
        }
    }
    free(msgA_string);
    free(msgB_string);
}

/**
 * @brief Counts the number of lines in a file
 *
 * @param filepath Path to the file to count lines in
 * @return int Number of lines in the file, or -1 if file cannot be opened
 */
int count_lines(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        PRINT_FAILURE();
        printf("\tunable to open %s in read mode", filepath);
        return -1;
    }

    int count = 0;
    char* buffer = malloc(sizeof(char) * 500);
    while (fgets(buffer, 500, file) != NULL) {
        count++;
    }

    free(buffer);
    fclose(file);
    return count;
}

/**
 * @brief Tests message storage functionality
 *
 * @param msg Message to store
 * @param expect_success Whether the store operation should succeed
 * @param expect_new_entry Whether a new entry should be added (true) or existing entry updated (false)
 */
void test_store_msg(message_t* msg, bool expect_success, bool expect_new_entry) {
    int lines_before = count_lines(STORE_PATH);
    bool passes = store_msg(msg);
    int lines_after = count_lines(STORE_PATH);

    char* message_string = msg_to_str(msg);

    if (expect_success && passes == false) {
        PRINT_FAILURE();
        printf("\tunable to store message\n");
        printf("\t%s\n", message_string);
    } else if (!expect_success && passes == true) {
        PRINT_FAILURE();
        printf("\tmessage should not have been stored\n");
        printf("\t%s\n", message_string);
    } else if (lines_after < lines_before) {
        PRINT_FAILURE();
        printf("\tlost %d messages in the store\n", lines_before - lines_after);
        printf("\t%s\n", message_string);
    } else if (expect_new_entry) {
        // new entry should be appended to the end of the file
        // we expect lines_after == lines_before + 1
        if (lines_after == lines_before) {
            PRINT_FAILURE();
            printf("\tfailed to add message to the store\n");
            printf("\t%s\n", message_string);
        } else if (lines_after > lines_before + 1) {
            PRINT_FAILURE();
            printf("\tthere are %d extra messages in the store\n", lines_before - lines_after - 1);
            printf("\t%s\n", message_string);
        }
    } else if (!expect_new_entry) {
        // existing entry for the msg object should be updated
        // so # of lines_after should be the same
        if (lines_after != lines_before) {
            PRINT_FAILURE();
            printf("\tthere are %d extra messages in the store\n", lines_before - lines_after);
            printf("\t%s\n", message_string);
        }
    }

    free(message_string);
}

/**
 * @brief Creates a copy of the message store
 *
 * @param backup_name Path where the backup should be stored
 */
void backup_msg_store(char* backup_name) {
    char* system_call = malloc(sizeof(char) * 100);

    sprintf(system_call, "cp %s %s", STORE_PATH, backup_name);
    system(system_call);

    free(system_call);
}

/**
 * @brief Tests message retrieval by ID
 *
 * @param id ID of the message to retrieve
 * @param exists Whether the message should exist in the store
 */
void test_retrieve_msg(int id, bool exists) {
    message_t* msg = retrieve_msg(id);
    char* msg_string = msg_to_str(msg);

    if (!exists) {
        // msg should be null since id doesn't exist in the store
        if (msg != NULL) {
            PRINT_FAILURE();
            PRINT_COMPARISON(msg_string, "NULL");
        }
    } else {
        // we should be able to find message with id in the store
        char* expected = malloc(sizeof(char) * 100);
        sprintf(expected, "message with ID = %d", id);
        PRINT_COMPARISON(expected, msg_string);
        free(expected);
    }

    if (msg) {
        free_msg(msg);
    }
    if (msg_string) {
        free(msg_string);
    }
}

int main(int argc, char* argv[]) {
    // before we start reset everything
    FILE* id_counter = fopen("message_store/id_counter.txt", "w");
    fprintf(id_counter, "%d", 0);
    fclose(id_counter);
    FILE* message_csv = fopen("message_store/messages.csv", "w");
    fclose(message_csv);

    PRINT_HEADER("create_msg");
    PRINT_SUBHEADER("happy path");
    message_t* msg0 = create_msg("obi-wan", "grievous", "hello there");
    message_t* msg1 = create_msg("navi", "link", "hey listen!");

    PRINT_SUBHEADER("allow messages with empty or null content");
    message_t* msg2 = create_msg("link", "navi", "");
    message_t* msg3 = create_msg("link", "zelda", NULL);

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

    PRINT_HEADER("msg_compare");
    PRINT_SUBHEADER("compare completely different objects");
    test_msg_equals(msg0, msg1, false);
    PRINT_SUBHEADER("compare exact same object");
    test_msg_equals(msg0, msg0, true);

    // should be similar enough to be considered "equal"
    PRINT_SUBHEADER("compare separate objects with different ID and timestamp");
    message_t* msg1_cpy = create_msg("obi-wan", "grievous", "hello there");
    test_msg_equals(msg0, msg1_cpy, true);

    // now make sure theyre different enough to be unequal
    PRINT_SUBHEADER("compare separate objects with different delivery status");
    msg1_cpy->delivered_flag = 1;
    msg0->delivered_flag = 0;
    test_msg_equals(msg0, msg1_cpy, false);

    // should just return false instead of crashing
    PRINT_SUBHEADER("comparison with NULL objects");
    test_msg_equals(msg0, NULL, false);
    test_msg_equals(NULL, msg1, false);
    test_msg_equals(NULL, NULL, false);

    PRINT_HEADER("msg_to_str");
    // NEW EXPECTED FORMAT: id,sender,receiver,time_sent,delivered_flag,content
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
    char* test_case = "3,,receiver,2025-10-26 19:20:49,0,content";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty receiver
    test_case = "4,sender,,2025-10-26 19:20:49,0,content";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty sender and receiver
    test_case = "5,,,2025-10-26 19:20:49,0,";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty everything except id number and delivered flag
    test_case = "6,,,,0,";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    // empty id number
    test_case = ",sender,receiver,2025-10-26 19:20:49,0,content";
    PRINT_SUBHEADER(test_case);
    test_csv_to_msg(test_case, NULL);

    PRINT_HEADER("store_msg");

    PRINT_SUBHEADER("happy path: store message for the first time");
    test_store_msg(msg0, true, true);
    test_store_msg(msg1, true, true);
    test_store_msg(msg2, true, true);
    test_store_msg(msg3, true, true);
    test_store_msg(msg4, true, true);
    backup_msg_store("message_store/history/1_after_happy_path.csv");

    PRINT_SUBHEADER("try to store NULL object");
    test_store_msg(NULL, false, false);
    backup_msg_store("message_store/history/2_after_null_object.csv");

    PRINT_SUBHEADER("try to store duplicate message");
    test_store_msg(msg0, true, false);
    backup_msg_store("message_store/history/3_after_duplicate_object.csv");

    PRINT_SUBHEADER("update existing message");
    free(msg1->content);
    msg1->content = strdup("HEY!! LISTEN!!");
    test_store_msg(msg1, true, false);
    backup_msg_store("message_store/history/4_after_update_object.csv");

    PRINT_HEADER("retrieve_msg");

    PRINT_SUBHEADER("happy path");
    test_retrieve_msg(0, true);
    test_retrieve_msg(1, true);
    test_retrieve_msg(2, true);
    test_retrieve_msg(3, true);

    PRINT_SUBHEADER("message doesn't exist");
    test_retrieve_msg(100000, false);

    printf("\n\nTOTAL TESTS FAILED = %d\n", NUM_FAILURES);

    free_msg(msg0);
    free_msg(msg1);
    free_msg(msg1_cpy);
    free_msg(msg2);
    free_msg(msg3);
    free_msg(msg4);
}