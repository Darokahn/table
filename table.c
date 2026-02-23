#include <stdlib.h>
#include <sys/param.h>
#include <string.h>
#include <stdio.h>

#include "table.h"

int TABLE_NULL_BACKING;

int* TABLE_NULL = &TABLE_NULL_BACKING;

void table_init(table_t* new, int initialSize) {
    new->capacity = initialSize;
    new->entries = malloc(new->capacity * sizeof *new->entries);
    memset(new->entries, 0, new->capacity * sizeof *new->entries);
    new->count = 0;
}

void table_destroy(table_t* t) {
    free(t->entries);
    *t = (table_t){0};
}

char GRAVESTONE;

int getIntKey(const char* key, int length) {
    int intKey;
    memcpy(&intKey, key, MIN(length, sizeof intKey));
    return intKey;
}

// returns the first entry which matches by value or the first entry which contains NULL
struct table_entry* linearSearch(table_t* t, int startingIndex, const char* string, unsigned int length) {
    int steps;
    int i;
    for (i = startingIndex, steps = 0; steps < t->capacity; i = (i + 1) % t->capacity, steps++) {
        if (t->entries[i].signature == NULL) return t->entries + i;
        if (length != t->entries[i].sigLength) continue;
        if (strncmp(t->entries[i].signature, string, length) != 0) continue;
        return t->entries + i;
    }
    return NULL;
}

void table_resize(table_t* t, int newSize);

int* table_lookup(table_t* t, const char* string, unsigned int length) {
    int startingIndex = getIntKey(string, length);
    table_entry* entry = linearSearch(t, startingIndex % t->capacity, string, length);
    if (entry->signature == NULL) {
        *TABLE_NULL = 0;
        return TABLE_NULL;
    }
    return &(entry->payload);
}

int* table_insert(table_t* t, const char* string, unsigned int length) {
    int startingIndex = getIntKey(string, length);
    if (t->count == t->capacity) {
        table_resize(t, 0);
    }
    table_entry* entry = linearSearch(t, startingIndex % t->capacity, string, length);
    if (entry->signature != NULL) return &(entry->payload);
    entry->signature = string;
    entry->sigLength = length;
    t->count++;
    return &(entry->payload);
}

bool table_delete(table_t* t, const char* string, unsigned int length) {
    int startingIndex = getIntKey(string, length);
    table_entry* entry = linearSearch(t, startingIndex % t->capacity, string, length);
    if (entry->signature == NULL) return false;
    entry->signature = &GRAVESTONE;
    entry->sigLength = 0;
    t->count--;
    return entry->payload;
}

void table_resize(table_t* t, int newSize) {
    if (newSize == 0) newSize = t->capacity * 2;
    table_t new;
    table_init(&new, newSize);
    printf("got here!\n");
    fflush(stdout);
    for (int i = 0; i < t->capacity; i++) {
        printf("got here!\n");
        fflush(stdout);
        struct table_entry ent = t->entries[i];
        if (ent.signature == NULL) continue;
        *table_insert(&new, ent.signature, ent.sigLength) = ent.payload;
    }
    table_destroy(t);
    *t = new;
}
