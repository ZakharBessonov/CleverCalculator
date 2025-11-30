#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "calc_structs.h"
#include "calc_general_funcs.h"
#include "calc_dump.h"
#include "calc_read_write_to_file.h"
#include "calc_set_get.h"
#include "calc_macros.h"
#include "size_of_file.h"
#include "calc_consts.h"
#include "binary_search.h"
#include "calc_comparators.h"
#include "calc_hash.h"
#include "calc_grammar_constructions.h"
#include "calc_simplification.h"

extern FILE* logfileCalc;
extern char* ioFileName;
extern Operation operations[];
extern size_t numOfOperations;

// Reading

static void CountHashesForOperations()
{
    unsigned long tempHash = 0;

    for (size_t i = 0; i < numOfOperations; i++)
    {
        tempHash = CalcCountHashDjb2(operations[i].spellingOfOperation);
        operations[i].hashOfOperation = tempHash;
    }
}

static void SkipSpaces(char** curPos)
{
    while (isspace(**curPos))
    {
        (*curPos)++;
    }
}

FILE* CalcOpenFile(int argc, char *argv[])
{
    FILE* ioFile = NULL;
    if (argc <= 1)
    {
        PRINT_LOG_FILE_CALC("ERROR: Too few arguments were passed. How to enter: ioFile.txt\n");
        printf("ERROR: Too few arguments were passed. How to enter: ioFile.txt\n");
        return NULL;
    }
    else
    {
        ioFile = fopen(argv[1], "rb");
        ioFileName = argv[1];
    }

    if (ioFile == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: A error was occurred while opening ioFile.txt.\n");
        return NULL;
    }

    return ioFile;
}

void CalcReadFile(MathExpression* mathExpression, FILE* ioFile)
{
    size_t sizeOfFile = SizeOfFile(ioFile);
    if (sizeOfFile == 0)
    {
        return;
    }

    char* buffer = (char*)calloc(sizeOfFile + 1, 1);
    if (buffer == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: A error was occurred while allocating memory for buffer.\n");
        return;
    }

    SetBufferPointer(mathExpression, buffer);
    size_t lenOfBuffer = fread(buffer, 1, sizeOfFile, ioFile);
    buffer[sizeOfFile] = '\0';
    SetLenOfBuffer(mathExpression, lenOfBuffer);
    fclose(ioFile);
}

void CalcCreateTree(MathExpression* mathExpression)
{
    CountHashesForOperations();
    qsort(operations, numOfOperations, sizeof(Operation), ComparatorOfOperationsByHash);

    size_t lenOfBuffer = 0;
    GetLenOfBuffer(mathExpression, &lenOfBuffer);

    if (lenOfBuffer == 0)
    {
        return;
    }

    char* curPos = GetBufferPointer(mathExpression);
    SkipSpaces(&curPos);
    fprintf(logfileCalc, "<meta charset=\"utf-8\">\n"
                       "<pre>\n<h3>Начинаем читать файл %s:</h3>"
                       "%s\n", ioFileName, curPos);
    fprintf(logfileCalc, "<h3>Успешно прочитаны символы: </h3>");
    SetRoot(mathExpression, GetG(mathExpression, &curPos));
    fprintf(logfileCalc, "\n</pre>\n");

    qsort(operations, numOfOperations, sizeof(Operation), ComparatorOfOperationsByCodes);
}

// Writing

static void CalcWritePreamble(FILE* texFile)
{
    fprintf(texFile, "\\documentclass{article}\n"
                     "\\usepackage[T2A]{fontenc}\n"
                     "\\usepackage[utf8]{inputenc}\n"
                     "\\usepackage[russian]{babel}\n"
                     "\\begin{document}\n"
                     "Итак, самое время заняться нашим любимым делом - вычислениями!\n");
}

void CalcWriteExpressionToTeXFile(MathExpression* mathExpression, const char* message, const char* title)
{
    static int IsThisFirstOpening = 1;
    FILE* texFile = NULL;
    if (IsThisFirstOpening == 1)
    {
        texFile = fopen("texFile.tex", "w");
    }
    else
    {
        texFile = fopen("texFile.tex", "a");
    }

    if (texFile == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: An error was occurred while opening texFile.tex\n");
        return;
    }

    if (IsThisFirstOpening == 1)
    {
        CalcWritePreamble(texFile);
        IsThisFirstOpening--;
    }

    fprintf(texFile, "%s\\\\\n"
                     "\\centerline{\\textbf{%s}}\n"
                     "\\begin{equation}\n" , message, title);
    OperationCode operationCode = GetOperation(GetRoot(mathExpression));
    operations[operationCode].funcToWritingInTeXFile(texFile, GetRoot(mathExpression));
    fprintf(texFile, "\n\\end{equation}\n");

    CalcSimplifyExpression(mathExpression);
    CALL_DUMP(mathExpression, "After simplification");
    fprintf(texFile, "После упрощения неожиданно получилось вот это:\n"
                     "\\begin{equation}\n");
    operationCode = GetOperation(GetRoot(mathExpression));
    operations[operationCode].funcToWritingInTeXFile(texFile, GetRoot(mathExpression));
    fprintf(texFile, "\n\\end{equation}\n");
    fclose(texFile);
}

void CalcFinishTeXFile()
{
    FILE* texFile = fopen("texFile.tex", "a");
    fprintf(texFile, "\n\\end{document}\n");
    fclose(texFile);
}



