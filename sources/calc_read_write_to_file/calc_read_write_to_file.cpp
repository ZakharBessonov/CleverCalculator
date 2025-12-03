#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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
FILE* texFile = NULL;

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

    free(GetBufferPointer(mathExpression));
    SetBufferPointer(mathExpression, NULL);
    qsort(operations, numOfOperations, sizeof(Operation), ComparatorOfOperationsByCodes);
}

// Writing

void CalcOpenTexFileAndWritePreamble()
{
    texFile = fopen("texFile.tex", "w");
    if (texFile == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: An error was occurred while opening texFile.tex\n");
        return;
    }
    fprintf(texFile, "\\documentclass[a4paper]{article}\n"
                     "\\usepackage[T2A]{fontenc}\n"
                     "\\usepackage[utf8]{inputenc}\n"
                     "\\usepackage[russian]{babel}\n"
                     "\\usepackage{breqn}\n"
                     "\\usepackage[left=2cm, top=2cm, right=2cm, bottom=2cm]{geometry}\n"
                     "\\begin{document}\n");
    fflush(texFile);
}

void CalcWriteExpressionToTeXFile(MathExpression* mathExpression, const char* message)
{
    fprintf(texFile, "%s\n\\begin{dmath}\n", message);
    Node* root = GetRoot(mathExpression);
    OperationCode operationCode = (GetTypeNode(root) == TYPE_OP) ? GetOperation(root) : DEFAULT_OP;
    operations[operationCode].funcToWritingInTeXFile(GetRoot(mathExpression));
    fprintf(texFile, "\n\\end{dmath}\n");
    fflush(texFile);
}

void CalcWriteTitleToTexFile(const char* title, ...)
{
    va_list args = NULL;
    va_start(args, title);
    char tempStr[MAX_SIZE_OF_STRING] = "";

    vsnprintf(tempStr, MAX_SIZE_OF_STRING - 1, title, args);

    fprintf(texFile, "\\centerline{\\textbf{%s}}\n", tempStr);
    fflush(texFile);
}

void CalcFinishTeXFile()
{
    fprintf(texFile, "\n\\end{document}\n");
    fclose(texFile);
}



