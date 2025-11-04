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

#include "cache.h"
#include "config.h"
#include "messages.h"

int main(int argc, char* argv[]) {
    // test printing only
    printf("cache size = %d messages\nmessage size = %d bytes\n", CACHE_SIZE, MESSAGE_SIZE);

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