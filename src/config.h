#define CACHE_SIZE 16          // how many messages in the cache
#define MAX_MESSAGE_SIZE 1024  // how many bytes for each message in the cache

// calculate space for one page of the cache
#define NON_STRINGS_SIZE (sizeof(int) + sizeof(time_t) + 2 * sizeof(bool))
#define STRING_SIZE (MAX_MESSAGE_SIZE - NON_STRINGS_SIZE)
#define MAX_SENDER_SIZE 64    // low priority todo: or STRING_SIZE/8 if smaller
#define MAX_RECEIVER_SIZE 64  // low priority todo: or STRING_SIZE/8 if smaller
#define MAX_CONTENT_SIZE (STRING_SIZE - MAX_SENDER_SIZE - MAX_RECEIVER_SIZE)

// globals for how to save messages to the disk as a CSV string
#define MESSAGE_FILENAME_FORMAT "data/message%d.txt"  // where %d is the message id number - is in relation to cwd of where the program starts so the Makefile
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define TIME_FORMAT_LEN 19  // low priority todo: make this not hardcoded anymore?

// how many characters in the csv string for the message
#define MAX_CSV_LENGTH (TIME_FORMAT_LEN + MAX_SENDER_SIZE + MAX_RECEIVER_SIZE + MAX_CONTENT_SIZE + 21)
/*
csv format: id,sender,receiver,time_sent,delivered_flag,content
21 <-- (10 digit max id number) + (1 character delivered_flag) + (5 commas) + (5 extra chars for padding/safety)
*/

#define NEXT_ID_PATH "../data/_NEXT_ID.txt"  // file that tells us the next un-used id number