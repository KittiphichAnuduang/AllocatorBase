#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AllocatorBase.h"
#include "DebugTool.h"
#include "colors.h"

// Simple function to fill a block with a pattern for demo purposes
static void touch(char *p, size_t n, char v) {
    if (!p) return;
    for (size_t i = 0; i < n; ++i) p[i] = (char)(v + (int)(i & 0xF));
}

void vm_load_test_basic(void) {
    printf("%s[RUN]%s Basic demo start\n", CLR(WHITE), CLR(RESET));

    char *a = (char*)mymalloc(64);
    char *b = (char*)mymalloc(128);
    touch(a, 64, 'A'); touch(b,128,'B');
    dbg_print_heap_map();

    myfree(a);
    dbg_print_heap_map();

    myfree(b);
    dbg_print_heap_map();

    printf("%s[DONE]%s Basic demo end\n", CLR(GREEN), CLR(RESET));
}

void vm_load_test_realloc(void) {
    printf("%s[RUN]%s Realloc demo start\n", CLR(WHITE), CLR(RESET));

    char *p = (char*)mymalloc(64);
    strcpy(p, "allocator test");
    dbg_print_heap_map();

    p = (char*)myrealloc(p, 128);
    dbg_print_heap_map();

    p = (char*)myrealloc(p, 32);
    dbg_print_heap_map();

    myfree(p);
    dbg_print_heap_map();

    printf("%s[DONE]%s Realloc demo end\n", CLR(GREEN), CLR(RESET));
}

void vm_load_test_frag(void) {
    printf("%s[RUN]%s Fragmentation test start\n", CLR(WHITE), CLR(RESET));

    void *p[10];
    for (int i = 0; i < 10; ++i) p[i] = mymalloc(256);
    for (int i = 0; i < 10; i += 2) myfree(p[i]); // free even indices
    dbg_print_heap_map();

    // Allocate a block that requires coalescing
    void *q = mymalloc(1024);
    (void)q;
    dbg_print_heap_map();

    // Cleanup
    for (int i = 1; i < 10; i += 2) myfree(p[i]);
    myfree(q);
    dbg_print_heap_map();

    printf("%s[DONE]%s Fragmentation test end\n", CLR(GREEN), CLR(RESET));
}

void vm_load_test_stress(void) {
    printf("%s[RUN]%s Stress test start\n", CLR(WHITE), CLR(RESET));

    enum { N = 200 };
    void *arr[N] = {0};

    for (int i = 0; i < N; ++i) {
        size_t sz = (rand() % 512) + 16;
        if (rand() % 2) {
            // allocate at random
            if (!arr[i]) arr[i] = mymalloc(sz);
        } else {
            // free a random existing allocation
            int j = rand() % (i + 1);
            if (arr[j]) { myfree(arr[j]); arr[j] = NULL; }
        }
    }
    dbg_print_heap_map();

    // Cleanup
    for (int i = 0; i < N; ++i) {
        if (arr[i]) { myfree(arr[i]); arr[i] = NULL; }
    }
    dbg_print_heap_map();

    printf("%s[DONE]%s Stress test end\n", CLR(GREEN), CLR(RESET));
}
