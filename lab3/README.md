# Hash Hash Hash
In this lab, I modified a hash table implementation to be thread-safe for concurrent use. For version 1 (v1), I implemented a global mutex to ensure correctness. In version 2 (v2), I added mutexes to individual hash table entries to enhance performance while maintaining thread safety.

## Building
```shell
make
```

## Running
To run the program, use the ./hash-table-tester executable with two command line arguments: -t to specify the number of threads (default is 4) and -s to specify the number of hash table entries to add per thread (default is 25,000).
```shell
./hash-table-tester -t 8 -s 50000
Generation: 72,938 usec
Hash table base: 1,308,185 usec
  - 0 missing
Hash table v1: 1,692,567 usec
  - 0 missing
Hash table v2: 414,950 usec
  - 0 missing
```

## First Implementation
In the `hash_table_v1_add_entry` function, I added a global mutex lock to ensure that only one thread can access and modify the hash table at a time. This prevents race conditions and maintains data consistency. While this approach guarantees correctness, it introduces performance overhead due to thread contention, as multiple threads must wait for the mutex to be available before proceeding.

### Performance
Low Threads:
```shell
./hash-table-tester -t 4 -s 50000
Generation: 36,993 usec
Hash table base: 220,392 usec
  - 0 missing
Hash table v1: 489,396 usec
  - 0 missing
Hash table v2: 64,990 usec
  - 0 missing
```
High Threads:
```shell
./hash-table-tester -t 8 -s 50000
Generation: 73,907 usec
Hash table base: 1,313,972 usec
  - 0 missing
Hash table v1: 1,940,504 usec
  - 0 missing
Hash table v2: 503,545 usec
  - 0 missing
```
Version 1 is slower than the base version because it introduces a global mutex to ensure thread safety, which adds overhead. As multiple threads attempt to add entries to the hash table, they must acquire and release the mutex, causing contention and blocking. This serialization of access to the hash table results in decreased performance compared to the base version, which does not have any locking overhead and allows all operations to proceed without waiting. The locking mechanism ensures correctness but at the cost of reduced speed.

## Second Implementation
In the `hash_table_v2_add_entry` function, I used a finer-grained locking approach by adding a mutex to each hash table entry. This allows multiple threads to operate on different entries concurrently, reducing contention and improving performance. Each entry's mutex ensures that only one thread can modify that specific entry at a time, maintaining data consistency while allowing higher parallelism.

### Performance
Low Threads:
```shell
./hash-table-tester -t 4 -s 50000
Generation: 36,993 usec
Hash table base: 220,392 usec
  - 0 missing
Hash table v1: 489,396 usec
  - 0 missing
Hash table v2: 64,990 usec
  - 0 missing
```
High Threads:
```shell
./hash-table-tester -t 8 -s 50000
Generation: 73,907 usec
Hash table base: 1,313,972 usec
  - 0 missing
Hash table v1: 1,940,504 usec
  - 0 missing
Hash table v2: 503,545 usec
  - 0 missing
```
Higher Number of Items w/Higher Threads:
```shell
./hash-table-tester -t 8 -s 200000
Generation: 294,904 usec
Hash table base: 36,476,921 usec
  - 0 missing
Hash table v1: 41,326,713 usec
  - 0 missing
Hash table v2: 11,870,339 usec
  - 0 missing
```
Lower Number of Items w/Lower Threads:
```shell
./hash-table-tester -t 4 -s 10000
Generation: 7,484 usec
Hash table base: 9,592 usec
  - 0 missing
Hash table v1: 29,273 usec
  - 0 missing
Hash table v2: 3,725 usec
  - 0 missing
```
Version 2 outperforms both the base case and version 1 by a significant margin, averaging around 75% faster than the base case and 80% faster than version 1 across different thread and item counts. This improvement is due to the finer-grained locking approach, which allows multiple threads to modify different hash table entries concurrently, reducing contention. Despite the performance boost, version 2 maintains the same accuracy as the base case and version 1, with no missing entries in all test scenarios.

## Cleaning up
```shell
make clean
```