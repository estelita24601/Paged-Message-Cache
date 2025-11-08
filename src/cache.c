/**
 * @file cache.c / source code for Cache.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#include "cache.h"
#include "message.h"

/**
 * @brief Create and initialize a cache object on the heap where its members are contiguous blocks of memory.
 *
 * @return cache_t* - return the initialized cache object
 */
cache_t* create_cache(replacement_strategy strategy) {
    cache_t* cache = (cache_t*) calloc(1, sizeof(cache_t));
    if (cache == NULL) {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }

    // set replacement strategy that this cache will use
    cache->strategy = strategy;

    // get these values from config.h
    cache->page_size_bytes = MAX_MESSAGE_SIZE;
    cache->total_pages = CACHE_SIZE;

    // initialize values for an empty unused cache
    cache->pages_occupied = 0;
    cache->last_added = -1;
    cache->total_accesses = 0;
    cache->hits = 0;
    cache->miss = 0;

    // initialize empty cache pages
    for (int i = 0; i < cache->total_pages; i++) {
        cache->page_array[i] = init_page();
    }

    return cache;
}

/**
 * @brief - Add a message to the cache. If the cache is full then use the replacement strategy specified.
 *
 * @param cache cache_t* - the pointer to the cache object to add a message to
 * @param msg message_t* - the pointer to the message object to add to the cache
 * @return bool - return true if a message has been successfully added into the cache otherwise false
 */
bool cache_add(cache_t* cache, message_t* msg) {
    if (cache == NULL) {
        printf("WARNING: trying to add to a cache that doesn't exist\n");
        return false;
    }

    if (msg == NULL) {
        printf("WARNING: message is NULL\n");
        return false;
    }

    // check if this message is already in the cache
    cache_page_t* found_page = cache_find(cache, msg->id);

    // update the message already in the cache
    if (found_page != NULL) {
        return replace_page(found_page, msg);
    }

    // if cache is full then need to replace a page
    if (cache->pages_occupied == cache->total_pages) {
        cache_page_t* page_to_replace;

        switch (cache->strategy) {
            case LIFO:
                page_to_replace = cache->page_array[cache->last_added];
                return replace_page(page_to_replace, msg);
            case RANDOM:
                srand(time(NULL));  // Seed with current time
                int random_index = rand() % cache->total_pages;
                page_to_replace = cache->page_array[random_index];
                return replace_page(page_to_replace, msg);
            default:
                printf("ERROR: cache does not have a replacement strategy\n");
                exit(1);
        }
    }
    // implied else: don't need to replace a page just fill an empty page
    int nextPage_index = cache->last_added + 1;
    set_page(cache->page_array[nextPage_index], msg);

    // update cache meta data
    cache->pages_occupied += 1;
    cache->last_added = nextPage_index;

    return true;
}

/**
 * @brief - use linear search to find a message in the cache
 *
 * @param cache cach_t* - cache we're searching
 * @param id int - id of the message we want to find
 * @return cache_page_t* - found cache page or NULL if not found
 */
cache_page_t* cache_find(cache_t* cache, int id) {
    if (id < 0) {
        printf("WARNING: cannot find page, invalid id\n");
        return NULL;
    }

    if (cache == NULL) {
        printf("WARNING: cannot find page, cache doesn't exist\n");
        return NULL;
    }

    int curr_msg_id = -1;
    for (int i = 0; i < cache->pages_occupied; i++) {
        curr_msg_id = cache->page_array[i]->id;
        if (curr_msg_id == id) {
            return cache->page_array[i];
        }
    }

    return NULL;
}

/**
 * @brief free the page_array inside the cache and the cache object itself
 *
 * @param cache cache_t* - the pointer to the cache object
 */
void free_cache(cache_t* cache) {
    if (cache == NULL) {
        fprintf(stderr, "ERROR: cache is NULL");
        exit(1);
    }

    for (int i = 0; i < cache->total_pages; i++) {
        free_page(cache->page_array[i]);
    }

    free(cache);
}

/**
 * @brief prints the cache object metadata
 *
 * @param cache cache_t* - the pointer to the cache object
 */
void print_cache_metadata(cache_t* cache) {
    if (cache == NULL) {
        printf("WARNING: trying to print a cache that doesn't exist\n");
        return;
    }
    printf("Total Pages = %d\n", cache->total_pages);
    printf("Pages Occupied = %d/%d\n", cache->pages_occupied, cache->total_pages);
    printf("Page Size = %d bytes\n", cache->page_size_bytes);
    printf("Index of Last Added Page = %d\n", cache->last_added);
    printf("Total Accesses = %d\n", cache->total_accesses);
    printf("Cache Hits = %d\n", cache->hits);
    printf("Cache Misses = %d\n", cache->miss);
}

/**
 * @brief prints the cache object's member values
 *
 * @param cache cache_t* - the pointer to the cache object
 */
void print_cache_contents(cache_t* cache) {
    if (cache == NULL) {
        printf("WARNING: trying to print a cache that doesn't exist\n");
        return;
    }

    printf("Page Contents = [\n");

    for (int i = 0; i < cache->total_pages; i++) {
        printf("\t");
        cache_page_t* curr = cache->page_array[i];
        print_page_metadata(curr);

        if (i != cache->total_pages - 1) {
            printf(",\n");
        } else {
            printf("\n]\n");
        }
    }
}

