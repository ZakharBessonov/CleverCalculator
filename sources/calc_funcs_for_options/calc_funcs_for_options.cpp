#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_funcs_for_options.h"
#include "calc_general_funcs.h"
#include "calc_macros.h"
#include "calc_set_get.h"
#include "binary_search.h"
#include "calc_comparators.h"
#include "calc_colors.h"
#include "calc_dump.h"
#include "calc_read_write_to_file.h"
#include "calc_simplification.h"
#include "calc_dsl.h"

extern Operation operations[];
extern size_t numOfOperations;
extern FILE* logfileCalc;
extern FILE* texFile;
int phraseCounter = 0;
int isNeedToWriteDerivativesInTeX = 0;

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

static int InputPoint(long double* point)
{
    int result = scanf("%Lf", point);
    while (getchar() != '\n');

    while (result <= 0)
    {
        if (result == EOF)
        {
            cprintf(RED, "\nВведён символ конца файла. Программа завершена.\n");
            return 1;
        }

        cprintf(RED, "Некорректный ввод. Повторите ввод: ");
        result = scanf("%Lf", point);
        while (getchar() != '\n');
    }

    return 0;
}

static int CalcChooseVarDifferentiation(MathExpression* mathExpression, char* varDifferentiation)
{
    printf("Выберите переменную дифференцирования: ");
    int result = scanf("%c", varDifferentiation);
    while (getchar() != '\n');

    while (result <= 0 || !isalpha(*varDifferentiation)
           || GetVarIdentifierFromArrayOfVars(mathExpression, *varDifferentiation) != *varDifferentiation)
    {
        if (result == EOF)
        {
            cprintf(RED, "\nВведён символ конца файла. Программа завершена.\n");
            return 1;
        }

        cprintf(RED, "Некорректный ввод '%c'. Повторите ввод: ", *varDifferentiation);
        result = scanf("%c", varDifferentiation);
        while (getchar() != '\n');
    }

    return 0;
}

static int FindVarDifferentiation(MathExpression* mathExpression, char* varDifferentiation)
{
    Variable* varPointer = GetVariablesPointer(mathExpression);
    int tempVarCounter = 0;

    for (int i = 0; i < MAX_NUM_OF_VARIABLES; i++)
    {
        if (varPointer[i].identifier == GetVariableSpelling(i))
        {
            tempVarCounter++;
            *varDifferentiation = GetVariableSpelling(i);
        }

        if (tempVarCounter >= 2)
        {
            *varDifferentiation = '\0';
            return 1;
        }
    }

    return !(tempVarCounter == 1);
}

static int CalcChooseOrder(MathExpression* mathExpression, int* orderOfDerivative)
{
    int result = scanf("%d", orderOfDerivative);
    while (getchar() != '\n');

    while (result <= 0 || *orderOfDerivative < 0)
    {
        if (result == EOF)
        {
            cprintf(RED, "\nВведён символ конца файла. Программа завершена.\n");
            return 1;
        }

        cprintf(RED, "Некорректный ввод. Повторите ввод: ");
        result = scanf("%d", orderOfDerivative);
        while (getchar() != '\n');
    }

    return 0;
}

static int CalcSetValuesOfVars(MathExpression* mathExpression)
{
    int varCounter = GetVariablesCounter(mathExpression);
    Variable* varPointer = GetVariablesPointer(mathExpression);

    if (varCounter != 0)
    {
        cprintf(GREEN, "Необходимо ввести значение каждой переменной:\n");
    }

    for (int i = 0; i < MAX_NUM_OF_VARIABLES; i++)
    {
        if (varPointer[i].identifier == GetVariableSpelling(i))
        {
            printf("%c = ", varPointer[i].identifier);
            long double tempVal = ScanfValueOfVar();
            if (isnan(tempVal))
            {
                return 1;
            }
            SetVariableValue(mathExpression, GetVariableSpelling(i), tempVal);
        }
    }

    return 0;
}

