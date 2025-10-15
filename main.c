#include <stdio.h>
#include <stdlib.h>
#include "AllocatorBase.h"   // brings ALLOCATOR_VERSION
#include "DebugTool.h"
#include "VM.h"
#include "colors.h"

extern int color_enabled;

static void menu(void) {
    int choice;
    do {
        printf("\n%s%s======%s======%s AllocatorBase Interactive Console %s %s======%s======%s\n",
            CLR(BOLD), CLR(RED), CLR(WHITE), CLR(BLUE), ALLOCATOR_VERSION, CLR(WHITE), CLR(RED), CLR(RESET));
        printf("Custom Heap Allocator | malloc / calloc / realloc / free\n");
        printf("----------------------------------------------------------------\n");
        printf("1) Run Basic VM Demo\n");
        printf("2) Run Realloc Demo\n");
        printf("3) Run Fragmentation Test\n");
        printf("4) Run Stress Test\n");
        printf("%s5) Toggle Verbose Allocator Output (v1.1)%s\n", CLR(DIM), CLR(RESET));
        printf("%s6) Toggle Auto Heap Print (v1.1)%s\n", CLR(DIM), CLR(RESET));
        printf("%s7) Toggle Debug Mode (v1.1)%s\n", CLR(DIM), CLR(RESET));
        printf("8) View Heap State\n");
        printf("9) Exit\n");
        printf("----------------------------------------------------------------\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("%s[ERROR]%s Invalid input. Try again.\n", CLR(RED), CLR(RESET));
            continue;
        }
        printf("\n");

        switch (choice) {
            case 1:
                printf("%s[RUN]%s Running Basic VM Demo...\n", CLR(WHITE), CLR(RESET));
                vm_load_test_basic();
                printf("%s[DONE]%s Basic VM Demo complete.\n", CLR(GREEN), CLR(RESET));
                break;
            case 2:
                printf("%s[RUN]%s Running Realloc Demo...\n", CLR(WHITE), CLR(RESET));
                vm_load_test_realloc();
                printf("%s[DONE]%s Realloc Demo complete.\n", CLR(GREEN), CLR(RESET));
                break;
            case 3:
                printf("%s[RUN]%s Running Fragmentation Test...\n", CLR(WHITE), CLR(RESET));
                vm_load_test_frag();
                printf("%s[DONE]%s Fragmentation Test complete.\n", CLR(GREEN), CLR(RESET));
                break;
            case 4:
                printf("%s[RUN]%s Running Stress Test...\n", CLR(WHITE), CLR(RESET));
                vm_load_test_stress();
                printf("%s[DONE]%s Stress Test complete.\n", CLR(GREEN), CLR(RESET));
                break;
            case 5:
                printf("%s[VERBOSE]%s Reserved for v1.1.\n", CLR(CYAN), CLR(RESET));
                break;
            case 6:
                printf("%s[AUTO]%s Reserved for v1.1.\n", CLR(YELLOW), CLR(RESET));
                break;
            case 7:
                printf("%s[DEBUG]%s Reserved for v1.1.\n", CLR(MAGENTA), CLR(RESET));
                break;
            case 8:
                print_heap_state();
                dbg_print_heap_map();
                break;
            case 9:
                printf("%s[EXIT]%s Exiting...\n", CLR(RED), CLR(RESET));
                break;
            default:
                printf("%s[ERROR]%s Invalid option.\n", CLR(RED), CLR(RESET));
                break;
        }
    } while (choice != 9);

    printf("\n%s%s======%s======%s========================================%s======%s======%s\n",CLR(BOLD), CLR(RED), CLR(WHITE), CLR(BLUE), CLR(WHITE), CLR(RED), CLR(RESET));
    printf("AllocatorBase %s — All systems operational\n", ALLOCATOR_VERSION);
    printf("Build Type: %s\n",
#ifdef __OPTIMIZE__
           "Release (Optimized)"
#else
           "Debug (Symbols + Warnings)"
#endif
    );
    printf("Thank you for using AllocatorBase.\n");
    printf("%s%s======%s======%s========================================%s======%s======%s\n",CLR(BOLD), CLR(RED), CLR(WHITE), CLR(BLUE), CLR(WHITE), CLR(RED), CLR(RESET));
}

int main(void) {
    heap_init();
    dbg_print_legend();
    menu();
    return 0;
}
