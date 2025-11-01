/*
* bst.h / header file for BST.
*
* Lori Kim / CS5600 / Northeastern University
*
* Fall 2025 / Sep 28, 2025
*
*/


#ifndef BST_H
#define BST_H

#define POST_ORDER 0
#define IN_ORDER 1
#define PRE_ORDER 2
#define BREADTH_FIRST 3



// forward declaration of message_t
typedef struct message_t message_t;

/**
 * @brief Represents a BST node.
 */
typedef struct bstnode_t {
    message_t * message; // message_t - a message element
    struct bstnode_t * left; // bstnode_t - left child of node
    struct bstnode_t * right; // bstnode_t - right child of node
} bstnode_t;

/**
 * @brief Represents a BST.
 */
typedef struct bst_t {
    bstnode_t * root; // bstnode_t - root node of the BST
    int size;         // int - size of the BST
} bst_t;


/**
 * @brief Creates a new BST. 
 * 
 * @return a pointer to the new BST
*/
bst_t * new_bst();


/**
 * @brief Frees the given BST and all of its nodes. 
 * 
 * @param bst bst_t* - the BST to free
*/
void free_bst(bst_t * bst);

/**
 * @brief Frees the given BST and all of its nodes contents. 
 * 
 * @param bst bst_t* - the BST to free
*/
void clear_and_free_bst(bst_t * bst);

/**
 * @brief Adds the given message into the BST. 
 * Handles the root case, but then calls the recursive helper
 * for anything after the root. 
 * 
 * Increments the size of the BST once a message is added.
 * 
 * @param bst bst_t* - the BST to insert into
 * @param msg message_t* - the message to insert
*/
void bst_add(bst_t * bst, message_t * msg);


/**
 * @brief Removes the given message from the BST. 
 * 
 * @param bst bst_t* - the BST to remove from
 * @param msg message_t* - the message to remove
*/
void bst_remove(bst_t * bst, message_t * msg);

 /**
  * @brief Finds the given message from the BST. 
  * 
  * Only returns the first message with that id match - unique ids.
  *
  * @param bst bst_t* - the BST to get from
  * @param id int - the id of the message to get
  * @return message_t* - the message that was found
 */
message_t * bst_find(bst_t * bst, int id);

 /**
  * @brief Converts the given BST to a string using the given traversal method.
  *
  * @param tree bst_t* - the BST to convert
  * @param traversal int - the traversal method to use
  * @return char* - the string that was appended to
 */
char* bst_to_str(bst_t * bst, int ordering);

 /**
  * @brief Converts the given BST to a sorted array.
  * 
  * @param tree bst_t* - the BST to convert
  * @return message_t** - the array that was created
 */
message_t ** bst_to_sorted_array(bst_t * bst);


#endif // BST_H