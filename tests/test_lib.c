#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PRINT_FAILURE() {
    printf("---- FAILED TEST\n");
}

void PRINT_COMPARISON(const char* expected, const char* actual) {
    printf("\texpected: %s\n", expected);
    printf("\t  actual: %s\n", actual);
}

void PRINT_HEADER(char* str) {
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\nTEST: %s\n", str);
}


void PRINT_COMPARE_MESSAGES(message_t* expected_msg, message_t* actual_msg){
    // todo: use msg_to_pretty_str
}


void PRINT_SUBHEADER(char* str) { printf("\n-- %s\n", str); }