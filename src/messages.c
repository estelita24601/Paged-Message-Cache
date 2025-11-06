/**
 * @file messages.c / source code for Messages.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */


#define _XOPEN_SOURCE 700

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "messages.h"

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
 * @brief use the id number to create the properly formatted filename for this message
 *
 * @param msg_id - int
 * @return char* - filename for this message ON THE HEAP
 */
char* create_msg_filename(int msg_id) {
    char* filename = malloc(sizeof(char) * (strlen(MESSAGE_FILENAME_FORMAT) + log10(msg_id) + 2)); // TODO: finalize a cap size for the id number
    sprintf(filename, MESSAGE_FILENAME_FORMAT, msg_id);
    return filename;
}

/**
 * @brief Creates a message object from its constituent parts
 *
 * @param id The unique identifier for the message
 * @param sender The name of the message sender
 * @param receiver The name of the message receiver
 * @param content The message content (if NULL or empty, will be set to "N/A")
 * @param time_sent The timestamp when the message was sent
 * @param sentFlag Whether the message has been delivered
 * @return message_t* A newly allocated message object, or NULL if sender/receiver are invalid
 */
message_t* create_msg_from_parts(int id, char* sender, char* receiver, char* content, time_t time_sent, bool sentFlag) {
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
    msg->sentTime = time_sent;
    msg->sender = strdup(sender);
    msg->receiver = strdup(receiver);
    msg->sentFlag = sentFlag;
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
    int id = get_next_id();
    bool sentFlag = false;

    return create_msg_from_parts(id, sender, receiver, content, now, sentFlag);
}

/**
 * @brief Frees the memory allocated for a message struct
 *
 * @param message the message struct to free
 */
void free_message(message_t* msg) {
    if (msg == NULL) {
        return;
    }
    free(msg->sender);
    free(msg->receiver);
    free(msg->content);
    free(msg);
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
                // content = malloc(sizeof(char) * strlen(token) + 1)
                // strcpy(content, token)
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

char* msg_to_csv(message_t* msg) {
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
    struct tm* tm = localtime(&msg->sentTime);
    char time[TIME_FORMAT_LEN + 1];
    strftime(time, TIME_FORMAT_LEN + 1, TIME_FORMAT, tm);

    // NEW EXPECTED FORMAT: id,sender,receiver,time_sent,delivered_flag,content
    sprintf(csv_str, "%d,%s,%s,%s,%d,%s", msg->id, msg->sender, msg->receiver, time, msg->sentFlag, msg->content);

    return csv_str;
}


/**
 * @brief store a message element to a message store on disk.
 *
 * @param msg message_t* - pointer to the message element
 * @param filename char* - name of the file to store the message
 * @return int - status code (0 for success, -1 for failure)
 */
bool store_msg(message_t* msg) {
    if (msg == NULL) {
        printf("ERROR: tried to store a NULL message\n");
        return false;
    }

    char* expected_filename = create_msg_filename(msg->id);
    FILE* file = fopen(expected_filename, "w");

    if (file == NULL) {
        printf("ERROR: unable to write to %s\n", expected_filename);
        free(expected_filename);
        return false;
    } else {
        bool success;

        char* msg_csv = msg_to_csv(msg);
        if (msg_csv != NULL) {
            fprintf(file, "%s", msg_csv);  // QUESTION: should this have \n at the end?
            free(msg_csv);
            success = true;
        } else {
            printf("ERROR: unable to turn message object into a csv string\n");
            success = false;
        }

        free(expected_filename);
        fclose(file);
        return success;
    }
}

/**
 * @brief Returns a string value representing the message. Note: need to free the returned string after use.
 *
 * @param message the message to convert to a string
 * @return a string representation of the message
 */
char* message_to_pretty_str(message_t* message) {
    char* str = (char*) malloc(MAX_CSV_LENGTH * sizeof(char));
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
 * @brief Compares two message structs based on message id.
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
 * @brief
 *
 * @param id
 * @return message_t*
 */
message_t* retrieve_msg(int id) {
    char* expected_filename = create_msg_filename(id);

    // initialize message object as null
    message_t* msg = NULL;

    // try to open the file
    FILE* msg_file = fopen(expected_filename, "r");
    if (msg_file == NULL) {
        printf("WARNING: unable to find message with id = %d in the store\n", id);
    } else {
        char* buffer = malloc(sizeof(char) * MAX_CSV_LENGTH);
        if (fgets(buffer, MAX_CSV_LENGTH, msg_file) == NULL) {
            printf("WARNING: unable to read contents of %s or contents do not exist\n", expected_filename);
        } else {
            msg = create_msg_from_str(buffer);
        }
        free(buffer);
    }

    free(expected_filename);
    return msg;
}