#ifndef COLORS_H
#define COLORS_H

// Base ANSI color codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Global color toggle (1=enabled, 0=disabled)
extern int color_enabled;

// Macro to conditionally apply color codes
#define CLR(c) (color_enabled ? c : "")

#endif
// === End of colors.h ===
