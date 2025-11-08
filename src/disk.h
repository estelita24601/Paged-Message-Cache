#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "message.h"


message_t* disk_find(int id);

bool disk_write(message_t* msg);

/**
 * @brief use the id number to create the properly formatted filename for this message
 *
 * @param msg_id - int
 * @return char* - filename for this message ON THE HEAP
 */
char* create_filename(int msg_id);

#endif