static Node* CopyNode(Node* oldNode)
{
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    memcpy(newNode, oldNode, sizeof(Node));
    if (GetLeft(oldNode) != NULL)
    {
        SetLeft(newNode, CopyNode(GetLeft(oldNode)));
        SetParent(GetLeft(newNode), newNode);
    }

    if (GetRight(oldNode) != NULL)
    {
        SetRight(newNode, CopyNode(GetRight(oldNode)));
        SetParent(GetRight(newNode), newNode);
    }
    return newNode;
}

static void CopyExpression(MathExpression* expression1, MathExpression* expression2)
{
    SetRoot(expression1, CopyNode(GetRoot(expression2)));
    expression1->fileCounter = expression2->fileCounter;
    Variable* pt1 = GetVariablesPointer(expression1);
    Variable* pt2 = GetVariablesPointer(expression2);

    for (int i = 0; i < MAX_NUM_OF_VARIABLES; i++)
    {
        pt1[i] = pt2[i];
    }
    SetVariablesCounter(expression1, GetVariablesCounter(expression2));
}

static Node* CalcNewNumNode(long double number)
{
    Node* newNumNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newNumNode);

    SetTypeNode(newNumNode, TYPE_NUMBER);
    SetNumVal(newNumNode, number);

    return newNumNode;
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
        tempVal = GetVariableValue(mathExpression, GetVarIdentifierFromNode(node));
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

static void PrintTaylorSeriesAndPlotGraphs(MathExpression* mathExpression, long double* coefficients,
                                           int order, char var, long double x0)
{
    CalcWriteTitleToTexFile("Разложение заданной функции в ряд Тейлора до %d-ого порядка:", order);
    fprintf(texFile, "\\begin{dmath}\n"
                     "f(%c) = ", var);
    int IsMaclaurin = IsEqual(x0, 0.0);

    for (int i = 0; i <= order; i++)
    {
        if (IsEqual(coefficients[i], 0.0))
        {
            continue;
        }

        if (i == 0)
        {
            fprintf(texFile, "%Lg", coefficients[i]);
            continue;
        }

        if (IsMaclaurin)
        {
            fprintf(texFile, "%+Lg%c^{%d}", coefficients[i], var, i);
        }
        else
        {
            fprintf(texFile, "%+Lg\\cdot(%c%c%Lg)^{%d}", coefficients[i], var, (x0 > 0) ? '-' : '+', x0, i);
        }
    }
    if (IsMaclaurin)
    {
        fprintf(texFile, "+o(%c^{%d})\n"
                         "\\end{dmath}\n", var, order);
    }
    else
    {
        fprintf(texFile, "+o((%c%c%Lg)^{%d})\n"
                         "\\end{dmath}\n", var, (x0 > 0) ? '-' : '+', x0, order);
    }
    fflush(texFile);
}

Node* CalcDifferentiate(Node* node, char varDifferentiation)
{
    NodeType type = GetTypeNode(node);
    Node* newNode = NULL;

    if (type == TYPE_NUMBER || (type == TYPE_VAR && GetVarIdentifierFromNode(node) != varDifferentiation))
    {
        newNode = CalcNewNumNode(0);
        $(WriteDerivative(node, newNode, varDifferentiation));
        return CalcNewNumNode(0);
    }
    else if (type == TYPE_VAR)
    {
        newNode = CalcNewNumNode(1);
        $(WriteDerivative(node, newNode, varDifferentiation));
        return CalcNewNumNode(1);
    }
    else
    {
        OperationCode operationCode = GetOperation(node);
        return operations[operationCode].funcForDerivative(node, varDifferentiation);
    }
}

