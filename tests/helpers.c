#include "helpers.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PRINT_TEST_RESULTS(bool result, char* message) {
    if (result) {
        printf("\nPASS: %s\n", message);
    } else {
        printf("\nFAIL: %s\n", message);
    }
}

void PRINT_COMPARISON(const char* expected, const char* actual) {
    printf("\texpected: %s\n", expected);
    printf("\t  actual: %s\n", actual);
}

void PRINT_HEADER(char* str) {
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("TEST - %s\n", str);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void DISPLAY_MESSAGE(message_t* msg) {
    char* pretty_str = message_to_pretty_str(msg);
    printf("%s\n", pretty_str);
    free(pretty_str);
}

void PRINT_COMPARE_MESSAGES(message_t* expected_msg, message_t* actual_msg) {
    char* exp_msg_print = message_to_pretty_str(expected_msg);
    char* actual_msg_print = message_to_pretty_str(actual_msg);

    if (strcmp(exp_msg_print, actual_msg_print) != 0) {
        printf("FAIL:\n");
    } else {
        printf("PASS:\n");
    }

    printf("\texpected: %s\n", exp_msg_print);
    printf("\t  actual: %s\n", actual_msg_print);
    free(exp_msg_print);
    free(actual_msg_print);
}

void PRINT_SUBHEADER(char* str) { printf("\n~~~~ %s ~~~~\n", str); }