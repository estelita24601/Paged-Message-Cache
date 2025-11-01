/*
 * messages.c / source code of Messages.
 *
 * Lori Kim / CS5600 / Northeastern University
 *
 * Fall 2025 / Sep 28, 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "bst.h"
#include "messages.h"

static int NEXT_ID;
static const char *NEXT_ID_PATH = "../data/_NEXT_ID.txt";

// todo: use this function when creating new messages to prevent messages with duplicate id numbers
/**
 * @brief Gets the next available message ID and updates the counter
 *
 * Reads the current ID from a file, increments it, saves it back,
 * and returns the original value.
 *
 * @return int The next available message ID
 */
int get_next_id()
{
    FILE *id_file;

    // get ID from the file
    id_file = fopen(NEXT_ID_PATH, "r");
    if (id_file)
    {
        fscanf(id_file, "%d", &NEXT_ID);
        fclose(id_file);
    }
    else
    {
        NEXT_ID = 0; // initialize first id number
    }

    // update file with next ID number
    id_file = fopen(NEXT_ID_PATH, "w");
    if (id_file)
    {
        fprintf(id_file, "%d", NEXT_ID + 1);
        fclose(id_file);
    }
    else
    {
        printf("WARNING: unable to save ID to file, next message ID will likely clash\n");
    }

    return NEXT_ID;
}

/**
 * @brief Construct a message element with all values input. Note: do not need to free this element since the values stored are in stack.
 *
 * @param id int - id of the message element
 * @param sender char* - sender of the message
 * @param receiver char* - receiver of the message
 * @param content char* - content of the message
 * @return message_t* - pointer to the constructed message element
 */
message_t *create_msg(int id, char *sender, char *receiver, char *content)
{
    message_t *message = calloc(1, sizeof(message_t));
    if (message == NULL)
    {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }
    message->id = id;
    time_t now = time(NULL);
    message->sentTime = now; // calculate at time of construction
    message->sender = sender;
    message->receiver = receiver;
    message->content = content;
    message->sentFlag = false;
    return message; // need to free after use
}

/**
 * @brief Construct a message element. Note: need to free this construct as the sender, receiver, content will most likely be stored on the heap unless values are known to be static.
 *
 * @return message_t* - pointer to the constructed message element
 */
message_t *create_empty_msg()
{
    message_t *message = calloc(1, sizeof(message_t));
    if (message == NULL)
    {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }

    message->id = -1;
    message->sentTime = 0; // calculate at time of construction
    message->sender = NULL;
    message->receiver = NULL;
    message->content = NULL;
    message->sentFlag = false;
    return message; // need to free after use
}

/**
 * @brief store a message element to a message store on disk.
 *
 * @param msg message_t* - pointer to the message element
 * @param filename char* - name of the file to store the message
 * @return int - status code (0 for success, -1 for failure)
 */
int store_msg(message_t *msg, char *filename)
{
    if (msg == NULL)
    {
        fprintf(stderr, "ERROR: message is NULL\n");
        return -1;
    }

    FILE *wFile = fopen(filename, "a"); // overwrite or write new to file - cleaning file at make
    if (wFile == NULL)
    {
        fprintf(stderr, "ERROR: could not open file for writing\n");
        return -1;
    }

    // Write the message data to the file
    fprintf(wFile, "%d|", msg->id); // fprintf() has issues when printing when there is no null terminator '\0'
    fprintf(wFile, "%ld|", msg->sentTime);
    fprintf(wFile, "%s|", msg->sender);
    fprintf(wFile, "%s|", msg->receiver);
    fprintf(wFile, "%s|", msg->content);
    fprintf(wFile, "%d\n", msg->sentFlag);

    fclose(wFile);
    return 0; // need to free after use
}

//todo: maybe?? make this a helper function for store_msg instead
/**
 * @brief Returns a string value representing the message. Note: need to free the returned string after use.
 *
 * @param message the message to convert to a string
 * @return a string representation of the message
 */
