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

void CalcWriteTreeToFile(MathExpression* mathExpression)
{
    FILE* ioFile = fopen(ioFileName, "w");
    if (ioFile == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: A error was occurred while opening ioFile.txt for writing.\n");
        return;
    }

    CalcWriteNodeToFile(GetRoot(mathExpression), ioFile);
    fclose(ioFile);
}

void CalcWriteNodeToFile(Node* node, FILE* ioFile)
{
    fprintf(ioFile, "( \"");
    //fputs(NodeGetData(node), ioFile);
    fprintf(ioFile, "\" ");

    if (GetRight(node) != NULL)
    {
        CalcWriteNodeToFile(GetRight(node), ioFile);
    }
    else
    {
        fprintf(ioFile, " nil ");
    }

    if (GetLeft(node) != NULL)
    {
        CalcWriteNodeToFile(GetLeft(node), ioFile);
    }
    else
    {
        fprintf(ioFile, "nil ");
    }

    fprintf(ioFile, ") ");
}



