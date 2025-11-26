#ifndef CALC_STRUCTS_HEADER
#define CALC_STRUCTS_HEADER

#include <stdio.h>

enum NodeType
{
    TYPE_OP,
    TYPE_VAR,
    TYPE_NUMBER
};

enum OperationCode
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_LOG,
    OP_LN,
    OP_SIN,
    OP_COS,
    OP_TG,
    OP_CTG,
    OP_SH,
    OP_CH,
    OP_TH,
    OP_CTH,
    OP_ARCSIN,
    OP_ARCCOS,
    OP_ARCTG,
    OP_ARCCTG
};

enum NumOfArgs
{
    ONE_ARG,
    TWO_ARGS
};

struct Operation
{
    unsigned long hashOfOperation;
    OperationCode operationCode;
    NumOfArgs     numOfArgs;
    const char*   spellingOfOperation;
};

union Value
{
    long double   numVal;
    char          varIdentifier;
    OperationCode operationCode;
};

struct Node
{
    NodeType  type;
    Value     val;
    Node*     parent;
    Node*     right;
    Node*     left;
};

struct Variable
{
    char         identifier;
    long double  value;
};

struct MathExpression
{
    Node*      root;
    char*      buffer;
    size_t     lenOfBuffer;
    int        fileCounter;
    Variable*  variables;
    int        variablesCounter;
};

enum CalcError
{
    CALC_OK = 0,
    CALC_ERROR_NULL_NODE_POINTER = 1,
    CALC_ERROR_NULL_MATH_EXPRESSION_POINTER = 2,
};

enum Sons
{
    LEFT = 1,
    RIGHT = 2
};

enum Answer
{
    NO_ANSWER,
    YES,
    NO
};

#endif
