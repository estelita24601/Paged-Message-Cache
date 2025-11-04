/**
 * @file messages.h / header file for Messages.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INPUT_LENGTH 1024
#define STORE_PATH "message_store/messages.csv"

// forward declaration of bst_t
typedef struct bst_t bst_t;

/**
 * @brief Represents a message.
 */
typedef struct message_t {
    int id;           // id of the message
    time_t sentTime;  // time the message was sent
    char* sender;     // sender of the message
    char* receiver;   // receiver of the message
    char* content;    // content of the message
    bool sentFlag;    // flag indicating if the message has been delivered
} message_t;

/**
 * @brief Gets the next available message ID and updates the counter
 *
 * Reads the current ID from a file, increments it, saves it back,
 * and returns the original value.
 *
 * @return int The next available message ID
 */
int get_next_id();

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
message_t* create_msg_from_parts(int id, char* sender, char* receiver, char* content, time_t time_sent, bool sentFlag);

/**
 * @brief Construct a message element with all values input.
 *
 * @param sender char* - sender of the message
 * @param receiver char* - receiver of the message
 * @param content char* - content of the message
 * @return message_t* - pointer to the constructed message element
 */
message_t* create_msg(char* sender, char* receiver, char* content);

/**
 * @brief Frees the memory allocated for a message struct
 *
 * @param message the message struct to free
 */
void free_message(message_t* message);

/**
 * @brief Parses a string token, handling NULL and empty string cases
 *
 * @param token The string token to parse
 * @return char* A newly allocated copy of the token, or NULL if token is NULL or empty
 */
char* parse_string_token(const char* token);

/**
 * @brief takes a string token and turns it into a bool
 *
 * @param token The string token to parse ("0" or "1")
 * @return true If token is "1"
 * @return false If token is "0" or invalid
 */
bool parse_bool_token(const char* token);

/**
 * @brief Parses a string token for time_sent in the format specified by TIME_FORMAT
 *
 * @param token The string token containing the time in format "%Y-%m-%d %H:%M:%S"
 * @param parsed_time Pointer to time_t where the parsed time will be stored
 */
void parse_time_token(const char* token, time_t* parsed_time);

/**
 * @brief Appends a token to a string with a comma separator
 *
 * @param original_str The original string to append to (will be reallocated)
 * @param token The token to append after a comma
 * @return char* The new string containing original_str + "," + token
 */
char* append_with_comma(char* original_str, const char* token);

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
message_t* create_msg_from_str(const char* input_str);

/**
 * @brief store a message element to a message store on disk.
 *
 * @param msg message_t* - pointer to the message element
 * @param filename char* - name of the file to store the message
 * @return int - status code (0 for success, -1 for failure)
 */
bool store_msg(message_t* msg);

/**
 * @brief Returns a string value representing the message. Note: need to free the returned string after use.
 *
 * @param message the message to convert to a string
 * @return a string representation of the message
 */
char* message_to_pretty_str(message_t* message);

/**
 * @brief Compares two message structs based on message id
 *
 * @param msg1 the first message to compare
 * @param msg2 the second message to compare
 * @return -1 if the msg1 comes before msg2, 0 if they are equal, 1 if msg1 comes after msg2
 */
int compare_messages(message_t* msg1, message_t* msg2);

// todo: retrieve by message ID #
// each message is now in its own file
// MESSAGE_FILENAME_FORMAT:message%d.txt
/**
 * @brief
 *
 * @param id
 * @return message_t*
 */
message_t* retrieve_msg(int id);

#endif  // MESSAGES_H