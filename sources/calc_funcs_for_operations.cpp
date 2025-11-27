#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include "calc_funcs_for_options.h"
#include "calc_funcs_for_operations.h"
#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_macros.h"
#include "calc_comparators.h"

// Operation operations[] =
// {
//     {0, OP_ADD,    TWO_ARGS, "+"      },
//     {0, OP_SUB,    TWO_ARGS, "-"      },
//     {0, OP_MUL,    TWO_ARGS, "*"      },
//     {0, OP_DIV,    TWO_ARGS, "/"      },
//     {0, OP_POW,    TWO_ARGS, "^"      },
//     {0, OP_LOG,    TWO_ARGS, "log"    },
//     {0, OP_LN,     ONE_ARG,  "ln"     },
//     {0, OP_SIN,    ONE_ARG,  "sin"    },
//     {0, OP_COS,    ONE_ARG,  "cos"    },
//     {0, OP_TG,     ONE_ARG,  "tg"     },
//     {0, OP_CTG,    ONE_ARG,  "ctg"    },
//     {0, OP_SH,     ONE_ARG,  "sh"     },
//     {0, OP_CH,     ONE_ARG,  "ch"     },
//     {0, OP_TH,     ONE_ARG,  "th"     },
//     {0, OP_CTH,    ONE_ARG,  "cth"    },
//     {0, OP_ARCSIN, ONE_ARG,  "arcsin" },
//     {0, OP_ARCCOS, ONE_ARG,  "arccos" },
//     {0, OP_ARCTG,  ONE_ARG,  "arctg"  },
//     {0, OP_ARCCTG, ONE_ARG,  "arcctg" }
// };

static int IsInteger(long double num)
{
    return fabsl(roundl(num) - num) < EPSILON;
}

long double CalcAdd(DataForCounting dataForCounting);
{
    long double num1 = dataForCounting.num1;
    long double num1 = dataForCounting.num2;

    return num1 + num2;
}

long double CalcSub(DataForCounting dataForCounting);
{
    long double num1 = dataForCounting.num1;
    long double num1 = dataForCounting.num2;

    return num1 - num2;
}

long double CalcMul(DataForCounting dataForCounting);
{
    long double num1 = dataForCounting.num1;
    long double num1 = dataForCounting.num2;

    return num1 * num2;
}

long double CalcDiv(DataForCounting dataForCounting);
{
    long double num1 = dataForCounting.num1;
    long double num1 = dataForCounting.num2;

    if (IsEqual(num2, 0.0))
    {
        PRINT_LOG_FILE_CALC("ERROR: Division by zero %Lg / %Lg\n", num1, num2);
        return 0.0;
    }
    return num1 / num2;
}

long double CalcPow(DataForCounting dataForCounting);
{
    long double num1 = dataForCounting.num1;
    long double num1 = dataForCounting.num2;

    if (!IsInteger(num2) && num1 < 0)
    {
        PRINT_LOG_FILE_CALC("ERROR: Raising a negative number to a fractional power %Lg ^ %Lg\n", num1, num2);
        return 0.0;
    }

    if (IsEqual(num1, 0.0) && num2 <= 0)
    {
        PRINT_LOG_FILE_CALC("ERROR: Raising 0 to a non-positive power %Lg ^ %Lg\n", num1, num2);
        return 0.0;
    }
    return powl(num1, num2);
}

long double CalcLog(DataForCounting dataForCounting);
{
    long double num1 = dataForCounting.num1;
    long double num1 = dataForCounting.num2;

    if (num2 < 0 || IsEqual(num2, 0.0) || IsEqual(num1, 1.0) || num1 < 0 || IsEqual(num1, 0.0))
    {
        PRINT_LOG_FILE_CALC("ERROR: The logarithm is meaningless for given num1 = %Lg and num2 = %Lg\n", num1, num2);
        return 0.0;
    }

    return logl(num2) / logl(num1);
}

long double CalcLn(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    if (num < 0 || IsEqual(num, 0.0))
    {
        PRINT_LOG_FILE_CALC("ERROR: The logarithm is meaningless for given num = %Lg\n", num);
        return 0.0;
    }

    return logl(num);
}

long double CalcSin(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    return sinl(num);
}

long double CalcCos(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    return cosl(num);
}

long double CalcTg(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    if (IsEqual(cosl(num), 0.0))
    {
        PRINT_LOG_FILE_CALC("ERROR: Tangent π/2 + πn, n is integer: tg(%Lg)\n", num);
        return 0.0;
    }

    return tanl(num);
}

long double CalcCtg(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    if (IsEqual(sinl(num), 0.0))
    {
        PRINT_LOG_FILE_CALC("ERROR: Cotangent πn, n is integer: ctg(%Lg)\n", num);
        return 0.0;
    }

    return 1.0 / tanl(num);
}

long double CalcSh(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    return sinhl(num);
}

long double CalcCh(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    return coshl(num);
}

long double CalcTh(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    return tanhl(num);
}

long double CalcCth(DataForCounting dataForCounting);
{
    long double num = dataForCounting.num1;

    return 1 / tanhl(num);
}

long double CalcArcsin(DataForCounting dataForCounting)
{
    long double num = dataForCounting.num1;
    if (num < -1 || num > 1)
    {
        PRINT_LOG_FILE_CALC("ERROR: Argument of arcsin() is out of range [-1; 1]: arcsin(%Lg)\n", num);
        return 0.0;
    }

}


