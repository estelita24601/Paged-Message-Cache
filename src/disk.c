
#define _XOPEN_SOURCE 700

#include "disk.h"

/**
 * @brief use the message id number to create the properly formatted filename for this message
 *
 * @param msg_id int - the id of the message to be used in the filename
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

/**
 * @brief use the message id to either find the message in disk
 *
 * @param id int - the message id used to find the message in disk
 * @return message_t* - return the message found from the disk
 */
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

    fclose(msg_file);
    free(expected_filename);
    return msg;
}

/**
 * @brief write the provided message to disk
 *
 * @param msg message_t* - the message to write to disk
 * @return bool - return true if the message was written to disk, false otherwise
 */
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
            // make sure csv string isn't too large to write to disk
            if (strlen(msg_csv) > MAX_CSV_LENGTH) {
                printf("WARNING: message csv is too long! some message content will be lost\n");
                msg_csv[MAX_CSV_LENGTH] = '\0';
            }

            fprintf(file, "%s", msg_csv);
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
