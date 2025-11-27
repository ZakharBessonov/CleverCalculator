#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "calc_structs.h"
#include "calc_general_funcs.h"
#include "calc_dump.h"
#include "calc_general_funcs.h"
#include "calc_set_get.h"
#include "calc_macros.h"
#include "calc_colors.h"
#include "calc_consts.h"

extern FILE* logfileCalc;
// extern int (*CalcFunctionsForOption[])(MathExpression*);
// extern size_t numOfFunctionsForOptions;
//
// static void CalcShowWelcomeMessage()
// {
//     printf("Умный Калькулятор 1.0.0\n");
// }
//
// static int CalcConvertStrOfOptionsToNum(char* tempStr, int lenOfStr)
// {
//     int chosenOptions = 0;
//     for (int i = 0; i < lenOfStr; i++)
//     {
//         if (tempStr[i] <= '0' || tempStr[i] > '0' + (int)numOfFunctionsForOptions)
//         {
//             printf("Некорректный номер операции '%c'.\n");
//             return -1;
//         }
//         chosenOptions += (int)lroundl(powl(2.0, (long double)((int)(tempStr[i] - '0') - 1)));
//     }
//
//     return chosenOptions;
// }
//
// static void CalcShowMenu()
// {
//     printf("Было прочитано выражение из файла %s. Выберите, что вы хотите сделать с этим выражением:\n"
//            "1) Вычислить\n"
//            "2) Посчитать производную заданного порядка по выбранной переменной\n"
//            "3) Разложить в ряд Тейлора в окрестности заданной точки до заданного порядка\n"
//            "4) Построить график выражения (работает только, если переменных одна или две; соответственно,\n"
//            "   будет построен 2D или 3D-график)\n"
//            "5) Построить касательную к графику в данной точке (только для 2D-режима с одной переменной)\n"
//            "6) Вычислить выражение с учётом погрешности, если известны погрешности переменных\n
//            Ваш выбор (вы можете выбрать несколько опций сразу, тогда нужно записать их подряд без пробелов): ");
//
// }
//
// static int CalcChoseOptions()
// {
//     char tempStr[SPARE_VOLUME] = "";
//     int lenOfStr = 0;
//     int result = scanf("%s%n", tempStr, &lenOfStr);
//     getchar();      // to take \n
//
//     while (result <= 0)
//     {
//         if (result == EOF)
//         {
//             printf("\nВведён символ конца файла. Программа завершена.\n");
//             return 1;
//         }
//
//         printf("Некорректный ввод. Повторите ввод (цифры записываются подряд без пробелов): ");
//         result = scanf("%s%n", tempStr, &lenOfStr);
//         getchar();      // to take \n
//     }
//
//     return CalcConvertStrOfOptionsToNum(tempStr, lenOfStr);
// }

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

// void CalcStart(MathExpression* mathExpression)
// {
//     CalcShowWelcomeMessage();
//
//     CalcShowMenu();
//     int choosenOptions = CalcChoseOptions();
//     int powerOfTwo = 1;
//     int result = 0;
//
//     for (int i = 0; i < (int)numOfFunctionsForOptions; i++)
//     {
//         if (choosenOptions & powerOfTwo)
//         {
//             result = CalcFunctionsForOption[i](mathExpression);
//         }
//
//         if (result)
//         {
//             PRINT_LOG_FILE_CALC("ERROR: An error occurred during execution option %d.\n", i + 1);
//             printf("Возникла ошибка в ходе выполнения опции %d.\n", i + 1);
//             return;
//         }
//     }
// }

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

