#include <stdio.h>
#include <stdlib.h>

#include "calc_structs.h"
#include "calc_general_funcs.h"
#include "calc_read_write_to_file.h"
#include "calc_dump.h"
#include "calc_macros.h"

extern FILE* logfileCalc;
extern char* ioFileName;

int main(int argc, char *argv[])
{
    atexit(CloseLogFileCalc);

    if (OpenLogFileCalc())
    {
        printf("FATAL ERROR: An error occurred while opening logfileCalc.\n");
        return 0;
    }

    MathExpression expression = {};
    CalcCtor(&expression);

    FILE* ioStream = CalcOpenFile(argc, argv);

    if (ioStream == NULL)
    {
        return 0;
    }

    CalcReadFile(&expression, ioStream);
    CalcCreateTree(&expression);
    CALL_DUMP(&expression, "After reading");

    //CalcStart();

    //CalcStart(&expression);

    //CalcSimplifyExpression(&expression);
    //CalcWriteTreeToFile(&expression);
    //CALL_DUMP(&expression, "After adding new elem");

    CalcDtor(&expression);

    return 0;
}
