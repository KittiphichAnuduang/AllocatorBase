#include <stdio.h>
#include "AllocatorBase.h"  // come before DebugTool.h so block_header is known
#include "DebugTool.h"
#include "colors.h"

// Print a visual map of the heap state to the console (50 columns wide)
void dbg_print_heap_map(void) {
    block_header *cur = allocator_first_block();

    // Calculate total and used sizes
    size_t total = 0, used = 0;
    for (block_header *t = cur; t; t = t->next) {
        total += t->size;
        if (!t->free) used += t->size;
    }
    if (total == 0) total = 1; // avoid div0
        printf("\nHeap Map:\n[");

    // Render blocks proportionally in 50 columns
    size_t cols = 50;
    size_t painted = 0;
    for (; cur; cur = cur->next) {
        size_t seg = (cur->size * cols) / total;
        if (seg == 0) seg = 1; // at least one column
        if (painted + seg > cols) seg = cols - painted; // don't overflow
        painted += seg;

        const char *col = cur->free ? CLR(DIM) : CLR(GREEN);
        char sym = cur->free ? '.' : '#';

        // Special cases
        if (cur->free && cur->next && cur->next->free) { col = CLR(MAGENTA); sym = 'C'; }
        else if (!cur->free && cur->size > 8192)       { col = CLR(YELLOW);  sym = 'L'; }

        for (size_t i = 0; i < seg; ++i)
            printf("%s%c%s", col, sym, CLR(RESET));
    }

    // Fill remaining space if any (due to rounding)
    while (painted++ < cols) printf("%s.%s", CLR(DIM), CLR(RESET));

    printf("]\n");

    // Print usage stats
    double pct = (total ? (100.0 * (double)used / (double)total) : 0.0);
    const char *pc_col = CLR(GREEN);
    if      (pct >= 85.0) pc_col = CLR(RED);
    else if (pct >= 60.0) pc_col = CLR(YELLOW);

    printf("Used: %zu bytes / %zu bytes  (%s%5.2f%%%s full)\n",
        used, total, pc_col, pct, CLR(RESET));
}

// Print legend for heap map symbols
void dbg_print_legend(void) {
    printf("\nLegend: [%s#%s]=Used  [%s.%s]=Free  [%sC%s]=Coalesced  [%sL%s]=Large\n",
           CLR(GREEN), CLR(RESET),
           CLR(DIM),   CLR(RESET),
           CLR(MAGENTA), CLR(RESET),
           CLR(YELLOW), CLR(RESET));
}