char *message_to_str(message_t *message)
{
    char *str = (char *)malloc(MAX_INPUT_LENGTH * sizeof(char));
    if (str == NULL)
    {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }
    char *string_timeSent = ctime(&message->sentTime);
    sprintf(str, "{id: %d, sender: %s, receiver: %s, content: %s, sentTime: %s}\n", message->id, message->sender, message->receiver, message->content, string_timeSent);
    // sprintf(str, "{sender: %s, receiver: %s, content: %s, id: %d}", message->sender, message->receiver, message->content, message->id);
    return str;
}

/**
 * @brief Compares two message structs based on message id
 *
 * @param msg1 the first message to compare
 * @param msg2 the second message to compare
 * @return -1 if the msg1 comes before msg2, 0 if they are equal, 1 if msg1 comes after msg2
 */
int compare_messages(message_t *msg1, message_t *msg2)
{

    if (msg1->id < msg2->id)
    {
        return -1;
    }
    else if (msg1->id > msg2->id)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}

/**
 * @brief Frees the memory allocated for a message struct
 *
 * @param message the message struct to free
 */
void free_message(message_t *message)
{
    if (message == NULL)
        return;

    free(message->sender);
    free(message->receiver);
    free(message->content);
    // free(message);
}

/**
 * @brief Retrieve a message element from a message store on disk.
 *
 * @param filename char* - name of the file to read the messages from
 * @param tree bst_t* - pointer to the binary search tree
 * @return int - status code (0 for success, -1 for failure)
 */
int retrieve_msg(char *filename, bst_t *tree)
{

    FILE *rFile = fopen(filename, "r"); // overwrite or write new to file - cleaning file at make

    if (rFile == NULL)
    {
        fprintf(stderr, "ERROR: could not open file for reading");
        return -1;
    }
    if (tree == NULL)
    {
        fclose(rFile);
        fprintf(stderr, "ERROR: bst tree is NULL");
        return -1;
    }

    char ch;
    int wordCount = 0;
    char temp[MAX_INPUT_LENGTH];
    int i = 0;
    int numOfMsgMembers = 6; // id, sentTime, sender, receiver, content, sentFlag
    message_t *bstMsg = create_empty_msg();

    while ((ch = fgetc(rFile)) != EOF)
    {
        if (ch == '|' || ch == '\n')
        {

            temp[i] = '\0'; // convert delimiter to null terminator
            // get words by delimiter and store into message struct
            if (wordCount % numOfMsgMembers == 0)
            {
                bstMsg->id = atoi(temp);
            }
            else if (wordCount % numOfMsgMembers == 1)
            {
                bstMsg->sentTime = (time_t)atol(temp);
            }
            else if (wordCount % numOfMsgMembers == 2)
            {
                bstMsg->sender = malloc(sizeof(char) * (strlen(temp) + 1));
                strcpy(bstMsg->sender, temp);
            }
            else if (wordCount % numOfMsgMembers == 3)
            {
                bstMsg->receiver = malloc(sizeof(char) * (strlen(temp) + 1));
                strcpy(bstMsg->receiver, temp);
            }
            else if (wordCount % numOfMsgMembers == 4)
            {
                bstMsg->content = malloc(sizeof(char) * (strlen(temp) + 1));
                strcpy(bstMsg->content, temp);
            }
            wordCount++;

            if (ch == '\n')
            {
                // End of message, process the current message
                if (bstMsg != NULL)
                {
                    // Insert the message into the BST
                    bst_add(tree, bstMsg);
                }
                bstMsg = create_empty_msg(); // reset msg element to intake next message. Note: remember to free last msg after end of loop - this will be end of loop since file ends with newline.
            }

            i = 0;          // reset the index for temp once word is stored
            temp[i] = '\0'; // reset the temp word char array to gather new word
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
    free(bstMsg); // need to free the last msg that's created by the last newline in the file
    fclose(rFile);

    return 0; // need to free after use
}