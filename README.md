# todo

- maybe rewrite `message_to_str` so its a helper message for `store_msg` instead?
- make message_t being on stack vs. heap more consistent
  - message_t from `create_msg` doesn't need to be freed
  - and message_t from `create_empty_msg` sometimes needs to be freed
  - so how do we prevent someone from trying to free stack memory with `free_message` ?
- use `get_next_id` function when creating new message_t objects
- `retrieve_msg`
  - use `append_with_comma` to deal with edge case of deliminator in the message contents
  - don't need to create and destroy BST inside of here anymore
  - maybe separate out helper function of `create_msg_from_string`
- maybe move over some tests from estelita_version
- `bst.c`
  - maybe remove unused functions
  - rotation helper functions
    - right left
    - right right
    - left left
    - left right
  - update add and remove functions to rotate nodes in order to keep bst balanced
- maybe rename data folder as our "message store" or something that matches the assignment more?
- config file stuff
  - do we want all stuff in one config file in the top and then `main.c` and `message.c` all read from the same config file?
  - or config file only for stuff `main.c` needs and then a separate one for the stuff that `message.c` needs and so forth?

# references

- <https://www.programiz.com/c-programming/c-unions>
- <https://www.geeksforgeeks.org/c/c-unions/>
- <https://thelinuxcode.com/unions-in-c/>
- <https://coderivers.org/c/c-basic/c-union/>
