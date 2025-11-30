#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

#include "calc_structs.h"
#include "calc_grammar_constructions.h"
#include "calc_macros.h"
#include "calc_consts.h"
#include "binary_search.h"
#include "calc_comparators.h"
#include "calc_hash.h"
#include "calc_set_get.h"

extern FILE* logfileCalc;
extern char* ioFileName;
extern Operation operations[];
extern size_t numOfOperations;

// NOTE: Заменить $ на \0

static void SkipSpaces(char** curPos)
{
    (*curPos)++;
    while (isspace(**curPos))
    {
        (*curPos)++;
    }
}

static void SkipSpacesAndPrintSymbolInLogfile(char** curPos)
{
    SkipSpaces(curPos);
    fprintf(logfileCalc, "%c", **curPos);
}

static int IsVar(char* str)
{
    return (isalpha(str[0]) && !isalpha(str[1]));
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

static int IsFunc(char* str)
{
    char tempStr[SPARE_VOLUME] = "";
    sscanf(str, "%[A-Za-z]", tempStr);
    unsigned long tempHash = CalcCountHashDjb2(tempStr);
    int indexOfOperation = (int)FindOperationByHash(tempHash, tempStr);

    return indexOfOperation;
}

static Node* CalcNewNumNode(long double number)
{
    Node* newNumNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newNumNode);

    SetTypeNode(newNumNode, TYPE_NUMBER);
    SetNumVal(newNumNode, number);

    return newNumNode;
}

static Node* CalcNewVarNode(MathExpression* mathExpression, char varIdentifier)
{
    Node* newVarNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newVarNode);

    SetTypeNode(newVarNode, TYPE_VAR);
    SetVarIdentifierToNode(newVarNode, varIdentifier);
    AddVarInArrayOfVarsIfNeeded(mathExpression, varIdentifier);

    return newVarNode;
}

static Node* CalcNewOpNodeWithOneArg(Node* node, OperationCode operationCode)
{
    Node* newOpNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newOpNode);

    SetTypeNode(newOpNode, TYPE_OP);
    SetOperation(newOpNode, operationCode);

    SetParent(node, newOpNode);
    SetRight(newOpNode, node);
    return newOpNode;
}

static Node* CalcNewOpNodeWithTwoArgs(Node* leftNode, Node* rightNode, OperationCode operationCode)
{
    Node* newOpNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newOpNode);

    SetTypeNode(newOpNode, TYPE_OP);
    SetOperation(newOpNode, operationCode);

    SetParent(leftNode, newOpNode);
    SetParent(rightNode, newOpNode);
    SetLeft(newOpNode, leftNode);
    SetRight(newOpNode, rightNode);

    return newOpNode;
}

Node* GetN(char** curPos)
{
    long double val = 0;
    int counter1 = 0;
    int counter2 = 0;
    int hasIntegerPartEnded = 0;

    while (('0' <= **curPos && **curPos <= '9') || **curPos == '.')
    {
        counter2 += hasIntegerPartEnded;

        if (**curPos == '.')
        {
            hasIntegerPartEnded++;
            SkipSpacesAndPrintSymbolInLogfile(curPos);
            continue;
        }

        if (hasIntegerPartEnded > 1)
        {
            PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetN)\n", **curPos);
            return NULL;
        }

        val = val * 10 + (**curPos - '0');
        SkipSpacesAndPrintSymbolInLogfile(curPos);
        counter1++;
    }

    if ((counter1 == 0) || (hasIntegerPartEnded == 1 && counter2 == 0))
    {
        PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetN)\n", **curPos);
        return NULL;
    }

    val /= powl(10, counter2);

    Node* newNumNode = CalcNewNumNode(val);
    return newNumNode;
}

Node* GetV(MathExpression* mathExpression, char** curPos)
{
    Node* newVarNode = CalcNewVarNode(mathExpression, **curPos);
    RET_IF_NULL(newVarNode);

    SkipSpacesAndPrintSymbolInLogfile(curPos);

    return newVarNode;
}

Node* GetE(MathExpression* mathExpression, char** curPos)
{
    Node* leftNode = GetT(mathExpression, curPos);
    Node* rightNode = NULL;
    Node* newOpNode = leftNode;
    RET_IF_NULL(leftNode);

    while (**curPos == '+' || **curPos == '-')
    {
        char op = **curPos;
        SkipSpacesAndPrintSymbolInLogfile(curPos);
        rightNode = GetT(mathExpression, curPos);
        RET_IF_NULL(rightNode);

        if (op == '+')
        {
            newOpNode = CalcNewOpNodeWithTwoArgs(newOpNode, rightNode, OP_ADD);
            RET_IF_NULL(newOpNode);
        }
        else
        {
            newOpNode = CalcNewOpNodeWithTwoArgs(newOpNode, rightNode, OP_SUB);
            RET_IF_NULL(newOpNode);
        }
    }

    return newOpNode;
}

