#include "helpers.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_test_results(bool result, char* message) {
    if (result) {
        printf("SUCCESS: %s\n\n", message);
    } else {
        printf("FAILURE: %s\n\n", message);
    }
}

void PRINT_COMPARISON(const char* expected, const char* actual) {
    printf("\texpected: %s\n", expected);
    printf("\t  actual: %s\n", actual);
}

void PRINT_HEADER(char* str) {
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\nTEST: %s\n", str);
}

void PRINT_COMPARE_MESSAGES(message_t* expected_msg, message_t* actual_msg) {
    // todo: use msg_to_pretty_str
    char* exp_msg_print = message_to_pretty_str(expected_msg);
    char* actual_msg_print = message_to_pretty_str(expected_msg);
    printf("\texpected: %s\n", exp_msg_print);
    printf("\t  actual: %s\n", actual_msg_print);
    free(exp_msg_print);
    free(actual_msg_print);
}

void PRINT_SUBHEADER(char* str) { printf("\n-- %s\n", str); }