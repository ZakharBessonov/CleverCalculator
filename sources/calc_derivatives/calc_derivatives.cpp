#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_derivatives.h"
#include "calc_funcs_for_options.h"
#include "calc_set_get.h"
#include "calc_macros.h"

static Node* CalcNewNumNode(long double number)
{
    Node* newNumNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newNumNode);

    SetTypeNode(newNumNode, TYPE_NUMBER);
    SetNumVal(newNumNode, number);

    return newNumNode;
}

static Node* CalcNewOpNodeWithOneArg(Node* node, OperationCode operationCode)
{
    Node* newOpNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newOpNode);

    SetTypeNode(newOpNode, TYPE_OP);
    SetOperation(newOpNode, operationCode);

    SetParent(node, newOpNode);
    SetRight(newOpNode, node);
    return newOpNode;
}

static Node* CalcNewOpNodeWithTwoArgs(Node* leftNode, Node* rightNode, OperationCode operationCode)
{
    Node* newOpNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newOpNode);

    SetTypeNode(newOpNode, TYPE_OP);
    SetOperation(newOpNode, operationCode);

    SetParent(leftNode, newOpNode);
    SetParent(rightNode, newOpNode);
    SetLeft(newOpNode, leftNode);
    SetRight(newOpNode, rightNode);

    return newOpNode;
}

static Node* CopyNode(Node* oldNode)
{
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    memcpy(newNode, oldNode, sizeof(Node));
    if (GetLeft(oldNode) != NULL)
    {
        SetLeft(newNode, CopyNode(GetLeft(oldNode)));
    }

    if (GetRight(oldNode) != NULL)
    {
        SetRight(newNode, CopyNode(GetRight(oldNode)));
    }
    return newNode;
}

Node* TakeDerivativeAdd(Node* node)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    return CalcNewOpNodeWithTwoArgs(CalcDifferentiate(left, 'x'), CalcDifferentiate(right, 'x'), OP_ADD);
}
//NOTE Не закладывать заранее на множество всего

Node* TakeDerivativeSub(Node* node)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    return CalcNewOpNodeWithTwoArgs(CalcDifferentiate(left, 'x'), CalcDifferentiate(right, 'x'), OP_SUB);
}

Node* TakeDerivativeMul(Node* node)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* firstNode =  CalcNewOpNodeWithTwoArgs(CopyNode(left), CalcDifferentiate(right, 'x'), OP_MUL);
    Node* secondNode = CalcNewOpNodeWithTwoArgs(CopyNode(right), CalcDifferentiate(left, 'x'), OP_MUL);
    return CalcNewOpNodeWithTwoArgs(firstNode, secondNode, OP_ADD);
}

Node* TakeDerivativeDiv(Node* node)
{
    return NULL;
}

Node* TakeDerivativePow(Node* node)
{
    return NULL;
}

Node* TakeDerivativeLog(Node* node)
{
    return NULL;
}

Node* TakeDerivativeLn(Node* node)
{
    return NULL;
}

Node* TakeDerivativeSin(Node* node)
{
    return NULL;
}

Node* TakeDerivativeCos(Node* node)
{
    return NULL;
}

Node* TakeDerivativeTg(Node* node)
{
    return NULL;
}

Node* TakeDerivativeCtg(Node* node)
{
    return NULL;
}

Node* TakeDerivativeSh(Node* node)
{
    return NULL;
}

Node* TakeDerivativeCh(Node* node)
{
    return NULL;
}

Node* TakeDerivativeTh(Node* node)
{
    return NULL;
}

Node* TakeDerivativeCth(Node* node)
{
    return NULL;
}

Node* TakeDerivativeArcsin(Node* node)
{
    return NULL;
}

Node* TakeDerivativeArccos(Node* node)
{
    return NULL;
}

Node* TakeDerivativeArctg(Node* node)
{
    return NULL;
}

Node* TakeDerivativeArcctg(Node* node)
{
    return NULL;
}