Node* GetT(MathExpression* mathExpression, char** curPos)
{
    Node* leftNode = GetS(mathExpression, curPos);
    Node* rightNode = NULL;
    Node* newOpNode = leftNode;
    RET_IF_NULL(leftNode);

    while (**curPos == '*' || **curPos == '/')
    {
        char op = **curPos;
        SkipSpacesAndPrintSymbolInLogfile(curPos);
        rightNode = GetS(mathExpression, curPos);
        RET_IF_NULL(rightNode);

        if (op == '*')
        {
            newOpNode = CalcNewOpNodeWithTwoArgs(newOpNode, rightNode, OP_MUL);
            RET_IF_NULL(newOpNode);
        }
        else
        {
            newOpNode = CalcNewOpNodeWithTwoArgs(newOpNode, rightNode, OP_DIV);
            RET_IF_NULL(newOpNode);
        }
    }

    return newOpNode;
}

Node* GetS(MathExpression* mathExpression, char** curPos)
{
    Node* leftNode = GetP(mathExpression, curPos);
    Node* rightNode = NULL;
    Node* newOpNode = leftNode;
    RET_IF_NULL(leftNode);

    while (**curPos == '^')
    {
        SkipSpacesAndPrintSymbolInLogfile(curPos);

        rightNode = GetP(mathExpression, curPos);
        RET_IF_NULL(rightNode);

        newOpNode = CalcNewOpNodeWithTwoArgs(newOpNode, rightNode, OP_POW);
        RET_IF_NULL(newOpNode);
    }

    return newOpNode;
}

Node* GetG(MathExpression* mathExpression, char** curPos)
{
    fprintf(logfileCalc, "%c", **curPos);
    Node* newOpNode = GetE(mathExpression, curPos);

    if (**curPos != '$')
    {
        PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetG)\n", **curPos);
        return NULL;
    }
    SkipSpacesAndPrintSymbolInLogfile(curPos);
    return newOpNode;
}

Node* GetP(MathExpression* mathExpression, char** curPos)
{
    Node* newOpNode = NULL;
    int sign = 1;
    int indexOfOperation = 0;

    if (**curPos == '-')
    {
        sign = -1;
        SkipSpacesAndPrintSymbolInLogfile(curPos);
    }

    if (**curPos == '(')
    {
        SkipSpacesAndPrintSymbolInLogfile(curPos);
        newOpNode = GetE(mathExpression, curPos);

        if (**curPos != ')')
        {
            PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetF). ')' was expected.\n", **curPos);
            return NULL;
        }

        SkipSpacesAndPrintSymbolInLogfile(curPos);
    }
    else if(IsVar(*curPos))
    {
        newOpNode = GetV(mathExpression, curPos);
    }
    else if((indexOfOperation = IsFunc(*curPos)) != -1)       // Заменить логику, строка читается дважды
    {
        newOpNode = GetF(mathExpression, curPos, indexOfOperation);
    }
    else
    {
        newOpNode = GetN(curPos);
    }

    if (sign == -1)
    {
        Node* rightNode = CalcNewNumNode(-1);
        RET_IF_NULL(rightNode);
        newOpNode = CalcNewOpNodeWithTwoArgs(newOpNode, rightNode, OP_MUL);
    }

    return newOpNode;
}

Node* GetF(MathExpression* mathExpression, char** curPos, int indexOfOperation)
{
    while (isalpha(**curPos))
    {
        (*curPos)++;     // name of function must be without spaces
        fprintf(logfileCalc, "%c", **curPos);
    }

    if (**curPos != '(')
    {
        PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetF). '(' was expected.\n", **curPos);
        return NULL;
    }

    SkipSpacesAndPrintSymbolInLogfile(curPos);

    Node* leftNode = NULL;
    Node* rightNode = NULL;
    Node* newOpNode = NULL;

    if (operations[indexOfOperation].operationCode == OP_LOG)
    {
        leftNode = GetE(mathExpression, curPos);
        RET_IF_NULL(leftNode);

        if (**curPos != ',')
        {
            PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetF). ',' was expected.\n", **curPos);
            return NULL;
        }
        SkipSpacesAndPrintSymbolInLogfile(curPos);
        rightNode = GetE(mathExpression, curPos);
        RET_IF_NULL(rightNode);

        if (**curPos != ')')
        {
            PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetF). ')' was expected.\n", **curPos);
            return NULL;
        }

        SkipSpacesAndPrintSymbolInLogfile(curPos);

        newOpNode = CalcNewOpNodeWithTwoArgs(leftNode, rightNode, OP_LOG);
    }
    else
    {
        newOpNode = GetE(mathExpression, curPos);
        RET_IF_NULL(newOpNode);

        if (**curPos != ')')
        {
            PRINT_LOG_FILE_CALC("SYNTAX_ERROR: Invalid symbol '%c' (from GetF). ')' was expected.\n", **curPos);
            return NULL;
        }

        SkipSpacesAndPrintSymbolInLogfile(curPos);

        newOpNode = CalcNewOpNodeWithOneArg(newOpNode, operations[indexOfOperation].operationCode);
    }

    return newOpNode;
}



