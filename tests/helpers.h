#include "../src/message.h"

/**
 * @brief prints if a test was a success or failure
 *
 * @param result bool - result of the test
 * @param message char * - describes the test that was run
 */
void PRINT_TEST_RESULTS(bool result, char* message);

/**
 * @brief Prints a comparison between expected and actual test results
 *
 * @param expected The expected result string
 * @param actual The actual result string
 */
void PRINT_COMPARISON(const char* expected, const char* actual);

/**
 * @brief
 *
 * @param expected_msg
 * @param actual_msg
 */
void PRINT_COMPARE_MESSAGES(message_t* expected_msg, message_t* actual_msg);

/**
 * @brief Prints a test section header with a given title
 *
 * @param str The title of the test section
 */
void PRINT_HEADER(char* str);

/**
 * @brief Prints a test subsection header
 *
 * @param str The title of the test subsection
 */
void PRINT_SUBHEADER(char* str);