#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "calc_funcs_for_options.h"
#include "calc_structs.h"
#include "calc_macros.h"

extern Operation operations[];
extern size_t numOfOperations;

static long double ScanfValueOfVar(Variables* varArray, int indexOfVar)
{
    long double tempVal = 0.0;
    int result = scanf("%Lf", tempVal);
    while (getchar() != '\n');      // to take all until \n

    while (result <= 0)
    {
        if (result == EOF)
        {
            printf("\nВведён символ конца файла. Программа завершена.\n");
            return NAN;
        }

        printf("Некорректный ввод. Повторите ввод: ");
        result = scanf("%Lf", tempVal);
        while (getchar() != '\n');      // to take all until \n
    }

    return tempVal;
}

static int CalcSetValuesOfVars(MathExpression* mathExpression)
{
    int varCounter = GetVariablesCounter(mathExpression);
    Variables* varArray = GetVariablesPointer(mathExpression);

    if (varCounter != 0)
    {
        printf("Чтобы вычислить значение выражения, необходимо ввести значение каждой переменной:\n");
    }

    for (int i = 0; i < varCounter; i++)
    {
        long double tempVal = ScanfValueOfVar(varArray, i);
        if (isnan(tempVal))
        {
            return 1;
        }
        SetVariableValue(mathExpression, i, tempVal);
    }

    return 0;
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
        PRINT_LOG_FILE_CALC("ERROR: Variable '%c' is not found.\n");
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
//     NodeType nodeType = GetTypeNode(node);
//     long double tempVal = 0.0;
//
//     if (nodeType == TYPE_NUMBER)
//     {
//         GetNumVal(node, &tempVal);
//         return tempVal;
//     }
//     else if (nodeType == TYPE_VAR)
//     {
//         tempVal = GetValOfVarByItsSpelling(mathExpression, GetVarIdentifierFromNode(node));
//         return tempVal;
//     }
//
//     OperationCode operationCode = GetOperation(node);
//     NumOfArgs numOfArgs = operations[operationCode].numOfArgs;
//     if (numOfArgs == ONE_ARG)
//     {
//
//     }
    return 1.0;
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
    return 1;
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
