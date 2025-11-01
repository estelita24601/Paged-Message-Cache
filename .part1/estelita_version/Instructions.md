# Motivation

Virtual memory provides memory for processes above the limit of physical memory in a computer by swapping unused pages to a secondary storage device, such as a disk (mechanical or solid-state). This type of hierarchical memory is also used for CPU instruction caching and in database systems. In this assignment, you will explore some aspects of constructing hierarchical memory structures by building a simulation.

# Hierarchical Memory

In the context of computer systems, the term "memory hierarchy" refers to the organization and structure of various types of memory storage devices within a computer, arranged in a hierarchy based on their proximity to the CPU (Central Processing Unit) and their speed, capacity, and cost characteristics. Memory hierarchy is a crucial concept in computer architecture and plays a vital role in determining the overall performance of a computer system.

The memory hierarchy typically consists of several levels, each with different properties:

**1. Registers:** These are the fastest and smallest storage units directly accessible by the CPU. Registers store data that the CPU is currently working on. Data can be read from or written to registers in a single clock cycle. Registers provide the highest performance but have the smallest capacity.

**2. Cache Memory:** Cache memory is a small-sized, high-speed memory that sits between the CPU and main memory (RAM). It stores frequently used data and instructions to reduce the time it takes to access them. Cache memory is divided into multiple levels (L1, L2, L3), with L1 being the smallest and fastest, and L3 being larger but slightly slower. Caches use a mechanism called caching to manage data movement between levels.

**3. Main Memory (RAM):** Main memory is the primary storage used to store data and programs that the CPU is actively using. While it is larger in capacity compared to cache memory, it is slower in terms of access times. Data must be moved between main memory and cache when needed. The CPUs memory management unit (MMU) does this automatically.

**4. Secondary Storage:** This includes non-volatile storage devices like hard disk drives (HDDs), solid-state drives (SSDs), and optical drives. Secondary storage provides much larger storage capacity but is significantly slower in terms of access times compared to main memory. Data from secondary storage must be loaded into main memory before the CPU can work on it. The virtual memory subsystem manages the movement of pages from secondary storage to main memory.

**5. Tertiary Storage:** This level includes slower and high-capacity storage solutions, often used for long-term archival and backup purposes. Examples include magnetic tapes and cloud storage.

The memory hierarchy is designed to optimize the trade-off between speed and capacity. Data that is frequently accessed is kept closer to the CPU in faster memory levels (e.g., registers and cache), while less frequently used data is stored in slower, higher-capacity memory levels (e.g., main memory and secondary storage). This organization aims to minimize data access times and maximize overall system performance while balancing cost.

Efficient management of the memory hierarchy is a critical aspect of computer architecture, and memory hierarchies are designed to ensure that the CPU can access data and instructions for processing as quickly as possible while maintaining a balance between cost and performance.

## Questions

* is time sent just when the message is created..?
* by store on disc can we just save in a csv file..?

# Tasks

The tasks below are a simulation and explore hierarchical memory, page faults, page swapping, and page replacement algorithms. The tasks below explore only some aspects of the simulation. Subsequent assignments explore additional details. The assignments are intentionally decomposed to allow for flexibility and level the effort across all assignments in this course.

You will implement a cache mechanism for a message-oriented data store. Messages need to be retrieved rapidly but due to the volume of messages not all of them will fit into main memory and need to be stored in secondary memory (on disk through the file system).

You may set up the source code in any way you deem reasonable, although we do expect that you use reasonable software engineering practices, follow naming conventions, appropriately split code into source files and use header files as necessary. All code must be written in C.

* [x] (15 min / 5 pts) Set up a data structure for a "message". The message must minimally contain:
  * a unique identifier,
  * time sent,
  * the sender (right now that is just some text),
  * a receiver (again, for now, just some text),
  * content (text),
  * a flag indicating whether the message was delivered.

* [x] (20 min / 10 pts) Write a function `create_msg()` that creates a new message with the fields appropriately set and returns a dynamically allocated message "object".
  * Define arguments and return types as necessary.
  * *You may discuss the design of the function signature and behavior (but not the implementation) with your peers.*

* [x] (90 min / 35 pts) Write a function `store_msg()` that stores the message in a message store on disk.
  * The design of the message store is up to you, but you should consider how you can quickly retrieve messages from disk when needed.
  * Define arguments and return types as necessary and appropriate.
  * *You may discuss the design of the function signature and behavior (but not the implementation) with your peers.*

* [x] (90 min / 30 pts) Write a function `retrieve_msg()` that finds and returns a message identified by its identifier.
  * *You may discuss the design of the function signature and behavior (but not the implementation) with your peers.*

* [x] (60 min / 20 pts) Test your code and demonstrate that all functions work as expected.
  * Consider edge cases and ensure that errors are handled properly.
  * Make sure that memory is managed properly and that your program does not have memory leaks.

# Submission

A .tar file with all of your code and a makefile. Properly commented code and instructions on how to run the code and recognize that it functions as required.
