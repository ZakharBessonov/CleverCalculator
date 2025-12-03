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
extern int (*CalcFunctionsForOption[])(MathExpression*);
extern size_t numOfFunctionsForOptions;
extern char* ioFileName;
extern FILE* texFile;

static void CalcShowWelcomeMessage()
{
    cprintf(CYAN, "Умный Калькулятор 1.0.0\n");
}

static void CalcShowMenu()
{
    cprintf(CYAN, "Было прочитано выражение из файла %s. Выберите, что вы хотите сделать с этим выражением:\n"
            "1) Вычислить\n"
            "2) Посчитать производную заданного порядка по выбранной переменной\n"
            "3) Разложить в ряд Тейлора в окрестности заданной точки до заданного порядка (в случае одной переменной)\n"
            "   и построить график функции и её тейлоровского разложения\n"
            "4) Построить график выражения (только, если переменная одна)\n"
            "5) Построить касательную к графику в данной точке (только, если переменная одна)\n"
            "6) Вычислить выражение с учётом погрешности, если известны погрешности переменных\n"
            "7) Выход\n"
            "Ваш выбор: ",
            ioFileName);

}

static int CalcChoseOption()
{
    int chosenOption = 0;
    int result = scanf("%d", &chosenOption);
    while (getchar() != '\n');

    while (result <= 0 || !(chosenOption >= 1 && chosenOption <= (int)numOfFunctionsForOptions + 1))
    {
        if (result == EOF)
        {
            cprintf(RED, "\nВведён символ конца файла. Программа завершена.\n");
            return -1;
        }

        cprintf(RED, "Некорректный ввод. Повторите ввод: ");
        result = scanf("%d", &chosenOption);
        while (getchar() != '\n');      // to take \n
    }

    return chosenOption;
}

void CalcCtor(MathExpression* mathExpression)
{
    SetRoot(mathExpression, NULL);
    SetBufferPointer(mathExpression, NULL);
    SetLenOfBuffer(mathExpression, 0);
    mathExpression->fileCounter = (int*)calloc(1, sizeof(int));
    SetFileCounter(mathExpression, 0);

    for (int i = 0; i < MAX_NUM_OF_VARIABLES; i++)
    {
        mathExpression->variables[i].value = NAN;
    }

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
    SetLenOfBuffer(mathExpression, 0);
    if (GetRoot(mathExpression) != NULL)
    {
        NodeFree(GetRoot(mathExpression));
    }
    SetRoot(mathExpression, NULL);

}

void FileCounterDtor(MathExpression* mathExpression)
{
    free(mathExpression->fileCounter);
}

void CalcStart(MathExpression* mathExpression)
{
    CalcShowWelcomeMessage();

    CalcShowMenu();
    int choosenOptions = CalcChoseOption();
    int result = 0;

    while (choosenOptions != (int)numOfFunctionsForOptions + 1)
    {
        if (choosenOptions == EOF || choosenOptions <= -1)
        {
            return;
        }

        result = CalcFunctionsForOption[choosenOptions - 1](mathExpression);

        if (result == 1)
        {
            PRINT_LOG_FILE_CALC("ERROR: An error occurred during execution option %d.\n", choosenOptions);
            cprintf(RED, "Возникла ошибка в ходе выполнения опции %d.\n", choosenOptions);
            return;
        }

        CalcShowMenu();
        choosenOptions = CalcChoseOption();
    }

    printf("До новых встреч!\n");
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

