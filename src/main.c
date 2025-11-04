 /**
 * @file main.c / source code for Practicum I.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* CONFIG_FILE_PATH = "../config.txt";
int CACHE_SIZE = 0;
int MESSAGE_SIZE = 0;

/**
 * @brief - configure size of the cache and size of messages in the cache by reading them in from ../config.txt
 *
 * @return int - status code (0 for success and -1 for failure)
 */
int configure_sizes() {
    FILE* config_file = fopen(CONFIG_FILE_PATH, "r");
    if (config_file == NULL) {
        printf("ERROR: unable to open %s\n", CONFIG_FILE_PATH);
        return -1;
    }

    char* buffer = malloc(sizeof(char) * 100);
    while (fgets(buffer, 100, config_file) != NULL) {
        // remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // config file expected to be formatted as "key:value"
        char* key = strtok(buffer, ":");
        char* value = strtok(NULL, ":");

        if (strcmp(key, "CACHE_SIZE") == 0) {
            CACHE_SIZE = atoi(value);
        } else if (strcmp(key, "MESSAGE_SIZE") == 0) {
            MESSAGE_SIZE = atoi(value);
        }
    }
    free(buffer);
    fclose(config_file);

    // double check we were able to read valid sizes from config file
    if (CACHE_SIZE <= 0 || MESSAGE_SIZE <= 0) {
        printf("ERROR: unable to properly initialize cache size and/or message size\n");
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // read in config file and load global variables from it
    if (configure_sizes() == -1) {
        return -1;  // end program if we can't finish set up
    } else {
        // test printing only
        printf("cache size = %d messages\nmessage size = %d bytes\n", CACHE_SIZE, MESSAGE_SIZE);
    }

    // initialize empty cache
    // initialize empty AVL tree

    // fill cache with...
    //  existing messages retrieved from disk
    //  new messages that also need to be stored on disk

    // search cache for...
    //  messages present in the cache
    //  messages present only in the disk
    //  messages present in neither cache nor disk

    // test LIFO replacement
    // test random replacement
}