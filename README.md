# Paged Message Cache

A C-based disk-backed message cache simulator that demonstrates core operating systems concepts, including hierarchical memory, fixed-size pages, cache lookup, and page replacement policies.

This project was built for a systems programming midterm focused on simulating how data can be cached in memory while also being persisted to disk. Messages are stored in a configurable in-memory cache and written to disk for long-term storage. When a message is requested, the program checks the cache first, then loads from disk on a cache miss.

## Features

- Fixed-size message records stored in memory and on disk
- Configurable cache size and message size through `src/config.h`
- Disk-backed message storage
- Cache-first message retrieval
- Lookup structures for finding cached messages efficiently
- Page replacement algorithms:
  - Random Replacement
  - LIFO Replacement
- Unit tests for:
  - Messages
  - Cache pages
  - Cache behavior
- Cache performance evaluation using random message accesses

## Concepts Demonstrated

- C programming
- Memory hierarchy simulation
- Cache design
- Page replacement algorithms
- Disk I/O
- Fixed-size data structures
- Unit testing
- Makefile-based builds
- Systems-level documentation


## Evaluation

The project instruments cache behavior and reports metrics for both supported replacement policies:

- Cache hits
- Cache misses
- Cache hit ratio

Metrics are calculated over a configurable number of random message accesses.


## Configuration

Before running the project, settings can be adjusted in `src/config.h`

This file controls values such as:

- Message size
- Number of messages cached
- Page/cache configuration
- Other relevant project constants

## Build and Run

Run all commands from the directory containing the `Makefile`.

### Run cache evaluation

```bash
make main
```

By default, this runs the cache evaluation using 10,000 random message accesses.

You can also provide a custom number of accesses:

```bash
make main 500
```

This runs the evaluation using 500 random message accesses.

### Run tests

Test cache behavior:

```bash
make test_cache
```

Test cache page behavior:

```bash
make test_page
```

Test message creation, storage, and retrieval:

```bash
make test_msg
```

### Clean generated files

```bash
make clean
```


## Project Structure

```text
.
├── src/              # Main C source and header files
├── tests/            # Unit tests for messages, pages, and cache behavior
├── data/             # Disk-backed message storage files
├── Makefile          # Build, test, run, and clean commands
├── Instructions.md   # Original project instructions
└── README.md         # Project overview and usage
```

### Key implementation files:

```text
src/cache.c/.h        # Cache implementation and replacement policies
src/message.c/.h      # Message creation, serialization, and retrieval logic
src/disk.c/.h         # Disk-backed storage helpers
src/config.h          # Cache/message configuration values
src/main.c            # Evaluation driver for cache metrics
tests/*.c             # Unit tests
```


## References

1. “Self-balancing binary search tree.” Wikipedia.
2. “AVL Tree Data Structure.” GeeksforGeeks.
3. “Introduction to Red-Black Tree.” GeeksforGeeks.
4. “C Unions.” Programiz.
5. “Unions in C.” GeeksforGeeks.
6. “Unions in C: A Deep Dive.” TheLinuxCode.
7. “Exploring C Unions: Concepts, Usage, and Best Practices.” CodeRivers.
8. “What is type punning? How type punning works with unions in C?” Stack Overflow.
9. “How to Declare a Struct Member Inside a Union in C?” GeeksforGeeks.
