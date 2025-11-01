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


#include "messages.h"
#include "bst.h"

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
    char* filename = "messages.txt";
    int inputID = 0;

    // TEST - store message to empty file

    // TEST - id1 message creation
    inputID++;
    message_t* msg = create_msg(inputID, "Alice", "Bob", "Hello, Bob!");
    if (msg == NULL) {
        fprintf(stderr, "Failed to create message %d\n", inputID);
        return -1;
    }

    print_msg_stored(msg, inputID);


    // TEST - id1 message storage
    if (store_msg(msg, filename) != 0) {
        fprintf(stderr, "Failed to store message %d\n", inputID);
        return -1;
    }
    free(msg);


    // TEST - id2 message creation
    inputID++;
    msg = create_msg(inputID, "Rick", "Hailey", "Need these reports by tomorrow. Thank you.");
    if (msg == NULL) {
        fprintf(stderr, "Failed to create message %d\n", inputID);
        return -1;
    }

    print_msg_stored(msg, inputID);

    // TEST - id2 message storage
    if (store_msg(msg, filename) != 0) {
        fprintf(stderr, "Failed to store message %d\n", inputID);
        return -1;
    }
    free(msg);

    // TEST - id2 input same message ID
    msg = create_msg(inputID, "Rick", "Hailey", "Need these reports by tomorrow. Thank you.");
    if (msg == NULL) {
        fprintf(stderr, "Failed to create message %d\n", inputID);
        return -1;
    }

    print_msg_stored(msg, inputID);

    // TEST - id2 input same message ID
    if (store_msg(msg, filename) != 0) {
        fprintf(stderr, "Failed to store message %d\n", inputID);
        return -1;
    }
    free(msg);

    // TEST - id3 message creation with |
    inputID++;
    msg = create_msg(inputID, "Sally", "Quan", "Happy Birthday!| Thank you.");
    if (msg == NULL) {
        fprintf(stderr, "Failed to create message %d\n", inputID);
        return -1;
    }

    print_msg_stored(msg, inputID);

    // TEST - id3 message storage
    if (store_msg(msg, filename) != 0) {
        fprintf(stderr, "Failed to store message %d\n", inputID);
        return -1;
    }
    free(msg);

    // TEST - message retrieval into BST
    bst_t* tree = new_bst();

    if (tree == NULL) {
        fprintf(stderr, "Failed to create BST\n");
        return -1;
    }

    if (retrieve_msg(filename, tree) != 0) {
        fprintf(stderr, "Failed to retrieve messages from file\n");
        clear_and_free_bst(tree);
        return -1;
    }
    // TEST - message find all inputIDs
    for (int id = 1; id <= inputID; id++) {
        message_t* found_msg = bst_find(tree, id);
        if (found_msg != NULL) {
            print_msg_found(found_msg);
        } else {
            printf("Message with ID %d not found\n", id);
        }
    }

    // TEST - message find id3
    message_t* found_msg1 = bst_find(tree, 3);
    if (found_msg1 != NULL) {
        print_msg_found(found_msg1);
    } else {
        printf("Message with ID %d not found\n", 3);
    }

    clear_and_free_bst(tree);
    
    return 0;
}