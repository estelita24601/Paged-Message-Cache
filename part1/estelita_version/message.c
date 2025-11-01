/**
 * @file message.c
 * @author Estelita Chen
 * @date 2025-10-24
 *
 */

#define _XOPEN_SOURCE 700
#include "message.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char* TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
static const int TIME_FORMAT_LEN = 19;  // Y4 + m2 + d2 + H2 + M2 + S2 + 4 punctuations + 1 whitespace

static const int MAX_STORE_SIZE = 1000;
static const int MAX_LINE_LENGTH = 500;
static const char* ID_FILE = "message_store/id_counter.txt";
static int ID;

int get_next_id(const char* id_counter_path) {
    FILE* id_file;

    // get ID from the file
    id_file = fopen(id_counter_path, "r");
    if (id_file) {
        fscanf(id_file, "%d", &ID);
        fclose(id_file);
    } else {
        ID = 0;  // initialize first id number
    }

    // update file with next ID number
    id_file = fopen(id_counter_path, "w");
    if (id_file) {
        fprintf(id_file, "%d", ID + 1);
        fclose(id_file);
    } else {
        printf("WARNING: unable to save ID to file, next message ID will likely clash\n");
    }

    return ID;
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

message_t* create_msg(char* sender, char* receiver, char* content) {
    time_t now;
    time(&now);
    int id = get_next_id(ID_FILE);
    bool delivered_flag = false;

    return create_msg_from_parts(id, sender, receiver, content, now, delivered_flag);
}

void free_msg(message_t* msg) {
    if (msg == NULL) {
        return;
    }
    free(msg->sender);
    free(msg->receiver);
    free(msg->content);
    free(msg);
}

bool msg_compare(message_t* msgA, message_t* msgB) {
    // first do null checks
    if (msgA == NULL || msgB == NULL) {
        return false;
    }

    if (strcmp(msgA->sender, msgB->sender) != 0) {
        return false;
    }
    if (strcmp(msgA->receiver, msgB->receiver) != 0) {
        return false;
    }
    if (strcmp(msgA->content, msgB->content) != 0) {
        return false;
    }
    if (msgA->delivered_flag != msgB->delivered_flag) {
        return false;
    }

    return true;
}

char* msg_to_str(message_t* msg) {
    if (msg == NULL) {
        return NULL;
    }

    // calculate length for csv string
    int len = 0;

    if (msg->id == 0) {
        len += 1;
    } else {
        len += log10(msg->id) + 1;
    }
    len += strlen(msg->sender);
    len += strlen(msg->receiver);
    len += strlen(msg->content);
    len += TIME_FORMAT_LEN;
    len += 1;  // bool prints as one digit number 0 or 1
    len += 5;  // 6 fields in msg object means 5 commas in csv
    char* csv_str = malloc(sizeof(char) * (len + 1));

    // turn time_t into formatted string
    struct tm* tm = localtime(&msg->time_sent);
    char time[TIME_FORMAT_LEN + 1];
    strftime(time, TIME_FORMAT_LEN + 1, TIME_FORMAT, tm);

    // NEW EXPECTED FORMAT: id,sender,receiver,time_sent,delivered_flag,content
    sprintf(csv_str, "%d,%s,%s,%s,%d,%s", msg->id, msg->sender, msg->receiver, time, msg->delivered_flag, msg->content);

    return csv_str;
}

/**
 * @brief Parses a string token, handling NULL and empty string cases
 *
 * @param token The string token to parse
 * @return char* A newly allocated copy of the token, or NULL if token is NULL or empty
 */
char* parse_string_token(const char* token) {
    if (token == NULL || strlen(token) == 0) {
        return NULL;
    } else {
        return strdup(token);
    }
}

/**
 * @brief takes a string token and turns it into a bool
 *
 * @param token The string token to parse ("0" or "1")
 * @return true If token is "1"
 * @return false If token is "0" or invalid
 */
bool parse_bool_token(const char* token) {
    if (strcmp(token, "0") == 0) {
        return false;
    } else if (strcmp(token, "1") == 0) {
        return true;
    } else {
        return NULL;  // this wasn't a valid boolean
    }
}

/**
 * @brief Parses a string token for time_sent in the format specified by TIME_FORMAT
 *
 * @param token The string token containing the time in format "%Y-%m-%d %H:%M:%S"
 * @param parsed_time Pointer to time_t where the parsed time will be stored
 */
void parse_time_token(const char* token, time_t* parsed_time) {
    struct tm tm_sent;
    memset(&tm_sent, 0, sizeof(struct tm));  // initialize
    strptime(token, TIME_FORMAT, &tm_sent);
    *parsed_time = mktime(&tm_sent);
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
 * @brief Creates a message object from its CSV string representation
 *
 * The expected CSV format is: id,sender,receiver,time_sent,delivered_flag,content
 * Where:
 * - id: numeric identifier
 * - sender: non-empty string
 * - receiver: non-empty string
 * - time_sent: timestamp in format "%Y-%m-%d %H:%M:%S"
 * - delivered_flag: 0 or 1
 * - content: string (if empty will be set to "N/A")
 *
 * Note: If content contains commas, they will be preserved and properly handled
 *
 * @param input_str The CSV string to parse
 * @return message_t* A newly allocated message object, or NULL if the string is invalid
 */
message_t* create_msg_from_str(const char* input_str) {
    if (input_str == NULL) {
        return NULL;
    } else if (strlen(input_str) < 5) {
        // there should at least be 5 commas
        return NULL;
    }

    // make duplicate string so original isn't modified by strtok
    char* csv_str = strdup(input_str);
    // strip newline character if there is one
    csv_str[strcspn(csv_str, "\n")] = '\0';

    // empty variables for creating the message object
    int id = -1;
    char* sender = NULL;
    char* receiver = NULL;
    char* content = NULL;
    time_t time_sent;
    bool delivered_flag;

    // NEW EXPECTED FORMAT: id,sender,receiver,time_sent,delivered_flag,content, more content, more content
    char* token = strtok(csv_str, ",");
    int i = 0;
    while (token != NULL) {
        // printf("%d-th token = %s\n", i, token);  // DEBUG PRINT

        if (i == 0) {  // token = id

            id = atoi(token);
        } else if (i == 1) {  // token = sender

            sender = parse_string_token(token);
        } else if (i == 2) {  // token = receiver

            receiver = parse_string_token(token);
        } else if (i == 3) {  // token = time_sent

            parse_time_token(token, &time_sent);
        } else if (i == 4) {  // token = delivered_flag

            delivered_flag = parse_bool_token(token);
        } else if (i == 5) {  // token = content
            if (strlen(token) == 0) {
                content = strdup("N/A");
            } else {
                content = strdup(token);
                //content = malloc(sizeof(char) * strlen(token) + 1)
                //strcpy(content, token)
            }
        } else {  // i>5 means multiple tokens in the content field
            // append to the end of existing content and don't forget to add comma back in
            content = append_with_comma(content, token);
            // content = content + "," + token
        }

        // try to get next token
        token = strtok(NULL, ",");
        i++;
    }

    message_t* msg;
    if (i >= 6) {
        msg = create_msg_from_parts(id, sender, receiver, content, time_sent, delivered_flag);
    } else {
        msg = NULL;  // not enough fields in the csv string
    }

    free(sender);
    free(receiver);
    free(content);
    free(csv_str);
    return msg;
}

/**
 * @brief Replaces an existing message in the store with a new version
 *
 * @param new_msg The new message to store (must have valid ID)
 * @param id The ID of the message to replace (must match new_msg->id)
 * @return true If the message was successfully replaced
 * @return false If any error occurred during
 */
bool replace_msg(message_t* new_msg, int id) {
    // printf("entered replace_msg() function\n");  // DEBUG PRINT

    message_t* all_messages[MAX_STORE_SIZE];
    char* buffer = malloc(sizeof(char) * MAX_LINE_LENGTH);
    FILE* store = fopen(STORE_PATH, "r");

    // null checks
    if (buffer == NULL) {
        printf("ERROR: unable to allocate memory for file reading buffer");
        return false;
    } else if (store == NULL) {
        printf("ERROR: unable to open message store in read mode");
        return false;
    }

    // read from store and save all messages from the store into an array
    int message_count = 0;
    while (fgets(buffer, MAX_LINE_LENGTH, store) != NULL) {
        message_t* curr_msg = create_msg_from_str(buffer);

        if (curr_msg != NULL) {  // we were able to create message object
            if (curr_msg->id == new_msg->id) {
                // if message id matches then replace it with our new message
                all_messages[message_count] = new_msg;
                free_msg(curr_msg);
            } else {
                // keep all other messages the same
                all_messages[message_count] = curr_msg;
            }
            message_count++;
        } else {
            printf("ERROR: unable to create message_t object from string '%s'\n", buffer);
            fclose(store);
            free(buffer);
            return false;
        }
    }
    // printf("read %d messages from the store\n", message_count);  // DEBUG PRINT
    free(buffer);
    fclose(store);

    // overwrite store with our array of messages
    store = fopen(STORE_PATH, "w");
    if (store == NULL) {
        printf("ERROR: unable to open message store in write mode");
        return false;
    }

    for (int i = 0; i < message_count; i++) {
        // look at each message in the array of message objects
        message_t* curr_msg = all_messages[i];

        // try to turn it into a string we can write to the file
        char* msg_csv_string = msg_to_str(curr_msg);
        if (msg_csv_string != NULL) {
            fprintf(store, "%s\n", msg_csv_string);
            free(msg_csv_string);
            // make sure we don't free new_msg object that belongs to the caller
            if (curr_msg->id != new_msg->id) {
                free_msg(curr_msg);
            }
        } else {
            printf("WARNING: a message in store has been lost due to inability to convert into a string\n");
            // chose not to end loop/function to avoid loss of all messages after this one
        }
    }
    fclose(store);
    return true;
}

bool store_msg(message_t* msg) {
    if (msg == NULL) {
        return false;
    }

    message_t* existing_msg = retrieve_msg(msg->id);

    if (existing_msg == NULL) {  // message isnt in the store yet
        // we can just append to the end of the file
        FILE* store = fopen(STORE_PATH, "a");
        if (store == NULL) {
            printf("ERROR: unable to open message store in append mode\n");
            return false;
        }

        char* msg_csv = msg_to_str(msg);
        if (msg_csv != NULL) {
            fprintf(store, "%s\n", msg_csv);
            free(msg_csv);
            fclose(store);
            return true;
        } else {
            printf("WARNING: unable to turn message_t object into a csv string\n");
            return false;
        }
    } else {  // message is already in the store
        // we need to edit/replace the existing message
        bool success = replace_msg(msg, msg->id);
        free_msg(existing_msg);
        return success;
    }
}

message_t* retrieve_msg(int id) {
    FILE* store_file = fopen(STORE_PATH, "r");
    if (store_file == NULL) {
        printf("ERROR: unable to open message store");
        return NULL;
    }

    char* buffer = malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (buffer == NULL) {
        printf("ERROR: unable to allocate memory for file reading buffer");
        return NULL;
    }

    message_t* msg = NULL;
    while (fgets(buffer, MAX_LINE_LENGTH, store_file) != NULL) {
        // printf("%s\n", buffer);  // DEBUG PRINT
        msg = create_msg_from_str(buffer);

        if (msg != NULL && msg->id == id) {
            // printf("\tfound msg!");  // DEBUG PRINT
            break;
        } else {
            free_msg(msg);
            msg = NULL;
        }
    }

    free(buffer);
    fclose(store_file);
    return msg;
}
