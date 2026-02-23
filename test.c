#include <stdio.h>
#include "table.h"

int main() {
    table_t t;
    table_init(&t, 2);

    *table_insert(&t, "hello", 5) = 10;
    *table_insert(&t, "world", 5) = 12;
    printf("%d, %d\n", *table_lookup(&t, "hello", 5), *table_lookup(&t, "world", 5));
    *table_insert(&t, "resize!", 7) = 13;
    printf("%d, %d, %d\n", *table_lookup(&t, "hello", 5), *table_lookup(&t, "world", 5), *table_lookup(&t, "resize!", 7));
}
