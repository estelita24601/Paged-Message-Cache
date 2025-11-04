/*
 * tests.c / main source code of tests for Messages.
 *
 * Lori Kim / CS5600 / Northeastern University
 * 
 * Fall 2025 / Sep 28, 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../src/messages.h"

/**
 * @brief prints the message when information is stored
 *
 */
void print_msg_stored(message_t* msg, int inputNum) {
    printf("Message %d created: ID=%d, Sender=%s, Receiver=%s, Content=%s, SentFlag=%d\n",
           inputNum, msg->id, msg->sender, msg->receiver, msg->content, msg->sentFlag);
}

/**
 * @brief prints the message when information is found
 *
 */
void print_msg_found(message_t* msg) {
    printf("Message found: ID=%d, Sender=%s, Receiver=%s, Content=%s, SentFlag=%d\n",
           msg->id, msg->sender, msg->receiver, msg->content, msg->sentFlag);
}

/**
 * @brief Main function for tests of multiprocess cipher programs to encrypt words.
 *
 * @param void - no parameters
 * @return int - value for if successful or not
 */
int main() {
    // TEST: test create_msg_from_parts & create_msg_from_str
    message_t* msg1 = retrieve_msg(1);
    char* prettyMsg1 = message_to_pretty_str(msg1);
    printf("Msg1 Check: %s", prettyMsg1);

    free_message(msg1);
    free(prettyMsg1);

    return 0;
}