#include "calc_structs.h"
#include "calc_funcs_for_options.h"
#include "calc_funcs_for_operations.h"
#include "calc_funcs_for_writing_to_TeX.h"
#include "calc_derivatives.h"
#include "calc_simplification.h"

Operation operations[] =
{
    {0, OP_ADD,    TWO_ARGS, "+"      , CalcAdd    , WriteAddInTeX    , TakeDerivativeAdd    , RemoveNeutElemAdd},
    {0, OP_SUB,    TWO_ARGS, "-"      , CalcSub    , WriteSubInTeX    , TakeDerivativeSub    , RemoveNeutElemSub},
    {0, OP_MUL,    TWO_ARGS, "*"      , CalcMul    , WriteMulInTeX    , TakeDerivativeMul    , RemoveNeutElemMul},
    {0, OP_DIV,    TWO_ARGS, "/"      , CalcDiv    , WriteDivInTeX    , TakeDerivativeDiv    , RemoveNeutElemDiv},
    {0, OP_POW,    TWO_ARGS, "^"      , CalcPow    , WritePowInTeX    , TakeDerivativePow    , RemoveNeutElemPow},
    {0, OP_LOG,    TWO_ARGS, "log"    , CalcLog    , WriteLogInTeX    , TakeDerivativeLog    , RemoveNeutElemLog},
    {0, OP_LN,     ONE_ARG,  "ln"     , CalcLn     , WriteLnInTeX     , TakeDerivativeLn     , NULL             },
    {0, OP_SIN,    ONE_ARG,  "sin"    , CalcSin    , WriteSinInTeX    , TakeDerivativeSin    , NULL             },
    {0, OP_COS,    ONE_ARG,  "cos"    , CalcCos    , WriteCosInTeX    , TakeDerivativeCos    , NULL             },
    {0, OP_TG,     ONE_ARG,  "tg"     , CalcTg     , WriteTgInTeX     , TakeDerivativeTg     , NULL             },
    {0, OP_CTG,    ONE_ARG,  "ctg"    , CalcCtg    , WriteCtgInTeX    , TakeDerivativeCtg    , NULL             },
    {0, OP_SH,     ONE_ARG,  "sh"     , CalcSh     , WriteShInTeX     , TakeDerivativeSh     , NULL             },
    {0, OP_CH,     ONE_ARG,  "ch"     , CalcCh     , WriteChInTeX     , TakeDerivativeCh     , NULL             },
    {0, OP_TH,     ONE_ARG,  "th"     , CalcTh     , WriteThInTeX     , TakeDerivativeTh     , NULL             },
    {0, OP_CTH,    ONE_ARG,  "cth"    , CalcCth    , WriteCthInTeX    , TakeDerivativeCth    , NULL             },
    {0, OP_ARCSIN, ONE_ARG,  "arcsin" , CalcArcsin , WriteArcsinInTeX , TakeDerivativeArcsin , NULL             },
    {0, OP_ARCCOS, ONE_ARG,  "arccos" , CalcArccos , WriteArccosInTeX , TakeDerivativeArccos , NULL             },
    {0, OP_ARCTG,  ONE_ARG,  "arctg"  , CalcArctg  , WriteArctgInTeX  , TakeDerivativeArctg  , NULL             },
    {0, OP_ARCCTG, ONE_ARG,  "arcctg" , CalcArcctg , WriteArcctgInTeX , TakeDerivativeArcctg , NULL             }
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
