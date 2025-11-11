# Practicum I
## Instructions:
1. Export files / folders.
2. Access Linux environment (bash, wsl, etc.).
3. Navigate / cd to exported files / folders.
4. Go to "src/config.h" to manually configure the size of message, number of messages that are cached, or other relevant configurations.
5. Run commands below inside the directory of the Makefile to execute based on the desired make commands:
  - `make main` - will run the evaluation which is the calculation of the cache metrics for each page replacement algorithm with an optional argument or a default of 10,000 random message accesses.
    - Ex: `make main 500` - will run the evaluation with an optional argument of 500 random message accesses.
  - `make test_cache` - will run the tests for the cache and display the cache creation, execution of page replacement policy, and other relevant cache functions.
  - `make test_page` - will run the tests for the cache pages and display the page creation, page replacement, and other relevant page functions.
  - `make test_msg` - will run the tests for the messages and display the message creation, storing and retreiving messages, and other relevant message functions.
6. To clean environment (remove files) run commane `make clean`


## References:
[^1]  
[^2]  
[^3]  
[^4]  
[^5]  
[^6]  
[^7]  
[^8]  
[^9]   

[^1]: "Self-balancing binary search tree." Wikipedia, 11 Nov 2025. https://en.wikipedia.org/wiki/Self-balancing_binary_search_tree. Accessed: 2025-11-05.  
[^2]: "AVL Tree Data Structure." GeeksforGeeks, 11 Oct 2025. https://www.geeksforgeeks.org/dsa/introduction-to-avl-tree/. Accessed: 2025-11-05.  
[^3]: "Introduction to Red-Black Tree." GeeksforGeeks, 03 Nov 2025. https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/. Accessed: 2025-11-05.  
[^4]: "C Unions." Programiz. https://www.programiz.com/c-programming/c-unions. Accessed: 2025-11-06.  
[^5]: "Unions in C." GeeksforGeeks, 25 Oct 2025. https://www.geeksforgeeks.org/c/c-unions/. Accessed: 2025-11-06.  
[^6]: "Unions in C: A Deep Dive." TheLinuxCode, 27 Dec 2023. https://thelinuxcode.com/unions-in-c/. Accessed: 2025-11-07.  
[^7]: "Exploring C Unions: Concepts, Usage, and Best Practices." CodeRivers. https://coderivers.org/c/c-basic/c-union/. Accessed: 2025-11-07.  
[^8]: "What is type punning? how type punning works with unions in C?." StackOverFlow, 06 Mar 2019. https://stackoverflow.com/questions/25152905/what-is-type-punning-how-type-punning-works-with-unions-in-c. Accessed: 2025-11-08.  
[^9]: "How to Declare a Struct Member Inside a Union in C?." GeeksforGeeks, 23 Jul 2025. https://www.geeksforgeeks.org/c/how-to-declare-struct-member-inside-union-in-c/. Accessed: 2025-11-08.  