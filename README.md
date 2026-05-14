# Paged Message Cache

A C-based message cache simulator that demonstrates core operating systems concepts: disk-backed storage, in-memory caching, fixed-size data structures, and page replacement policies. (Team project, 2 people)

## Project Overview

This project implements a disk-backed message cache with configurable size and message format. Messages are stored in a fixed-size in-memory cache and persisted to disk. Cache lookups prioritize memory; cache misses load from disk and update the cache. When the cache is full, messages are evicted using one of two replacement policies.

## Core Features

* Fixed-size message records (configurable through compile-time config)
* In-memory cache with disk persistence
* Cache-first message retrieval (check cache, load from disk on miss)
* Two page replacement policies:
  * **LIFO:** Most recently added message is evicted
  * **Random:** Random message is evicted
* Unit tests for messages, cache pages, and cache behavior
* Instrumented evaluation: cache hits, misses, and hit ratio over random accesses

## Cache Components

**Message (`message_t`):** Represents a message with sender, receiver, content, timestamp, and delivery flag.

**Cache Page (`cache_page_t`):** Fixed-size struct holding a message with metadata (occupied flag, sender/receiver/content fields).

**Cache (`cache_t`):** Array of cache pages with metadata tracking (strategy, total pages, pages occupied, hit/miss counts).

**Lookup Strategy:** Linear search through the cache array. For this project scope (configurable cache size, typically dozens of pages), linear search is appropriate and straightforward. A hash table would add complexity without proportional benefit.

## Configuration

Cache behavior is controlled at compile time through `src/config.h`:

* `MAX_MESSAGE_SIZE` — fixed size of each cached message
* `CACHE_SIZE` — maximum number of messages in cache
* Message field sizes (sender, receiver, content)

Modify these values and recompile to test different cache configurations.


## Concepts Demonstrated

* Memory hierarchy and caching
* Paging and page replacement algorithms
* Fixed-size data structures in C
* Disk I/O and persistence
* Struct-based data organization
* Unit testing in C
* Makefile builds
* Cache performance metrics

## Testing

The project includes unit tests for:

* **Message (`test_msg.c`):** Message creation, serialization, and field integrity
* **Cache Page (`test_page.c`):** Page occupation, data storage, and field access
* **Cache (`test_cache.c`):** Insert/retrieve operations, both replacement policies, cache full behavior

Run `make test_cache` (or `test_msg`/`test_page`) to execute tests and verify correct behavior.

## Evaluation

The evaluation driver (`main.c`) performs random message accesses against the cache and measures:

* Number of cache hits
* Number of cache misses
* Hit ratio (hits / total accesses)

Metrics are calculated separately for **LIFO** and **Random** replacement policies to compare their performance on the same access pattern.


## Build and Run

**Evaluate cache performance:**

```
make main
```

Runs the evaluation using 10,000 random message accesses (default).

```
make main ACCESSES=5000
```

Run evaluation with custom access count.

**Run unit tests:**

```
make test_msg       # Test message creation and storage
make test_page      # Test cache page behavior
make test_cache     # Test cache operations and replacement
```

**Clean built files:**

```
make clean
```

## Project Structure

```
.
├── src/
│   ├── cache.c/.h          # Cache implementation, page replacement
│   ├── message.c/.h        # Message creation, serialization
│   ├── disk.c/.h           # Disk I/O helpers
│   ├── config.h            # Configuration constants
│   └── main.c              # Evaluation driver
├── tests/
│   ├── test_msg.c          # Message unit tests
│   ├── test_page.c         # Cache page unit tests
│   └── test_cache.c        # Cache behavior unit tests
├── data/                   # Disk-backed message storage
├── Makefile
└── README.md
```
