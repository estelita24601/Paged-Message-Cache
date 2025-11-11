# Practicum I
## Instructions:
1. Export files/folders.
2. Access Linux environment (bash, wsl, etc.).
3. Navigate / cd to exported files / folders.
4. Go to "src/config.h" to manually configure the size of message, number of messages that are cached, or other relevant configurations.
5. Run commands below inside the directory of the Makefile to execute based on the desired make commands:
  - `make main` - will run the evaluation which is the calculation of the cache metrics for each page replacement algorithm
  - `make test_cache` - will run the tests for the cache and display the cache creation, execution of page replacement policy, and other relevant cache functions.
  - `make test_page` - will run the tests for the cache pages and display the page creation, page replacement, and other relevant page functions.
  - `make test_msg` - will run the tests for the messages and display the message creation, storing and retreiving messages, and other relevant message functions.
6. To clean environment (remove files) run commane `make clean`


## References:
[^1]  
[^2]  
[^3]  
[^1]: "Self-balancing binary search tree." Wikipedia, 11 Nov 2025. https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree. Accessed: 2025-11-06. 
[^2]: "AVL Tree Data Structure." GeeksforGeeks, 11 Oct 2025. https://www.geeksforgeeks.org/dsa/introduction-to-avl-tree/. Accessed: 2025-11-07. 
[^3]: "Introduction to Red-Black Tree." GeeksforGeeks, 03 Nov 2025. https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/. Accessed: 2025-11-07. 


- <https://www.programiz.com/c-programming/c-unions>
- <https://www.geeksforgeeks.org/c/c-unions/>
- <https://thelinuxcode.com/unions-in-c/>
- <https://coderivers.org/c/c-basic/c-union/>

- https://stackoverflow.com/questions/25152905/what-is-type-punning-how-type-punning-works-with-unions-in-c
- https://www.geeksforgeeks.org/c/how-to-declare-struct-member-inside-union-in-c/