/**
 * @file message.h
 * @author Estelita Chen
 * @brief header file for the message library
 * @date 2025-10-24
 *
 */

#include <stdbool.h>
#include <time.h>

#ifndef MESSAGE_H

#define MESSAGE_H

#define STORE_PATH "message_store/messages.csv"

typedef struct message {
    int id;
    time_t time_sent;
    char* sender;
    char* receiver;
    char* content;
    bool delivered_flag;
} message_t;

/**
 * @brief creates a new message with the fields appropriately set and returns a dynamically allocated message "object"
 *
 * @param sender
 * @param receiver
 * @param content
 * @return message_t*
 */
message_t* create_msg(char* sender, char* receiver, char* content);

/**
 * @brief Frees all memory associated with a message object
 *
 * @param msg The message object to free. If NULL, function returns without doing anything.
 */
void free_msg(message_t* msg);

/**
 * @brief Stores the message in a message store on disk
 *
 * If a message with the same ID already exists, it will be updated.
 * Otherwise, a new message will be appended to the store.
 *
 * @param msg Pointer to the message struct we want to store
 * @return true If the message was successfully stored
 * @return false If unable to store the message (NULL msg or file error)
 */
bool store_msg(message_t* msg);

/**
 * @brief Replaces an existing message in the store with a new version
 *
 * @param new_msg The new message to store (must have valid ID)
 * @param id The ID of the message to replace (must match new_msg->id)
 * @return true If the message was successfully replaced
 * @return false If any error occurred during replacement
 */
bool replace_msg(message_t* new_msg, int id);

/**
 * @brief Finds and returns a message identified by its ID
 *
 * @param id The unique identifier of the message to retrieve
 * @return message_t* The found message or NULL if not found/error
 */
message_t* retrieve_msg(int id);

/**
 * @brief Gets the next available message ID and updates the counter
 *
 * Reads the current ID from a file, increments it, saves it back,
 * and returns the original value.
 *
 * @param id_counter_path Path to the file storing the ID counter
 * @return int The next available message ID
 */
int get_next_id(const char* id_counter_path);

/**
 * @brief Converts a message object to its CSV string representation
 *
 * Format: id,sender,receiver,time_sent,delivered_flag,content
 *
 * @param msg The message to convert
 * @return char* Newly allocated string with CSV representation, or NULL if msg is NULL
 */
char* msg_to_str(message_t* msg);

/**
 * @brief Creates a message object from its CSV string representation
 *
 * @param csv_str The CSV string to parse (must contain at least 6 fields)
 * @return message_t* A new message object or NULL if string is invalid
 */
message_t* create_msg_from_str(const char* csv_str);

/**
 * @brief Compares two messages for equality
 *
 * Messages are considered equal if they have matching sender, receiver,
 * content, and delivered_flag. The ID and timestamp are not compared.
 *
 * @param msgA First message to compare
 * @param msgB Second message to compare
 * @return true If messages are equal
 * @return false If messages differ or either is NULL
 */
bool msg_compare(message_t* msgA, message_t* msgB);

#endif