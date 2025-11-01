/*
 * bst.c / source code for BST.
 *
 * Lori Kim / CS5600 / Northeastern University
 *
 * Fall 2025 / Sep 28, 2025
 *
 */

#include "bst.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "messages.h"

/**
 * @brief Creates a new BSTNode with the given message.
 *
 * @param msg message_t* - the message to store in the node
 * @return bstnode_t* - a pointer to the new node
 */
bstnode_t* __bst__new_node(message_t* msg) {
    bstnode_t* node = malloc(sizeof(bstnode_t));
    node->message = msg;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * @brief Creates a new BST.
 *
 * @return a pointer to the new BST
 */
bst_t* new_bst() {
    bst_t* bst = malloc(sizeof(bst_t));
    bst->root = NULL;
    bst->size = 0;
    return bst;
}

/**
 * @brief Frees the given node and all of its children.
 * This helps with the recursive nature of freeing a BST.
 *
 * @param node bstnode_t* - the node to free
 * @param clear bool - whether to free the node object (message) contents
 */
void __bst__free_node(bstnode_t* node, bool clear) {
    if (node == NULL) {
        return;
    }
    // Is this, pre, post, or in order traversal?
    // This is an in order traversal
    __bst__free_node(node->left, clear);
    __bst__free_node(node->right, clear);
    if (clear) {
        free_message(node->message);
    }
    free(node->message);
    free(node);
}

/**
 * @brief Frees the given BST and all of its nodes.
 *
 * @param bst bst_t* - the BST to free
 */
void free_bst(bst_t* bst) {
    __bst__free_node(bst->root, false);
    free(bst);
}

/**
 * @brief Frees the given BST and all of its nodes contents.
 *
 * @param bst bst_t* - the BST to free
 */
void clear_and_free_bst(bst_t* bst) {
    __bst__free_node(bst->root, true);
    bst->root = NULL;
    free(bst);
}

/**
 * @brief Add helper for adding recursively to BST
 *
 * will keep calling recursively until it finds the correct spot to add the message.
 * Makes use of __bst_new_node(message) and compare_messages(message, curr->message)
 *
 * @param curr bstnode_t* - the current node
 * @param msg message_t* - the message to add
 */
void binary_search_add(bstnode_t* curr, message_t* msg) {
    int cmp = compare_messages(msg, curr->message);

    if (cmp < 0) {
        if (curr->left == NULL) {
            curr->left = __bst__new_node(msg);
            return;
        }
        binary_search_add(curr->left, msg);
    } else if (cmp > 0) {
        if (curr->right == NULL) {
            curr->right = __bst__new_node(msg);
            return;
        }
        binary_search_add(curr->right, msg);
    }
}

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
void bst_add(bst_t* bst, message_t* msg) {
    if (bst->root == NULL) {
        bst->root = __bst__new_node(msg);
        bst->size++;
        return;
    }
    bstnode_t* curr = bst->root;
    binary_search_add(curr, msg);
    bst->size++;
    // todo: AVL rebalance tree
}

/**
 * @brief Helper function for removing a node from the BST.
 *
 * @param curr bstnode_t* - the current node
 * @param msg message_t* - the message to remove
 * @return bstnode_t* - the new root/parent node
 */
bstnode_t* __bst__remove(bstnode_t* curr, message_t* msg) {
    if (curr == NULL) {
        return NULL;
    }
    if (compare_messages(msg, curr->message) == 0) {
        if (curr->left == NULL && curr->right == NULL) {
            free(curr);
            return NULL;
        }
        if (curr->left == NULL) {
            bstnode_t* right = curr->right;
            free(curr);
            return right;
        }
        if (curr->right == NULL) {
            bstnode_t* left = curr->left;
            free(curr);
            return left;
        }
        bstnode_t* min = curr->right;
        while (min->left != NULL) {
            min = min->left;
        }
        curr->message = min->message;
        curr->right = __bst__remove(curr->right, min->message);
        return curr;
    } else if (compare_messages(msg, curr->message) < 0) {
        curr->left = __bst__remove(curr->left, msg);
    } else {
        curr->right = __bst__remove(curr->right, msg);
    }
    return curr;
}

/**
 * @brief Removes the given message from the BST.
 *
 * @param bst bst_t* - the BST to remove from
 * @param msg message_t* - the message to remove
 */
void bst_remove(bst_t* bst, message_t* msg) {
    if (bst->root == NULL || msg == NULL) {
        return;
    }
    bstnode_t* returned = __bst__remove(bst->root, msg);
    bst->root = returned;  // incase the root needs updated, often will be the same.
    bst->size--;
    // todo: AVL Balance tree
}

/**
 * @brief Helper function for getting a node from the BST.
 * Only returns the first message with that id match - unique ids.
 *
 * cmp = id - curr->message->id; IF id search is less than curr id, go left, greater go right, equal return curr
 *
 * @param curr bstnode_t* - the current node
 * @param id int - the id of the message to get
 * @return bstnode_t* - the node that was found
 */
bstnode_t* __bst__find(bstnode_t* curr, int id) {
    if (curr == NULL) {
        return NULL;
    }

    int cmp;
    while (curr != NULL) {
        cmp = id - curr->message->id;
        if (cmp < 0) {
            curr = curr->left;
        } else if (cmp > 0) {
            curr = curr->right;
        } else {
            return curr;
        }
    }

    return NULL;
}

/**
 * @brief Finds the given message from the BST.
 *
 * Only returns the first message with that id match - unique ids.
 *
 * @param bst bst_t* - the BST to get from
 * @param id int - the id of the message to get
 * @return message_t* - the message that was found
 */
message_t* bst_find(bst_t* bst, int id) {
    bstnode_t* node = __bst__find(bst->root, id);
    if (node == NULL) {
        return NULL;
    }
    return node->message;
}

////////////////////////////////////////////
// todo maybe remove all functions below? not needed for this program

/**
 * @brief function that updates the given string with the given message.
 *
 * This function should be called in based on the order of traversal,
 *
 * for example with inorder it may be
 * inorder(...) (with dots being the parameters)
 * __bst__update_str(curr->message, str);
 * inorder(...)
 *
 * @param message message_t* - the message to add to the string
 * @param str char* - the string to append to
 */
char* __bst__update_str(message_t* message, char* str) {
    char* msg_str;
    if (message == NULL) {
        msg_str = (char*) "NULL";  // used by BREATH_FIRST to print null nodes, others ignore this.
    } else {
        msg_str = message_to_str(message);
    }
    str = realloc(str, sizeof(char) * (strlen(str) + strlen(msg_str) + 5));
    if (strlen(str) > 0) {
        strcat(str, ", ");
    }
    strcat(str, msg_str);
    if (message != NULL) {
        free(msg_str);  // since "NULL" is on the stack, we don't need to free directly
    }
    return str;
}

/**
 * @brief Post order traversal helper function for converting a BST to a string.
 * Similar to printing from the team activity, but instead of printf, you will be calling __bst__update_str
 *
 * if (curr == NULL)  - just return the str, don't add NULL to the string
 *
 * @param curr bstnode_t* - the current node
 * @param str char* - the string to append to
 * @return char* - the string that was appended to
 */
char* __bst__to_str_postorder(bstnode_t* curr, char* str) {
    if (curr == NULL) {
        return str;
    }

    str = __bst__to_str_postorder(curr->left, str);
    str = __bst__to_str_postorder(curr->right, str);
    str = __bst__update_str(curr->message, str);
    return str;
}

/**
 * @brief Pre order traversal helper function for converting a BST to a string.
 * Similar to printing from the team activity, but instead of printf, you will be calling __bst__update_str
 *
 * if (curr == NULL)  - just return the str, don't add NULL to the string
 *
 * @param curr bstnode_t* - the current node
 * @param str char* - the string to append to
 * @return char* - the string that was appended to
 *
 */
char* __bst__to_str_preorder(bstnode_t* curr, char* str) {
    if (curr == NULL) {
        return str;
    }

    str = __bst__update_str(curr->message, str);
    str = __bst__to_str_preorder(curr->left, str);
    str = __bst__to_str_preorder(curr->right, str);

    return str;
}

/**
 * @brief In order traversal helper function for converting a BST to a string.
 *
 * Similar to printing from the team activity, but instead of printf, you will be calling __bst__update_str
 *
 * if (curr == NULL)  - just return the str, don't add NULL to the string
 *
 * @param curr bstnode_t* - the current node
 * @param str char* - the string to append to
 * @return char* - the string that was appended to
 */
char* __bst__to_str_inorder(bstnode_t* curr, char* str) {
    if (curr == NULL) {
        return str;
    }

    str = __bst__to_str_inorder(curr->left, str);
    str = __bst__update_str(curr->message, str);
    str = __bst__to_str_inorder(curr->right, str);

    return str;
}

/**
 * @brief Helper function for converting a BST to a string using breadth first traversal.
 *
 * Unlike post/pre/in order traversal, this is not recursive. It also does print NULL
 * nodes, as we want to print the structure of the tree to help us with debugging.
 *
 * @param tree bst_t* - the BST to convert
 * @param str char* - the string to append to
 * @return char* - the string that was appended to
 */
char* __bst__to_str_breadth_first(bst_t* tree, char* str) {
    if (tree->root == NULL) {
        return str;
    }
    bstnode_t** queue = malloc(sizeof(bstnode_t) * tree->size + 1);
    queue[0] = tree->root;
    int front = 0;
    int back = 1;
    while (front < back) {
        bstnode_t* node = queue[front++];
        if (node == NULL) {  // as we still want to print the null nodes
            str = __bst__update_str(NULL, str);
            continue;
        }
        str = __bst__update_str(node->message, str);
        queue[back++] = node->left;
        queue[back++] = node->right;
    }
    free(queue);
    return str;
}

/**
 * @brief Converts the given BST to a string using the given traversal method.
 *
 * @param tree bst_t* - the BST to convert
 * @param traversal int - the traversal method to use
 * @return char* - the string that was appended to
 */
char* bst_to_str(bst_t* tree, int traversal) {
    char* str = malloc(sizeof(char));
    str[0] = '\0';
    switch (traversal) {
        case PRE_ORDER:
            return __bst__to_str_preorder(tree->root, str);
        case IN_ORDER:
            return __bst__to_str_inorder(tree->root, str);
        case POST_ORDER:
            return __bst__to_str_postorder(tree->root, str);
        case BREADTH_FIRST:
            return __bst__to_str_breadth_first(tree, str);
        default:
            return str;
    }
}

/**
 * @brief Helper function for converting a BST to a sorted array.
 *
 * a major question is what type of traversal do you need to
 * take a tree and sort it from smallest to largest?
 *
 * Once you figure out the type of traversal, you can update the
 * array and index parameters to help store the message. For example
 *
 * array[*index] = curr->message;
 * (*index)++;
 *
 * It is good to draw this out with some simple trees (and actually very similar
 * to a common leetcode questions)
 *
 * @param curr bstnode_t* - the current node
 * @param array message_t** - the array to store the messages in, you can safely assume it is the correct size
 * @param index int* - the current index in the array, used as a pointer to be updated across all recursive calls
 *
 */
void __bst__to_sorted_array(bstnode_t* curr, message_t** array, int* index) {
    if (curr == NULL) {
        return;
    }

    __bst__to_sorted_array(curr->left, array, index);

    array[*index] = curr->message;
    (*index)++;

    __bst__to_sorted_array(curr->right, array, index);
}

/**
 * @brief Converts the given BST to a sorted array.
 *
 * @param tree the BST to convert
 * @return the array that was created
 */
message_t** bst_to_sorted_array(bst_t* tree) {
    message_t** array = malloc(sizeof(message_t*) * tree->size);
    int index = 0;
    __bst__to_sorted_array(tree->root, array, &index);
    return array;
}
