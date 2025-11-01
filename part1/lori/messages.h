/*
* messages.h / header file for Messages.
*
* Lori Kim / CS5600 / Northeastern University

* Fall 2025 / Sep 28, 2025
*
*/


#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define MAX_INPUT_LENGTH 1024

// forward declaration of bst_t
typedef struct bst_t bst_t;


/**
 * @brief Represents a message.
 */
typedef struct message_t {
    int id; // id of the message
    time_t sentTime; // time the message was sent
    char* sender; // sender of the message
    char* receiver; // receiver of the message
    char* content; // content of the message
    bool sentFlag; // flag indicating if the message has been delivered
} message_t;


/**
 * @brief Construct a message element.
 *
 * @param id int - id of the message element
 * @param sender char* - sender of the message
 * @param receiver char* - receiver of the message
 * @param content char* - content of the message
 * @return message_t* - pointer to the constructed message element
 */
message_t* create_msg(int id, char* sender, char* receiver, char* content);


/**
 * @brief Construct a message element.
 *
 * @return message_t* - pointer to the constructed message element
 */
message_t* create_empty_msg();


/**
 * @brief store a message element to a message store on disk.
 *
 * @param msg message_t* - pointer to the message element
 * @param filename char* - name of the file to store the message
 * @return int - status code (0 for success, -1 for failure)
 */
int store_msg(message_t* msg, char* filename);


/**
 * @brief Returns a string value representing the message
 *
 * @param message the message to convert to a string
 * @return a string representation of the message
 */
char *message_to_str(message_t *message);


/**
 * @brief Compares two message structs based on message id
 *
 * @param msg1 the first message to compare
 * @param msg2 the second message to compare
 * @return -1 if the msg1 comes before msg2, 0 if they are equal, 1 if msg1 comes after msg2
 */
int compare_messages(message_t *msg1, message_t *msg2);


/**
 * @brief Frees the memory allocated for a message struct
 *
 * @param message the message struct to free
 */
void free_message(message_t *message);


/**
 * @brief Retrieve a message element from a message store on disk.
 *
 * @param filename char* - name of the file to read the messages from
 * @param tree bst_t* - pointer to the binary search tree
 * @return int - status code (0 for success, -1 for failure)
 */
int retrieve_msg(char* filename, bst_t* tree);



#endif // MESSAGES_H