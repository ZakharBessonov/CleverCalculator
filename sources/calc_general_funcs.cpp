#include <stdio.h>
#include <stdlib.h>

#include "calc_structs.h"
#include "calc_general_funcs.h"
#include "calc_dump.h"
#include "calc_general_funcs.h"
#include "calc_set_get.h"
#include "calc_macros.h"
#include "calc_colors.h"
#include "calc_consts.h"

extern FILE* logfileCalc;

static void CalcShowWelcomeMessage()
{
    printf("Умный Калькулятор 1.0.0\n");
}

void CalcCtor(MathExpression* mathExpression)
{
    SetRoot(mathExpression, NULL);
    SetBufferPointer(mathExpression, NULL);
    SetLenOfBuffer(mathExpression, 0);
    SetFileCounter(mathExpression, 0);

    Variable* tempVariablesPointer = (Variable*)calloc(MAX_COUNT_OF_VARIABLES, sizeof(Variable));
    if (tempVariablesPointer == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: A error was occurred while allocating memory for array of variables.\n");
        return;
    }

    SetVariablesPointer(mathExpression, tempVariablesPointer);
    SetVariablesCounter(mathExpression, 0);
}

int CalcVerify(MathExpression* mathExpression)
{
    int errors = 0;
    if (mathExpression == NULL)
    {
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    return CALC_OK;
}

void CalcDtor(MathExpression* mathExpression)
{
    free(GetBufferPointer(mathExpression));
    SetLenOfBuffer(mathExpression, 0);
    SetFileCounter(mathExpression, 0);
    if (GetRoot(mathExpression) != NULL)
    {
        NodeFree(GetRoot(mathExpression));
    }
    free(GetVariablesPointer(mathExpression));
}

void CalcStart(MathExpression* mathExpression)
{
    CalcShowWelcomeMessage();
}

void NodeFree(Node* node)
{
    if (GetLeft(node) != NULL)
    {
        NodeFree(GetLeft(node));
    }

    if (GetRight(node) != NULL)
    {
        NodeFree(GetRight(node));
    }

    free(node);
}

