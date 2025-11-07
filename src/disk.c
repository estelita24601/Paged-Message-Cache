#include "disk.h"
#define _XOPEN_SOURCE 700
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief use the id number to create the properly formatted filename for this message
 *
 * @param msg_id - int
 * @return char* - filename for this message ON THE HEAP
 */
char* create_filename(int msg_id) {
    char* filename = NULL;
    if (msg_id == 0) {  // log10(0) = -inf
        filename = malloc(sizeof(char) * (strlen(MESSAGE_FILENAME_FORMAT) + msg_id));
    } else {
        filename = malloc(sizeof(char) * (strlen(MESSAGE_FILENAME_FORMAT) + log10(msg_id) +
                                          2));  // TODO: finalize a cap size for the id number
    }
    sprintf(filename, MESSAGE_FILENAME_FORMAT, msg_id);
    return filename;
}

message_t* disk_find(int id) {
    char* expected_filename = create_filename(id);

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

bool disk_write(message_t* msg) {
    if (msg == NULL) {
        printf("ERROR: tried to store a NULL message\n");
        return false;
    }

    char* expected_filename = create_filename(msg->id);
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
