/**
 * @file messages.c / source code for Messages.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date 2025-10-31
 *
 */

#include "messages.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bst.h"

static int NEXT_ID;
static const char* NEXT_ID_PATH = "../data/_NEXT_ID.txt";

// todo: use this function when creating new messages to prevent messages with duplicate id numbers
/**
 * @brief Gets the next available message ID and updates the counter
 *
 * Reads the current ID from a file, increments it, saves it back,
 * and returns the original value.
 *
 * @return int The next available message ID
 */
int get_next_id() {
    FILE* id_file;

    // get ID from the file
    id_file = fopen(NEXT_ID_PATH, "r");
    if (id_file) {
        fscanf(id_file, "%d", &NEXT_ID);
        fclose(id_file);
    } else {
        NEXT_ID = 0;  // initialize first id number
    }

    // update file with next ID number
    id_file = fopen(NEXT_ID_PATH, "w");
    if (id_file) {
        fprintf(id_file, "%d", NEXT_ID + 1);
        fclose(id_file);
    } else {
        printf("WARNING: unable to save ID to file, next message ID will likely clash\n");
    }

    return NEXT_ID;
}

/**
 * @brief Creates a message object from its constituent parts
 *
 * @param id The unique identifier for the message
 * @param sender The name of the message sender
 * @param receiver The name of the message receiver
 * @param content The message content (if NULL or empty, will be set to "N/A")
 * @param time_sent The timestamp when the message was sent
 * @param delivered_flag Whether the message has been delivered
 * @return message_t* A newly allocated message object, or NULL if sender/receiver are invalid
 */
message_t* create_msg_from_parts(int id, char* sender, char* receiver, char* content, time_t time_sent,
                                 bool delivered_flag) {
    // first do null checks
    if (sender == NULL || receiver == NULL) {
        return NULL;
    }
    // make sure sender/receiver aren't empty strings
    else if (strlen(sender) == 0 || strlen(receiver) == 0) {
        return NULL;
    }
    // allow "empty" content but fill it in with something
    if (content == NULL || strlen(content) == 0) {
        content = "N/A";
    }

    message_t* msg = malloc(sizeof(message_t));

    msg->id = id;
    msg->time_sent = time_sent;
    msg->sender = strdup(sender);
    msg->receiver = strdup(receiver);
    msg->delivered_flag = delivered_flag;
    msg->content = strdup(content);

    return msg;
}

/**
 * @brief Construct a message element with all values input.
 *
 * @param sender char* - sender of the message
 * @param receiver char* - receiver of the message
 * @param content char* - content of the message
 * @return message_t* - pointer to the constructed message element
 */
message_t* create_msg(char* sender, char* receiver, char* content) {
    time_t now;
    time(&now);
    int id = get_next_id(ID_FILE);
    bool delivered_flag = false;

    return create_msg_from_parts(id, sender, receiver, content, now, delivered_flag);
}

/**
 * @brief Frees the memory allocated for a message struct
 *
 * @param message the message struct to free
 */
void free_msg(message_t* msg) {
    if (msg == NULL) {
        return;
    }
    free(msg->sender);
    free(msg->receiver);
    free(msg->content);
    free(msg);
}

/**
 * @brief store a message element to a message store on disk.
 *
 * @param msg message_t* - pointer to the message element
 * @param filename char* - name of the file to store the message
 * @return int - status code (0 for success, -1 for failure)
 */
int store_msg(message_t* msg, char* filename) {
    if (msg == NULL) {
        fprintf(stderr, "ERROR: message is NULL\n");
        return -1;
    }

    FILE* wFile = fopen(filename, "a");  // overwrite or write new to file - cleaning file at make
    if (wFile == NULL) {
        fprintf(stderr, "ERROR: could not open file for writing\n");
        return -1;
    }

    // Write the message data to the file
    fprintf(wFile, "%d|", msg->id);  // fprintf() has issues when printing when there is no null terminator '\0'
    fprintf(wFile, "%ld|", msg->sentTime);
    fprintf(wFile, "%s|", msg->sender);
    fprintf(wFile, "%s|", msg->receiver);
    fprintf(wFile, "%s|", msg->content);
    fprintf(wFile, "%d\n", msg->sentFlag);

    fclose(wFile);
    return 0;  // need to free after use
}

/**
 * @brief Returns a string value representing the message. Note: need to free the returned string after use.
 *
 * @param message the message to convert to a string
 * @return a string representation of the message
 */
