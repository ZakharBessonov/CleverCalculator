#include "calc_structs.h"
#include "calc_funcs_for_options.h"
#include "calc_funcs_for_operations.h"

Operation operations[] =
{
    {0, OP_ADD,    TWO_ARGS, "+"      , CalcAdd    },
    {0, OP_SUB,    TWO_ARGS, "-"      , CalcSub    },
    {0, OP_MUL,    TWO_ARGS, "*"      , CalcMul    },
    {0, OP_DIV,    TWO_ARGS, "/"      , CalcDiv    },
    {0, OP_POW,    TWO_ARGS, "^"      , CalcPow    },
    {0, OP_LOG,    TWO_ARGS, "log"    , CalcLog    },
    {0, OP_LN,     ONE_ARG,  "ln"     , CalcLn     },
    {0, OP_SIN,    ONE_ARG,  "sin"    , CalcSin    },
    {0, OP_COS,    ONE_ARG,  "cos"    , CalcCos    },
    {0, OP_TG,     ONE_ARG,  "tg"     , CalcTg     },
    {0, OP_CTG,    ONE_ARG,  "ctg"    , CalcCtg    },
    {0, OP_SH,     ONE_ARG,  "sh"     , CalcSh     },
    {0, OP_CH,     ONE_ARG,  "ch"     , CalcCh     },
    {0, OP_TH,     ONE_ARG,  "th"     , CalcTh     },
    {0, OP_CTH,    ONE_ARG,  "cth"    , CalcCth    },
    {0, OP_ARCSIN, ONE_ARG,  "arcsin" , CalcArcsin },
    {0, OP_ARCCOS, ONE_ARG,  "arccos" , CalcArccos },
    {0, OP_ARCTG,  ONE_ARG,  "arctg"  , CalcArctg  },
    {0, OP_ARCCTG, ONE_ARG,  "arcctg" , CalcArcctg }
};

size_t numOfOperations = sizeof(operations) / sizeof(*operations);

int (*CalcFunctionsForOption[])(MathExpression*) =
{
    CalcCountExpression,
    CalcCountDerivative,
    CalcCountTaylorSeries,
    CalcPlotGraph,
    CalcPlotTangentToGraph,
    CalcCountExpressionTakingIntoAccountTheError
};

size_t numOfFunctionsForOptions = sizeof(CalcFunctionsForOption) / sizeof(*CalcFunctionsForOption);
