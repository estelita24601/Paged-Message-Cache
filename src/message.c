/**
 * @file message.c / source code for Messages.
 * @authors Estelita Chen & Lori Kim / CS5600 / Northeastern University
 * @brief
 * @date Nov 11, 2025 / Fall 2025
 *
 */

#define _XOPEN_SOURCE 700

#include "message.h"

/**
 * @brief Gets the next available message ID from the file
 *
 * @return int The next available message ID
 */
int get_next_id() {
    int next_id = -1;
    // get ID from the file
    FILE* id_file = fopen(NEXT_ID_PATH, "r");
    if (id_file) {
        fscanf(id_file, "%d", &next_id);
        fclose(id_file);
    } else {
        next_id = 1;  // initialize first id number
    }

    return next_id;
}

bool replace_next_id(int new_id){
	FILE* id_file = fopen(NEXT_ID_PATH, "w");
    if (id_file) {
        fprintf(id_file, "%d", new_id);
        fclose(id_file);
		return true;
    } else {
        printf("WARNING: unable to save ID to file\n");
		return false;
    }
}

/**
 * @brief updates the next available message ID
 *
 * increments the current ID read from a file and saves it back to the file.
 *
 */
void increment_next_id() {
    int next_id = get_next_id();
	replace_next_id(next_id + 1);
}

void decrement_next_id(){
	int next_id = get_next_id();
    replace_next_id(next_id - 1);
}

/**
 * @brief Creates a message object from its constituent parts
 *
 * @param id The unique identifier for the message
 * @param sender The name of the message sender
 * @param receiver The name of the message receiver
 * @param content The message content (if NULL or empty, will be set to "N/A")
 * @param time_sent The timestamp when the message was sent
 * @param sent_flag Whether the message has been delivered
 * @return message_t* A newly allocated message object, or NULL if sender/receiver are invalid
 */
message_t* create_msg_from_parts(int id, const char* sender, const char* receiver, const char* content,
                                 const time_t time_sent, bool sent_flag) {
    // make sure id number is valid
    if (id <= 0) {
        return NULL;
    }

    // first do null checks
    if (sender == NULL || receiver == NULL) {
        return NULL;
    }
    // make sure sender/receiver aren't empty strings
    else if (strlen(sender) == 0 || strlen(receiver) == 0) {
        printf("WARNING: message sender/receiver are not allowed to be empty!\n");
        return NULL;
    }
    // allow "empty" content but fill it in with something
    if (content == NULL || strlen(content) == 0) {
        content = "N/A";
    }

    message_t* msg = malloc(sizeof(message_t));

    msg->id = id;
    msg->sent_time = time_sent;
    msg->sender = strdup(sender);
    msg->receiver = strdup(receiver);
    msg->sent_flag = sent_flag;
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
message_t* create_msg(const char* sender, const char* receiver, const char* content) {
    time_t now;
    time(&now);
    int id = get_next_id();
    increment_next_id();
    bool sent_flag = false;

    return create_msg_from_parts(id, sender, receiver, content, now, sent_flag);
}

/**
 * @brief Frees the memory allocated for a message struct
 *
 * @param msg the message struct to free
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

/**
 *
 * @param msg
 * @return
 */
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
    struct tm* tm = localtime(&msg->sent_time);
    char time[TIME_FORMAT_LEN + 1];
    strftime(time, TIME_FORMAT_LEN + 1, TIME_FORMAT, tm);

    // NEW EXPECTED FORMAT: id,sender,receiver,time_sent,delivered_flag,content
    sprintf(csv_str, "%d,%s,%s,%s,%d,%s", msg->id, msg->sender, msg->receiver, time, msg->sent_flag, msg->content);

    return csv_str;
}

/**
 * @brief Returns a string value representing the message. Note: need to free the returned string after use.
 *
 * @param message the message to convert to a string
 * @return a string representation of the message
 */
char* message_to_pretty_str(message_t* message) {
    if (message == NULL) {
        return NULL;
    }

    char* str = (char*) malloc(MAX_CSV_LENGTH * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "ERROR: dynamic memory was not able to be allocated");
        exit(1);
    }
    char* string_timeSent = ctime(&message->sent_time);
    string_timeSent[strcspn(string_timeSent, "\n")] = '\0';
    sprintf(str, "{id: %d, sender: %s, receiver: %s, content: %s, sent_time: %s}", message->id, message->sender,
            message->receiver, message->content, string_timeSent);
    // sprintf(str, "{sender: %s, receiver: %s, content: %s, id: %d}", message->sender, message->receiver,
    // message->content, message->id);
    return str;
}

/**
 * @brief store a message element to the disk AND to the cache
 * if this message already exists in either then that message is replaced with this one
 *
 * @param msg message_t* - pointer to the message element
 * @param cache cache_t* - pointer to the cache we should store element inside of
 * @return true on success
 * @return false on failure
 */
bool store_msg(message_t* msg, cache_t* cache) {
    if (cache == NULL) {
        printf("WARNING: tried to store a message to a cache that does not exist\n");
        return false;
    }

    if (msg == NULL) {
        printf("WARNING: tried to store a NULL message object\n");
        return false;
    }

    // try to write to the file no matter what
    bool disk_success = disk_write(msg);

    // only write to cache if it exists
    bool cache_success;
    if (cache == NULL) {
        cache_success = false;
    } else {
        cache_success = cache_add(cache, msg);
    }

    if (cache_success && disk_success) {
        return true;
    } else {
        if (cache_success == false && disk_success == false) {
            printf("WARNING: unable to store message #%d in the cache or in the disk\n", msg->id);
        } else if (cache_success) {
            printf("WARNING: message #%d was stored to cache but NOT stored in the disk\n", msg->id);
        } else {
            printf("WARNING: message #%d was stored in the disk but NOT in the cache\n", msg->id);
        }
        return false;
    }
}

/**
 * @brief first searches the cache for a message with the given ID and then searches the disk store
 * if message was not in the cache this function adds it to the cache
 *
 * @param id int - unique identifier for the message we want to retrieve
 * @param cache cache_t* - pointer to cache we'll search for the message
 * @return message_t* - message that we found
 * @return NULL - if message couldn't be found in the cache OR in the disk store
 */
message_t* retrieve_msg(int id, cache_t* cache) {
    if (cache == NULL) {
        printf("ERROR: tried to retrieve message from a NULL cache\n");
        exit(1);
    }

    if (id < 0) {
        printf("WARNING: cannot find page, invalid id\n");
        return NULL;
    }

    cache_page_t* cache_page = cache_find(cache, id);
    if (cache_page != NULL) {
        // is a cache hit if the cache is full and cache_page does not return NULL
        if (cache->pages_occupied == cache->total_pages) {
            cache->hits += 1;
        }

        message_t* msg = create_msg_from_page(cache_page);
        if (msg == NULL) {
            printf("WARNING: found message in the cache but could not turn it into a message object\n");
        }
        return msg;
    }

    // else is a cache miss if cache is full and cache_page returns NULL
    if (cache->pages_occupied == cache->total_pages) {
        cache->miss += 1;
    }

    // implied else: message isn't in the cache but might be in the disk
    message_t* msg = disk_find(id);

    if (msg != NULL) {
        // now need to add this to the cache
        if (cache_add(cache, msg) == true) {
            return msg;
        }
        printf("ERROR: found message in the disk but wasn't able to add it to the cache\n");
        exit(1);
    }

    return msg;  // aka return NULL
}