/**
 * @brief - initialize empty cache page on the heap
 *
 * @return cache_page_t*
 */
cache_page_t* init_page() {
    cache_page_t* new_page = malloc(sizeof(cache_page_t));
    if (new_page == NULL) {
        fprintf(stderr, "ERROR: memory was not able to be allocated\n");
        exit(1);
    }
    new_page->occupied = false;
    new_page->id = -1;

    return new_page;
}

/**
 * @brief - sets the initialized cache page with the provided message
 *
 * @param page cache_page_t* - the pointer to the cache page object
 * @param msg message_t* - the pointer to the message object
 *
 * @return bool - true if page is set otherwise false
 */
bool set_page(cache_page_t* page, const message_t* msg) {  // if set_page doesn't fill the page do we want the program
                                                           // to gracefully return to the main program or crash?
    // make sure objects aren't null
    if (page == NULL) {
        printf("WARNING: trying to fill page that doesn't exist\n");
        return false;
    } else if (msg == NULL) {
        printf("WARNING: trying to fill page with a NULL message object\n");
        return false;
    }
    // make sure strings inside of message aren't null
    if (msg->sender == NULL || msg->receiver == NULL || msg->content == NULL) {
        printf("WARNING: trying to fill page with a message object that contains NULL string fields\n");
        return false;
    }

    // make sure page isn't already filled
    if (page->occupied) {
        printf("WARNING: trying to fill a page that is occupied!\n");
        return false;
    }

    // update page status
    page->occupied = true;

    // copy message values over
    page->id = msg->id;
    page->sent_time = msg->sent_time;
    page->sent_flag = msg->sent_flag;

    // copy strings over
    strncpy(page->sender, msg->sender, MAX_SENDER_SIZE - 1);
    strncpy(page->receiver, msg->receiver, MAX_RECEIVER_SIZE - 1);
    strncpy(page->content, msg->content, MAX_CONTENT_SIZE - 1);

    // make sure strings are null terminated in case msg strings were truncated
    page->sender[MAX_SENDER_SIZE - 1] = '\0';
    page->receiver[MAX_RECEIVER_SIZE - 1] = '\0';
    page->content[MAX_CONTENT_SIZE - 1] = '\0';

    return true;
}

/**
 * @brief - clears the cache page to prepare to set a new message input
 *
 * @param page cache_page_t* - the pointer to the cache page object
 *
 * @return bool - true if page is cleared otherwise false
 */
bool clear_page(cache_page_t* page) {
    if (page == NULL) {
        printf("WARNING: trying to clear page that doesn't exist\n");
        return false;
    }
    // update page status
    page->occupied = false;

    // reset values and empty strings
    page->id = -1;
    page->sent_time = -1;
    page->sent_flag = false;
    page->sender[0] = '\0';
    page->receiver[0] = '\0';
    page->content[0] = '\0';

    return true;
}

/**
 *
 * @param page cache_page_t* - pointer to the page whose contents we are going to replace
 * @param msg message_t* - message we're placing in the page
 * @return true - on success
 * @return false - if unable to replace contents of the page
 */
bool replace_page(cache_page_t* page, const message_t* msg) {
    if (page == NULL) {
        printf("ERROR: trying to replace page that doesn't exist\n");
        exit(1);
    }
    if (msg == NULL) {
        printf("WARNING: trying to replace page with a NULL message object\n");
        return false;
    }

    clear_page(page);
    return set_page(page, msg);
}

/**
 * @brief - Creates a new message object with the provided cache page.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 * @return message_t*
 */
message_t* create_msg_from_page(const cache_page_t* page) {
    if (page == NULL) {
        printf("WARNING: trying to create message from a page that doesn't exist\n");
        return NULL;
    } else if (page->occupied == false) {
        printf("WARNING: tried to create a message from a page that is unoccupied\n");
        return NULL;
    }

    message_t* msg =
        create_msg_from_parts(page->id, page->sender, page->receiver, page->content, page->sent_time, page->sent_flag);

    return msg;
}

/**
 * @brief - Prints the members of the input cache page.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 */
void print_page(cache_page_t* page) {
    if (page == NULL) {
        printf("WARNING: trying to print page that doesn't exist\n");
        return;
    }

    char* boolStr;
    if (page->occupied) {
        boolStr = "true";
    } else {
        boolStr = "false";
    }
    printf("page occupied = %s\n", boolStr);
    printf("page id = %d\n", page->id);
    printf("page sender = '%s'\n", page->sender);
    printf("page receiver = '%s'\n", page->receiver);
    printf("page content = '%s'\n", page->content);
    // todo: maybe add printing for sent time and sent flag
}

/**
 * @brief - Prints the cache page metadata.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 */
void print_page_metadata(cache_page_t* page) {
    if (page == NULL) {
        printf("WARNING: trying to print page that doesn't exist\n");
        return;
    }
    if (page->occupied) {
        printf("{msg_id: %d}", page->id);
    } else {
        printf("{msg_id: NaN}");
    }
}

/**
 * @brief - Frees the memory allocated for a cache page.
 *
 * @param page cache_page_t* - the pointer to the cache page object
 */
void free_page(cache_page_t* page) {
    if (page == NULL) {
        return;
    }
    free(page);
}
