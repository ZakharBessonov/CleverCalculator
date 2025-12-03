#ifndef CALC_DSL_HEADER
#define CALC_DSL_HEADER

#define cpy_(X)        CopyNode(X)
#define d_(X)          CalcDifferentiate(X, varDifferentiation)
#define ADD_(X, Y)     CalcNewOpNodeWithTwoArgs(X, Y, OP_ADD)
#define SUB_(X, Y)     CalcNewOpNodeWithTwoArgs(X, Y, OP_SUB)
#define MUL_(X, Y)     CalcNewOpNodeWithTwoArgs(X, Y, OP_MUL)
#define DIV_(X, Y)     CalcNewOpNodeWithTwoArgs(X, Y, OP_DIV)
#define POW_(X, Y)     CalcNewOpNodeWithTwoArgs(X, Y, OP_POW)
#define LOG_(X, Y)     CalcNewOpNodeWithTwoArgs(X, Y, OP_LOG)
#define LN_(X)         CalcNewOpNodeWithOneArg(X, OP_LN)
#define SIN_(X)        CalcNewOpNodeWithOneArg(X, OP_SIN)
#define COS_(X)        CalcNewOpNodeWithOneArg(X, OP_COS)
#define TG_(X)         CalcNewOpNodeWithOneArg(X, OP_TG)
#define CTG_(X)        CalcNewOpNodeWithOneArg(X, OP_CTG)
#define SH_(X)         CalcNewOpNodeWithOneArg(X, OP_SH)
#define CH_(X)         CalcNewOpNodeWithOneArg(X, OP_CH)
#define TH_(X)         CalcNewOpNodeWithOneArg(X, OP_TH)
#define CTH_(X)        CalcNewOpNodeWithOneArg(X, OP_CTH)
#define ARCSIN_(X)     CalcNewOpNodeWithOneArg(X, OP_ARCSIN)
#define ARCCOS_(X)     CalcNewOpNodeWithOneArg(X, OP_ARCCOS)
#define ARCTG_(X)      CalcNewOpNodeWithOneArg(X, OP_ARCTG)
#define ARCCTG_(X)     CalcNewOpNodeWithOneArg(X, OP_ARCCTG)

#endif
