**Format**: *You may work in pairs or individually. 
If you wish to work in pairs, inform the instructor by sending a message
 on Teams that includes your and your collaborator's names. We will then
 form a group in Canvas. If you start working in a pair and the other 
person does not sufficiently contribute, you may dissolve the pair, take
 any code developed up to that point, and finish individually, but you 
may not form another pair. The members of the pair may not necessarily 
receive the same grade if they did not equally contribute. All code must
 be co-developed; you may not simply split the practicum work among the 
members of the pair.*

**Estimated Time: 15-20 hours  
Due Date**: November 14  
**Late Submission**: accepted (*with penalty*) until November 23  
**Programming Language**: C/C++ or Java on Linux

### Background

This practicum further explores memory hierarchies, page replacement,
 and page replacement algorithms by continuing with the work from the 
assignment [Memory Hierarchy Simulation - Part I](https://northeastern.instructure.com/courses/225963/assignments/2831532?wrap=1 "[4 hrs] Assignment / Memory Hierarchy Simulation - Part I"). Copy the assignment's code and modify as necessary and required. You do not have to complete the questions in sequence.

### Tasks and Requirements

#### Part 1 (30 Points) / Caching

* [ ] add a cache so that some number of messages are stored in a paged structure 
  in memory
  
  * in either a file per message stored in a folder, or a block per message in a single file
  
  * every message that is sent is stored in the cache and *also* written to disk
  
  * retrieve message should always look in the cache first. if not in the cache then load from disk and save in cache

- [ ] messages must be a fixed size (power of 2)
  
  * size includes "house keeping" such as identification of the sender and the receiver, time 
    sent, and perhaps priority, whether it is encrypted, etc.

- [ ] number of messages in the cache and size of the message must be configurable
  
  * through global variables in an include file 
  
  * OR
  
  * through configuration file stored on disk that is read upon startup

- [ ] create appropriate lookup data structures for finding messages in the cache
  
  - [ ] thoroughly describe chosen data structure design and strategy
  
  - [ ] explain why it was chosen over other alternative designs



#### Part 2 (30 Points) / Page Replacement

If a message is placed into the cache but no "slot" for a message is 
open in the cache, then an existing message in the cache must be 
replaced. Devise and then implement two page (a message in our case) 
replacement algorithms:

* [ ] Random Replacement: a replacement "page" is chosen at random
* [ ] LIFO: most recently used "page" is replaced



#### Part 3 (20 Points) / Testing

How would you test that the cache mechanism works as intended? 

* [ ] Devise one or more testing strategies and thoroughly test your cache and page 
  replacement. 

You may discuss your testing approach with all of your 
colleagues and post messages to Teams and discuss on Teams, but you may 
not share code.

#### Part 4 (10 Points) / Evaluation

* [ ] Instrument your code and calculate the following cache metrics for each page replacement algorithm:
1. number of cache hits per 1000 random message accesses
2. number of cache misses per 1000 random message accesses
3. cache hit ratio per 1000 random message accesses 
* [ ] You need to create a representative message set for testing. 

You may share your testing approach, test message mix, and metrics with your 
colleagues on Teams and discuss. You may discuss the calculation 
algorithms and formulas and share test messages but not code.

#### Part 5 (10 Points) / Quality

The remaining points are awarded based on:

* quality

* documentation

* thoroughness

* professionalism of your code

### Simplifying Assumptions, Hints, and Tips

1. You do not need to be concerned about internal fragmentation within messages.
2. You may store message in blocks rather than individually.
3. Additional hints and clarifications will be posted on Teams and 
   added here, so re-read these instructions daily until the due date.

---

### Submission

* [ ] Complete the self-evaluation rubric item in the Practicum I module ([Complete Self-Evaluation Rubric](https://northeastern.instructure.com/courses/225963/assignments/2831529 "[30 min] DO: Complete Self-Evaluation Rubric (Practicum I Fall 2023)"))
* [ ] Submit a *.tar* file containing all of your code, notes, 
  explanations, metrics, and test cases. You may embed everything in your 
  code or write a separate report
* [ ] Schedule time for your review and attend as scheduled
* [ ] One submission per pair is sufficient
