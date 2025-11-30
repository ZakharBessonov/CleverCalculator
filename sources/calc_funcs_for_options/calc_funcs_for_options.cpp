#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "calc_structs.h"
#include "calc_funcs_for_options.h"
#include "calc_general_funcs.h"
#include "calc_macros.h"
#include "calc_set_get.h"
#include "binary_search.h"
#include "calc_comparators.h"
#include "calc_colors.h"
#include "calc_dump.h"
#include "calc_read_write_to_file.h"

extern Operation operations[];
extern size_t numOfOperations;
extern FILE* logfileCalc;

static long double ScanfValueOfVar()
{
    long double tempVal = 0.0;
    int result = scanf("%Lf", &tempVal);
    while (getchar() != '\n');      // to take all until \n

    while (result <= 0)
    {
        if (result == EOF)
        {
            cprintf(RED, "\nВведён символ конца файла. Программа завершена.\n");
            return NAN;
        }

        cprintf(RED, "Некорректный ввод. Повторите ввод: ");
        result = scanf("%Lf", &tempVal);
        while (getchar() != '\n');      // to take all until \n
    }

    return tempVal;
}

static int CalcSetValuesOfVars(MathExpression* mathExpression)
{
    int varCounter = GetVariablesCounter(mathExpression);
    Variable* varPointer = GetVariablesPointer(mathExpression);

    if (varCounter != 0)
    {
        cprintf(GREEN, "Чтобы вычислить значение выражения, необходимо ввести значение каждой переменной:\n");
    }

    for (int i = 0; i < varCounter; i++)
    {
        printf("%c = ", varPointer[i].identifier);
        long double tempVal = ScanfValueOfVar();
        if (isnan(tempVal))
        {
            return 1;
        }
        SetVariableValue(mathExpression, i, tempVal);
    }

    return 0;
}

static Node* CalcNewNumNode(long double number)
{
    Node* newNumNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newNumNode);

    SetTypeNode(newNumNode, TYPE_NUMBER);
    SetNumVal(newNumNode, number);

    return newNumNode;
}

Node* CalcDifferentiate(Node* node, char varDifferentiation)
{
    NodeType type = GetTypeNode(node);
    Node* newNode = (Node*)calloc(1, sizeof(newNode));
    if (type == TYPE_NUMBER || (type == TYPE_VAR && GetVarIdentifierFromNode(node) != varDifferentiation))
    {

        return CalcNewNumNode(0);
    }
    else if (type == TYPE_VAR)
    {
        return CalcNewNumNode(1);
    }
    else
    {
        OperationCode operationCode = GetOperation(node);
        return operations[operationCode].funcForDerivative(node);
    }
}

static long double GetValOfVarByItsSpelling(MathExpression* mathExpression, char varIdentifier)
{
    Variable wantedVar = {varIdentifier, 0.0};
    Variable* ptToFirst = GetVariablesPointer(mathExpression);
    size_t varCounter = (size_t)GetVariablesCounter(mathExpression);
    ssize_t indexOfVar = FindElemInSortedArray(&wantedVar, ptToFirst, varCounter,
                              sizeof(Variable), ComparatorOfVars);
    long double valueOfVar = 0.0;
    if (indexOfVar == -1)
    {
        PRINT_LOG_FILE_CALC("ERROR: Variable '%c' is not found.\n", varIdentifier);
        return NAN;
    }
    else
    {
        valueOfVar = ptToFirst[indexOfVar].value;
    }

    return valueOfVar;
}

static long double CalcCountNode(MathExpression* mathExpression, Node* node)
{
    NodeType nodeType = GetTypeNode(node);
    long double tempVal = 0.0;

    if (nodeType == TYPE_NUMBER)
    {
        GetNumVal(node, &tempVal);
        return tempVal;
    }
    else if (nodeType == TYPE_VAR)
    {
        tempVal = GetValOfVarByItsSpelling(mathExpression, GetVarIdentifierFromNode(node));
        return tempVal;
    }

    OperationCode operationCode = GetOperation(node);
    NumOfArgs numOfArgs = operations[operationCode].numOfArgs;
    Node* leftNode = GetLeft(node);
    Node* rightNode = GetRight(node);
    if (numOfArgs == ONE_ARG)
    {
        return operations[operationCode].funcForOperation({CalcCountNode(mathExpression, rightNode)});
    }
    else
    {
        return operations[operationCode].funcForOperation({CalcCountNode(mathExpression, leftNode),
                                                           CalcCountNode(mathExpression, rightNode)});
    }
}

int CalcCountExpression(MathExpression* mathExpression)
{
    if (CalcSetValuesOfVars(mathExpression))
    {
        return 1;
    }

    printf("Значение выражения равно %Lg\n", CalcCountNode(mathExpression, GetRoot(mathExpression)));
    return 0;
}

int CalcCountDerivative(MathExpression* mathExpression)
{
    // if (CalcChooseVarDifferentiation(mathExpression, &varDifferentiation))
    // {
    //     return 1;
    // }

    MathExpression derivative = {};
    CalcCtor(&derivative);
    SetRoot(&derivative, CalcDifferentiate(GetRoot(mathExpression), 'x'));
    SetVariablesPointer(&derivative, GetVariablesPointer(mathExpression));
    SetFileCounter(&derivative, 2);
    CALL_DUMP(&derivative, "After differentiation");
    CalcWriteExpressionToTeXFile(&derivative, "Попробуем посчитать производную", "Надеюсь, я ещё не забыл то,"
                                              " что изучается в 10 классе");

    return 0;
}

int CalcCountTaylorSeries(MathExpression* mathExpression)
{
    return 1;
}

int CalcPlotGraph(MathExpression* mathExpression)
{
    return 1;
}

int CalcPlotTangentToGraph(MathExpression* mathExpression)
{
    return 1;
}

int CalcCountExpressionTakingIntoAccountTheError(MathExpression* mathExpression)
{
    return 1;
}
