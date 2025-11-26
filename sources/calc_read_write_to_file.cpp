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

static int IsDigit(char* str)
{
    return ((str[0] == '-' && isdigit(str[1])) || isdigit(str[0]));
}

static int IsVar(char* str)
{
    return (str[1] == '\0' && isalpha(str[0]));
}

static void AddVarInArrayOfVarsIfNeeded(MathExpression* mathExpression, char varIdentifier)
{
    Variable wantedVar = {varIdentifier, 0.0};
    Variable* ptToFirst = GetVariablesPointer(mathExpression);
    size_t varCounter = (size_t)GetVariablesCounter(mathExpression);
    ssize_t indexOfVarInArrayOfVars = FindElemInSortedArray(&wantedVar, ptToFirst, varCounter,
                                                            sizeof(Variable), ComparatorOfVars);

    if (indexOfVarInArrayOfVars == -1)
    {
        InsertElemInSortedArray(&wantedVar, ptToFirst, varCounter,
                                sizeof(Variable), ComparatorOfVars);
        IncrementVariablesCounter(mathExpression);
    }
}

static ssize_t FindOperationByHash(unsigned long hash, const char* spellingOfOperation)
{
    Operation wantedOp = {hash, DEFAULT_OP, DEFAULT_NUM_OF_ARGS, spellingOfOperation};
    ssize_t indexOfOperations = FindElemInSortedArray(&wantedOp, operations, numOfOperations,
                                                      sizeof(Operation), ComparatorOfOperationsByHash);
    if (indexOfOperations == -1 || strcmp(spellingOfOperation, operations[indexOfOperations].spellingOfOperation) != 0)
    {
        return -1;
    }

    return indexOfOperations;
}

static int CalcReadValueAndDefineItsType(MathExpression* mathExpression, Node* node, char** curPos)
{
    int lenOfString = 0;
    char tempStr[SPARE_VOLUME] = "";

    sscanf(*curPos, "%[^( \n\t]%n", tempStr, &lenOfString);

    tempStr[lenOfString] = '\0';
    if (IsDigit(tempStr))
    {
        long double numVal = 0.0;
        sscanf(tempStr, "%Lf", &numVal);
        SetTypeNode(node, TYPE_NUMBER);
        SetNumVal(node, numVal);
    }
    else if (IsVar(tempStr))
    {
        SetTypeNode(node, TYPE_VAR);
        SetVarIdentifierToNode(node, tempStr[0]);
        AddVarInArrayOfVarsIfNeeded(mathExpression, tempStr[0]);
    }
    else
    {
        unsigned long tempHash = CalcCountHashDjb2(tempStr);
        ssize_t indexOfOperation = FindOperationByHash(tempHash, tempStr);
        if (indexOfOperation == -1)
        {
            fprintf(logfileCalc, "<b><red>Что_то пошло не так! Неверная команда \"%s\" </red></b>\n", tempStr);
            *curPos += lenOfString;
            SkipSpaces(curPos);
            return -1;
        }
        SetTypeNode(node, TYPE_OP);
        SetOperation(node, operations[indexOfOperation].operationCode);
    }

    *curPos += lenOfString;
    SkipSpaces(curPos);
    return GetTypeNode(node);
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
    SetRoot(mathExpression, CalcReadNode(mathExpression, &curPos, NULL));
    fprintf(logfileCalc, "</pre>\n");

    qsort(operations, numOfOperations, sizeof(Operation), ComparatorOfOperationsByCodes);
}

Node* CalcReadNode(MathExpression* mathExpression, char** curPos, Node* parent)
{
    if (**curPos == '(')
    {
        Node* newNode = (Node*)calloc(1, sizeof(Node));
        fprintf(logfileCalc, "<b>Создали узел,</b>\n");
        (*curPos)++;        // Skip '('
        fprintf(logfileCalc, "<b>Пропустили скобку:</b>\n"
                           "%s\n\n", *curPos);
        SkipSpaces(curPos);
        fprintf(logfileCalc, "<b>Пропустили пробелы:</b>\n"
                           "%s\n\n", *curPos);
        int typeOfValue = CalcReadValueAndDefineItsType(mathExpression, newNode, curPos);
        if (typeOfValue == -1)
        {
            free(newNode);
            return NULL;
        }
        fprintf(logfileCalc, "<b>Считали имя узла:</b>\n"
                           "%s\n\n", *curPos);
        SkipSpaces(curPos);
        fprintf(logfileCalc, "<b>Пропустили пробелы:</b>\n"
                           "%s\n\n", *curPos);
        SetParent(newNode, parent);
        fprintf(logfileCalc, "<b>Установили родителя узла,</b>\n");

        SetLeft(newNode, CalcReadNode(mathExpression, curPos, newNode));
        fprintf(logfileCalc, "<b>Считали левое поддерево:</b>\n"
                           "%s\n\n", *curPos);
        SetRight(newNode, CalcReadNode(mathExpression, curPos, newNode));
        fprintf(logfileCalc, "<b>Считали правое поддерево:</b>\n"
                           "%s\n\n", *curPos);

        SkipSpaces(curPos);
        fprintf(logfileCalc, "<b>Пропустили пробелы:</b>\n"
                           "%s\n\n", *curPos);
        (*curPos)++;        // Skip ')'
        fprintf(logfileCalc, "<b>Пропустили скобку:</b>\n"
                           "%s\n\n", *curPos);
        SkipSpaces(curPos);
        fprintf(logfileCalc, "<b>Пропустили пробелы:</b>\n"
                           "%s\n\n", *curPos);
        return newNode;
    }
    else if (IsNil(*curPos))
    {
        *curPos += LEN_NIL;
        fprintf(logfileCalc, "<b>Пропустили nil:</b>\n"
                           "%s\n\n", *curPos);
        SkipSpaces(curPos);
        return NULL;
    }

    // Here we can put log-message
    fprintf(logfileCalc, "<b><red>Что_то пошло не так! Неверный символ '%c' </red></b>\n", **curPos);
    return NULL;
}

void SkipSpaces(char** curPos)
{
    while (isspace(**curPos))
    {
        (*curPos)++;
    }
}

int IsNil(char* curPos)
{
    return strncmp(curPos, "nil", 3) == 0;
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



