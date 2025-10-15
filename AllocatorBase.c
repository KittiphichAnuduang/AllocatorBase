#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AllocatorBase.h"
#include "colors.h"

// Global color toggle (1=enabled, 0=disabled)
int color_enabled = 1;

// ---- Simple Heap Allocator Implementation ----
// Fixed-size heap (4 MB for demonstration) 
#define HEAP_SIZE (4 * 1024 * 1024)
static unsigned char heap[HEAP_SIZE];

// Internal pointer to the first block in the heap
static block_header *heap_base = NULL; // NULL if uninitialized

// ---- Accessors for DebugTool/VM ----
const unsigned char *heap_start(void) { return (const unsigned char*)heap; }// may be NULL if uninitialized
block_header *allocator_first_block(void) { return heap_base; } // may be NULL if uninitialized

// ---- Internal Helper Functions ----
// Split blk into two if it's significantly larger than requested size
static void split_block(block_header *blk, size_t req) {
    // Need enough space for new header + 16 bytes payload
    if (blk->size >= req + sizeof(block_header) + 16) {
        unsigned char *payload = (unsigned char*)blk + sizeof(block_header);
        block_header *new_blk = (block_header*)(payload + req);
        new_blk->size = blk->size - req - sizeof(block_header);
        new_blk->free = 1;
        new_blk->next = blk->next;

        blk->size = req;
        blk->next = new_blk;
    }
}

// Coalesce blk with subsequent free blocks
static void coalesce_forward(block_header *blk) {
    // Merge with next blocks while they are free
    while (blk && blk->next && blk->next->free) {
        blk->size += sizeof(block_header) + blk->next->size;
        blk->next  = blk->next->next;
    }
}

// ---- Public API Functions ----
void heap_init(void) {
    heap_base       = (block_header*)heap;
    heap_base->size = HEAP_SIZE - sizeof(block_header);
    heap_base->free = 1;
    heap_base->next = NULL;

    printf("%s[HEAP INIT]%s Allocator initialized with %zu KB total.\n",
           CLR(BLUE), CLR(RESET), (size_t)(HEAP_SIZE / 1024));
}

// Allocate a block of at least 'size' bytes, or NULL if failed
void *mymalloc(size_t size) {
    if (size == 0) return NULL;

    block_header *cur = heap_base;
    // First-fit search for a free block
    while (cur) {
        if (cur->free && cur->size >= size) {
            split_block(cur, size);
            cur->free = 0;

            printf("%s[ALLOC]%s Request: %zu bytes → Block %p (%s)\n",
                   CLR(GREEN), CLR(RESET), size, (void*)cur,
                   (cur->next && cur->next->free) ? "split" : "full");
            return (unsigned char*)cur + sizeof(block_header);
        }
        cur = cur->next;
    }

    printf("%s[ERROR]%s Out of memory: failed to allocate %zu bytes.\n",
           CLR(RED), CLR(RESET), size);
    return NULL;
}

// Free a previously allocated block
void myfree(void *ptr) {
    // Ignore NULL pointers
    if (!ptr) {
        printf("%s[ERROR]%s Attempted to free NULL pointer.\n", CLR(RED), CLR(RESET));
        return;
    }

    block_header *blk = (block_header*)((unsigned char*)ptr - sizeof(block_header));
    if (blk->free) {
        printf("%s[ERROR]%s Double free detected at %p.\n", CLR(RED), CLR(RESET), (void*)blk);
        return;
    }

    blk->free = 1;
    printf("%s[FREE]%s  Freed Block %p", CLR(CYAN), CLR(RESET), (void*)blk);

    // Coalesce with next if free
    if (blk->next && blk->next->free) {
        coalesce_forward(blk);
        printf(" → %sMerged forward%s", CLR(MAGENTA), CLR(RESET));
    }
    printf("\n");
}

// Allocate and zero-initialize an array
void *mycalloc(size_t num, size_t size) {
    size_t total = num * size;
    void *p = mymalloc(total);
    if (p) memset(p, 0, total);
    return p;
}

// Resize an allocated block
void *myrealloc(void *ptr, size_t new_size) {
    // Handle special cases
    if (!ptr) return mymalloc(new_size);
    if (new_size == 0) {
        myfree(ptr);
        return NULL;
    }

    block_header *blk = (block_header*)((unsigned char*)ptr - sizeof(block_header));

    // If the current block is already large enough
    if (blk->size >= new_size) {
        // Can shrink in place
        split_block(blk, new_size);
        printf("%s[REALLOC]%s Shrink in-place %p → %zu bytes\n",
               CLR(YELLOW), CLR(RESET), (void*)blk, new_size);
        return ptr;
    }

    // Try to grow in place by merging with next free block
    if (blk->next && blk->next->free &&
        (blk->size + sizeof(block_header) + blk->next->size) >= new_size) {

        blk->size += sizeof(block_header) + blk->next->size;
        blk->next  = blk->next->next;
        // Now split if still larger than needed
        split_block(blk, new_size);

        printf("%s[REALLOC]%s Grew in-place %p → %zu bytes (merged next)\n",
               CLR(YELLOW), CLR(RESET), (void*)blk, new_size);
        return (unsigned char*)blk + sizeof(block_header);
    }

    // Need to allocate a new block and copy data over
    void *new_ptr = mymalloc(new_size);
    if (!new_ptr) {
        printf("%s[ERROR]%s Realloc failed for %zu bytes.\n", CLR(RED), CLR(RESET), new_size);
        return NULL;
    }
    memcpy(new_ptr, ptr, blk->size);
    myfree(ptr);
    printf("%s[REALLOC]%s Moved to new block %p → %zu bytes\n",
           CLR(YELLOW), CLR(RESET), new_ptr, new_size);
    return new_ptr;
}

// Print detailed heap state for debugging
void print_heap_state(void) {
    block_header *c = heap_base;
    size_t used = 0, free = 0;
    int i = 0;

    printf("\n===== HEAP STATE =====\n");
    // Header
    printf("Index | Address  |   Size   | Status\n");
    while (c) {
        printf("[%02d] %p | %8zu | %s%-5s%s\n", i, (void*)c, c->size,
               c->free ? CLR(CYAN) : CLR(GREEN),
               c->free ? "FREE" : "USED", CLR(RESET));
        if (c->free) free += c->size; else used += c->size;
        c = c->next;
        i++;
    }
    const size_t total = used + free;
    double pct = (total ? (100.0 * (double)used / (double)total) : 0.0);

    // Colored percentage indicator
    const char *pc_col = CLR(GREEN);
    if      (pct >= 85.0) pc_col = CLR(RED);
    else if (pct >= 60.0) pc_col = CLR(YELLOW);

    // ASCII bar (30 columns)
    int bar_cols = 30;
    int filled   = (total ? (int)((double)bar_cols * (double)used / (double)total) : 0);
    if (filled < 0) filled = 0;
    if (filled > bar_cols) filled = bar_cols;

    printf("------------------------------\n");
    printf("Used: %zu bytes | Free: %zu bytes | Total: %zu KB\n",
           used, free, (size_t)(HEAP_SIZE / 1024));

    printf("Usage: [");
    for (int i2 = 0; i2 < filled; ++i2)  printf("%s#%s", pc_col, CLR(RESET));
    for (int i2 = filled; i2 < bar_cols; ++i2) printf("%s.%s", CLR(DIM), CLR(RESET));
    printf("] %s%5.2f%%%s\n", pc_col, pct, CLR(RESET));
}