 /**
 * @file message_test.c / source code for message object tests.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../src/message.h"
#include "../src/cache.h"

/**
 * @brief prints if a test was a success or failure
 *
 * @param result bool - result of the test
 * @param message char * - describes the test that was run
 */
void print_test_results(bool result, char *message)
{
    if (result)
    {
        printf("SUCCESS: %s\n\n", message);
    }
    else
    {
        printf("FAILURE: %s\n\n", message);
    }
}


/**
 * @brief Main function for tests of multiprocess cipher programs to encrypt words.
 *
 * @param void - no parameters
 * @return int - value for if successful or not
 */
int main() {
    // TEST: test store message and next_id - commented out since don't want to always create a new message with the same values but incremented id
    /*
    char* s = "John Vargas";
    char* r = "Mary Jane";
    char* c = "Hi, is the budget report ready?";
    int id_check = 0;
    message_t* msg = create_msg(s, r, c);
    char* pretty_msg = message_to_pretty_str(msg);
    // get ID from the file
    FILE* id_file = fopen(NEXT_ID_PATH, "r");
    if (id_file) {
        fscanf(id_file, "%d", &id_check);
        fclose(id_file);
    } else {
        NEXT_ID = 0;  // initialize first id number
    }

    // check msg id against file next id - 1
    print_test_results(msg->id == id_check - 1, "store_msg() and get_next_id() functions");

    store_msg(msg);
    free_message(msg);
    free(pretty_msg);
    */

    // TEST: test retrieve_msg & message_to_pretty_str
    message_t* msg1 = retrieve_msg(1);
    char* pretty_msg1 = message_to_pretty_str(msg1);
    printf("Msg1 Check: %s", pretty_msg1);

    free_message(msg1);
    free(pretty_msg1);

    
    return 0;
}