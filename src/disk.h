#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "message.h"

/**
 * @brief use the message id number to create the properly formatted filename for this message
 *
 * @param msg_id - int
 * @return char* - filename for this message ON THE HEAP
 */
char* create_filename(int msg_id);

/**
 * @brief use the message id to either find the message in disk or in the cache
 *
 * @param id int - the message id used to find the message in disk
 * @return message_t* - the message found from the disk
 */
message_t* disk_find(int id);

/**
 * @brief write the provided message to disk
 *
 * @param msg message_t* - the message to write to disk
 * @return bool - true if the message was written to disk, false otherwise
 */
bool disk_write(message_t* msg);

#endif
