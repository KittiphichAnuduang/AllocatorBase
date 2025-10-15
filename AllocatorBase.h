#ifndef ALLOCATOR_BASE_H
#define ALLOCATOR_BASE_H

#include <stddef.h> // size_t

// Version tag for UI and manifest reference
#define ALLOCATOR_VERSION "v1.0"

// Block header structure (placed before each payload)
typedef struct block_header {
    size_t               size; // payload size
    int                  free; // 1=free, 0=used
    struct block_header *next; // next block
} block_header;

// Allocator API
void   heap_init(void);
void  *mymalloc(size_t size);
void  *mycalloc(size_t num, size_t size);
void   myfree(void *ptr);
void  *myrealloc(void *ptr, size_t new_size);
void   print_heap_state(void);

// Accessors for DebugTool/VM
const unsigned char *heap_start(void);
block_header *allocator_first_block(void);

#endif // ALLOCATOR_BASE_H
// === End of AllocatorBase.h ===