char* message_to_str(message_t* message) {
    char* str = (char*) malloc(MAX_INPUT_LENGTH * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }
    char* string_timeSent = ctime(&message->sentTime);
    sprintf(str, "{id: %d, sender: %s, receiver: %s, content: %s, sentTime: %s}\n", message->id, message->sender,
            message->receiver, message->content, string_timeSent);
    // sprintf(str, "{sender: %s, receiver: %s, content: %s, id: %d}", message->sender, message->receiver,
    // message->content, message->id);
    return str;
}

/**
 * @brief Compares two message structs based on message id
 *
 * @param msg1 the first message to compare
 * @param msg2 the second message to compare
 * @return -1 if the msg1 comes before msg2, 0 if they are equal, 1 if msg1 comes after msg2
 */
int compare_messages(message_t* msg1, message_t* msg2) {
    if (msg1->id < msg2->id) {
        return -1;
    } else if (msg1->id > msg2->id) {
        return 1;
    } else {
        return 0;
    }
    return 0;
}


/**
 * @brief Appends a token to a string with a comma separator
 *
 * @param original_str The original string to append to (will be reallocated)
 * @param token The token to append after a comma
 * @return char* The new string containing original_str + "," + token
 */
char* append_with_comma(char* original_str, const char* token) {
    int new_length = strlen(original_str) + strlen(token) + 2;  //+1 for null terminator and +1 for comma

    char* new_str = realloc(original_str, sizeof(char) * new_length);
    strcat(new_str, ",");
    strcat(new_str, token);

    return new_str;
}

/**
 * @brief Retrieve a message element from a message store on disk.
 *
 * @param filename char* - name of the file to read the messages from
 * @param tree bst_t* - pointer to the binary search tree
 * @return int - status code (0 for success, -1 for failure)
 */
int retrieve_msg(char* filename, bst_t* tree) {
    FILE* rFile = fopen(filename, "r");  // overwrite or write new to file - cleaning file at make

    if (rFile == NULL) {
        fprintf(stderr, "ERROR: could not open file for reading");
        return -1;
    }
    if (tree == NULL) {
        fclose(rFile);
        fprintf(stderr, "ERROR: bst tree is NULL");
        return -1;
    }

    char ch;
    int wordCount = 0;
    char temp[MAX_INPUT_LENGTH];
    int i = 0;
    int numOfMsgMembers = 6;  // id, sentTime, sender, receiver, content, sentFlag
    message_t* bstMsg = create_empty_msg();

    while ((ch = fgetc(rFile)) != EOF) {
        if (ch == '|' || ch == '\n') {
            temp[i] = '\0';  // convert delimiter to null terminator
            // get words by delimiter and store into message struct
            if (wordCount % numOfMsgMembers == 0) {
                bstMsg->id = atoi(temp);
            } else if (wordCount % numOfMsgMembers == 1) {
                bstMsg->sentTime = (time_t) atol(temp);
            } else if (wordCount % numOfMsgMembers == 2) {
                bstMsg->sender = malloc(sizeof(char) * (strlen(temp) + 1));
                strcpy(bstMsg->sender, temp);
            } else if (wordCount % numOfMsgMembers == 3) {
                bstMsg->receiver = malloc(sizeof(char) * (strlen(temp) + 1));
                strcpy(bstMsg->receiver, temp);
            } else if (wordCount % numOfMsgMembers == 4) {
                bstMsg->content = malloc(sizeof(char) * (strlen(temp) + 1));
                strcpy(bstMsg->content, temp);
            }
            wordCount++;

            if (ch == '\n') {
                // End of message, process the current message
                if (bstMsg != NULL) {
                    // Insert the message into the BST
                    bst_add(tree, bstMsg);
                }
                bstMsg =
                    create_empty_msg();  // reset msg element to intake next message. Note: remember to free last msg
                                         // after end of loop - this will be end of loop since file ends with newline.
            }

            i = 0;           // reset the index for temp once word is stored
            temp[i] = '\0';  // reset the temp word char array to gather new word
            continue;
        }
        temp[i] = ch;
        i++;
    }

    /*
    if (wordCount == 0) {
        free(bstmsg);
        fclose(file);
        fprintf(stderr, "ERROR: Empty file\n");
        return -1;
    }
    */
    free(bstMsg);  // need to free the last msg that's created by the last newline in the file
    fclose(rFile);

    return 0;  // need to free after use
}