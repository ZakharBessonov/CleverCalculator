#ifndef CALC_MACROS_HEADER
#define CALC_MACROS_HEADER

#define BEGIN do
#define END while(false)

#define PRINT_LOG_FILE_CALC(X, ...) BEGIN { printf("ERROR: Look logfile!\n");\
fprintf(logfileCalc, "<pre>\n%s:%d %s: <red>" X "</red>\n</pre>",__FILE__, __LINE__,\
                                   __func__, ##__VA_ARGS__);\
                                   fflush(logfileCalc); } END

#define CALL_DUMP(X, Y, ...) BEGIN { CalcDump(X, __FILE__, __func__, __LINE__, Y, ##__VA_ARGS__); } END

#define RET_IF_NULL(X) BEGIN { if (X == NULL) {return NULL;} } END

#endif
