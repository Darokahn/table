#pragma once

// table_t is a linear search table which converts strings to integer keys to start searching at an informed point.
// It has an intentionally bare and memory-vulnerable API for simplicity, control, and efficiency.
// It implicitly places trust onto the programmer to understand memory contracts and use them properly.

// table_t does not own keys after they are used in insertion. The caller inserting via a key and then mutating or freeing that key will cause incorrect or undefined behavior.
// Either use static keys or store the table in association with a key-copy object.

#include <stdint.h>
#include <stdbool.h>

struct table_entry {
    const char* signature;
    int payload;
    unsigned int sigLength;
};

typedef struct table_entry table_entry;

struct table_t {
    table_entry* entries;
    unsigned int count;
    unsigned int capacity;
};

typedef struct table_t table_t;

// The address of this value is used as the sentinel for lookups that find no existing key.
// Returning it rather than `NULL` makes unconditional dereference safe.
extern int* TABLE_NULL;

// The pointer returned by table_lookup should be dereferenced inside the expression table_lookup is called in, or stored for just long enough to check against TABLE_NULL.
// They can be used to modify or capture the value that is indexed by the given key.
// This is kind of fragile if you use it wrong. Follow the rule "The pointer returned should be dereferenced in the same expression, or stored temporarily to check TABLE_NULL"
int* table_lookup(table_t* t, const char* string, unsigned int length);
// returns pointer to new or existing slot.
int* table_insert(table_t* t, const char* string, unsigned int length);
// returns prior value
bool table_delete(table_t* t, const char* string, unsigned int length);
// places initial values into `new`
void table_init(table_t* new, int initialSize);
// frees array inside `t` and zeroes its members.
void table_destroy(table_t* t);