void WriteDerivative(Node* node, Node* dNode, char varDifferentiation)
{
    phraseCounter = rand() % numOfPhrases;
    fprintf(texFile, "%s\n\\begin{dmath}\n"
                     "\\frac{d}{d%c}\\left(", phrases[phraseCounter], varDifferentiation);
    OperationCode operationCode = (GetTypeNode(node) == TYPE_OP) ? GetOperation(node) : DEFAULT_OP;
    operations[operationCode].funcToWritingInTeXFile(node);

    fprintf(texFile, "\\right)=");
    operationCode = (GetTypeNode(dNode) == TYPE_OP) ? GetOperation(dNode) : DEFAULT_OP;
    operations[operationCode].funcToWritingInTeXFile(dNode);
    fprintf(texFile, "\n\\end{dmath}\n");
    fflush(texFile);
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
    char varDifferentiation = '\0';
    int orderOfDerivative = 0;
    isNeedToWriteDerivativesInTeX = 1;
    if (CalcChooseVarDifferentiation(mathExpression, &varDifferentiation))
    {
        return 1;
    }

    printf("Выберите порядок производной (>= 1; если введёте 0, то ничего не произойдёт): ");
    if (CalcChooseOrder(mathExpression, &orderOfDerivative))
    {
        return 1;
    }

    MathExpression tempDerivative = {};
    MathExpression totalDerivative = {};

    CopyExpression(&tempDerivative, mathExpression);

    for (int i = 1; i <= orderOfDerivative; i++)
    {
        CalcWriteTitleToTexFile("Посчитаем производную %d-ого порядка!", i);
        CopyExpression(&totalDerivative, &tempDerivative);
        // CALL_DUMP(&totalDerivative, "Before differentiation (derivative (%d))", i - 1);
        SetRoot(&totalDerivative, CalcDifferentiate(GetRoot(&tempDerivative), varDifferentiation));
        // CALL_DUMP(&totalDerivative, "Before simplification (derivative (%d))", i);
        CalcSimplifyExpression(&totalDerivative);
        // CALL_DUMP(&totalDerivative, "Derivative (%d)", i);
        WriteDerivative(GetRoot(&tempDerivative), GetRoot(&totalDerivative), varDifferentiation);
        CalcDtor(&tempDerivative);
        CopyExpression(&tempDerivative, &totalDerivative);
        CalcDtor(&totalDerivative);
    }

    CalcDtor(&tempDerivative);
    return 0;
}

int CalcCountTaylorSeries(MathExpression* mathExpression)
{
    char varDifferentiation = '\0';
    int order = 0;
    long double x0 = 0.0;
    isNeedToWriteDerivativesInTeX = 0;
    if (FindVarDifferentiation(mathExpression, &varDifferentiation))
    {
        cprintf(RED, "Переменная должна быть одна!\n");
        return 1;
    }

    printf("Выберите порядок, до которого хотите разложиться (=>0): ");
    if (CalcChooseOrder(mathExpression, &order))
    {
        return 1;
    }

    printf("Выберите точку, в которой хотите разложиться: ");
    if (InputPoint(&x0))
    {
        return 1;
    }
    SetVariableValue(mathExpression, varDifferentiation, x0);

    long double* coefficients = (long double*)calloc((size_t)order + 1, sizeof(long double));
    coefficients[0] = CalcCountNode(mathExpression, GetRoot(mathExpression));

    MathExpression tempDerivative = {};
    MathExpression totalDerivative = {};

    CopyExpression(&tempDerivative, mathExpression);
    int fact = 1;

    for (int i = 1; i <= order; i++)
    {
        CopyExpression(&totalDerivative, &tempDerivative);
        // CALL_DUMP(&totalDerivative, "Before differentiation (derivative (%d))", i - 1);
        SetRoot(&totalDerivative, CalcDifferentiate(GetRoot(&tempDerivative), varDifferentiation));
        // CALL_DUMP(&totalDerivative, "Before simplification (derivative (%d))", i);
        CalcSimplifyExpression(&totalDerivative);
        // CALL_DUMP(&totalDerivative, "Derivative (%d)", i);
        CalcDtor(&tempDerivative);
        CopyExpression(&tempDerivative, &totalDerivative);
        CalcDtor(&totalDerivative);
        coefficients[i] = CalcCountNode(&tempDerivative, GetRoot(&tempDerivative)) / (long double)(fact *= i);
    }

    CalcDtor(&tempDerivative);

    PrintTaylorSeriesAndPlotGraphs(mathExpression, coefficients, order, varDifferentiation, x0);
    free(coefficients);
    return 0;
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
