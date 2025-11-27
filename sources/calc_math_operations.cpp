#include "calc_structs.h"
//#include "calc_funcs_for_options.h"
#include "calc_funcs_for_operations.h"

Operation operations[] =
{
    {0, OP_ADD,    TWO_ARGS, "+"      },
    {0, OP_SUB,    TWO_ARGS, "-"      },
    {0, OP_MUL,    TWO_ARGS, "*"      },
    {0, OP_DIV,    TWO_ARGS, "/"      },
    {0, OP_POW,    TWO_ARGS, "^"      },
    {0, OP_LOG,    TWO_ARGS, "log"    },
    {0, OP_LN,     ONE_ARG,  "ln"     },
    {0, OP_SIN,    ONE_ARG,  "sin"    },
    {0, OP_COS,    ONE_ARG,  "cos"    },
    {0, OP_TG,     ONE_ARG,  "tg"     },
    {0, OP_CTG,    ONE_ARG,  "ctg"    },
    {0, OP_SH,     ONE_ARG,  "sh"     },
    {0, OP_CH,     ONE_ARG,  "ch"     },
    {0, OP_TH,     ONE_ARG,  "th"     },
    {0, OP_CTH,    ONE_ARG,  "cth"    },
    {0, OP_ARCSIN, ONE_ARG,  "arcsin" },
    {0, OP_ARCCOS, ONE_ARG,  "arccos" },
    {0, OP_ARCTG,  ONE_ARG,  "arctg"  },
    {0, OP_ARCCTG, ONE_ARG,  "arcctg" }
};

size_t numOfOperations = sizeof(operations) / sizeof(*operations);

int (*CalcFunctionsForOption[])(MathExpression*) =
{
    // CalcCountExpression,
    // CalcCountDerivative,
    // CalcCountTaylorSeries,
    // CalcPlotGraph,
    // CalcPlotTangentToGraph,
    // CalcCountExpressionTakingIntoAccountTheError
};

size_t numOfFunctionsForOptions = sizeof(CalcFunctionsForOption) / sizeof(*CalcFunctionsForOption);
