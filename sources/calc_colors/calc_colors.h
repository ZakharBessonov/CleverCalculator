#ifndef CALC_COLORS_HEADER
#define CALC_COLORS_HEADER

const char * const RED = "\x1b[31m";
const char * const GREEN = "\x1b[32m";
const char * const BLACK = "\x1b[0m";
const char * const BLUE = "\x1b[34m";
const char * const MAGENTA = "\x1b[35m";
const char * const CYAN = "\x1b[36m";
const char * const YELLOW = "\x1b[33m";

int cprintf(const char* color, const char* fmt, ...);

int cputs(const char* color, const char* str);

#endif
