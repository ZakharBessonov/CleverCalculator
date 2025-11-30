#include "calc_structs.h"
#include "calc_funcs_for_options.h"
#include "calc_funcs_for_operations.h"
#include "calc_funcs_for_writing_to_TeX.h"
#include "calc_derivatives.h"

Operation operations[] =
{
    {0, OP_ADD,    TWO_ARGS, "+"      , CalcAdd    , WriteAddInTeX    , TakeDerivativeAdd    },
    {0, OP_SUB,    TWO_ARGS, "-"      , CalcSub    , WriteSubInTeX    , TakeDerivativeSub    },
    {0, OP_MUL,    TWO_ARGS, "*"      , CalcMul    , WriteMulInTeX    , TakeDerivativeMul    },
    {0, OP_DIV,    TWO_ARGS, "/"      , CalcDiv    , WriteDivInTeX    , TakeDerivativeDiv    },
    {0, OP_POW,    TWO_ARGS, "^"      , CalcPow    , WritePowInTeX    , TakeDerivativePow    },
    {0, OP_LOG,    TWO_ARGS, "log"    , CalcLog    , WriteLogInTeX    , TakeDerivativeLog    },
    {0, OP_LN,     ONE_ARG,  "ln"     , CalcLn     , WriteLnInTeX     , TakeDerivativeLn     },
    {0, OP_SIN,    ONE_ARG,  "sin"    , CalcSin    , WriteSinInTeX    , TakeDerivativeSin    },
    {0, OP_COS,    ONE_ARG,  "cos"    , CalcCos    , WriteCosInTeX    , TakeDerivativeCos    },
    {0, OP_TG,     ONE_ARG,  "tg"     , CalcTg     , WriteTgInTeX     , TakeDerivativeTg     },
    {0, OP_CTG,    ONE_ARG,  "ctg"    , CalcCtg    , WriteCtgInTeX    , TakeDerivativeCtg    },
    {0, OP_SH,     ONE_ARG,  "sh"     , CalcSh     , WriteShInTeX     , TakeDerivativeSh     },
    {0, OP_CH,     ONE_ARG,  "ch"     , CalcCh     , WriteChInTeX     , TakeDerivativeCh     },
    {0, OP_TH,     ONE_ARG,  "th"     , CalcTh     , WriteThInTeX     , TakeDerivativeTh     },
    {0, OP_CTH,    ONE_ARG,  "cth"    , CalcCth    , WriteCthInTeX    , TakeDerivativeCth    },
    {0, OP_ARCSIN, ONE_ARG,  "arcsin" , CalcArcsin , WriteArcsinInTeX , TakeDerivativeArcsin },
    {0, OP_ARCCOS, ONE_ARG,  "arccos" , CalcArccos , WriteArccosInTeX , TakeDerivativeArccos },
    {0, OP_ARCTG,  ONE_ARG,  "arctg"  , CalcArctg  , WriteArctgInTeX  , TakeDerivativeArctg  },
    {0, OP_ARCCTG, ONE_ARG,  "arcctg" , CalcArcctg , WriteArcctgInTeX , TakeDerivativeArcctg }
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
