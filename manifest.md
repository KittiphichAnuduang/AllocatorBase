# 📦 AllocatorBase Manifest

|      Fields      |                    Value                    |
|------------------|---------------------------------------------|
| **Project Name** | AllocatorBase                               |
| **Version**      | 1.0                                         |
| **Stage**        | Release                                     |
| **Language**     | C (C11)                                     |
| **Compiler**     | GCC / Clang                                 |
| **Build System** | GNU Make                                    |
| **License**      | MIT                                         |
| **Repository**   | github.com/KittiphichAnuduang/AllocatorBase |
| **Author**       | Kittiphich Anuduang                         |
| **Created**      | April 2025                                  |
| **Last Updated** | October 2025                                |

## Description
AllocatorBase is a minimalist, educational memory allocator demonstrating block-based heap management, coalescing, and reallocation techniques.  
It includes debug visualization and a testing virtual machine for showcasing allocator behavior.

## Dependencies
- Standard C Library (`stdio.h`, `stdlib.h`, `string.h`)
- GNU Make 4.4+
- GCC 11+ or Clang 12+

## Module Summary
|           File           |             Description             |
|--------------------------|-------------------------------------|
| `AllocatorBase.c` / `.h` | Core heap allocator implementations |
| `DebugTool.c` / `.h`     | Visualization and debug toggles     |
| `VM.c` / `.h`            | Allocation test programs            |
| `main.c`                 | Interactive console entry point     |
| `colors.h`               | ANSI color definitions              |
| `Makefile`               | Build automation script             |

## Integrity
Test Status: Passed all VM demos  
Warnings: None (compiled cleanly with `-Wall -Wextra -Wpedantic`)

## Notes
Future releases will include:
- Persistent heap snapshots
- File-backed virtual allocator memory
