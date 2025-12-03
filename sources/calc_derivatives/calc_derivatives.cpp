#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <assert.h>

#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_derivatives.h"
#include "calc_funcs_for_options.h"
#include "calc_set_get.h"
#include "calc_macros.h"
#include "calc_dsl.h"

extern FILE* texFile;
extern int isNeedToWriteDerivativesInTeX;

static Node* CalcNewOpNodeWithTwoArgs(Node* leftNode, Node* rightNode, OperationCode operationCode)
{
    assert(leftNode != NULL);
    assert(rightNode != NULL);

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

static Node* CalcNewOpNodeWithOneArg(Node* node, OperationCode operationCode)
{
    assert(node != NULL);

    Node* newOpNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newOpNode);

    SetTypeNode(newOpNode, TYPE_OP);
    SetOperation(newOpNode, operationCode);

    SetParent(node, newOpNode);
    SetRight(newOpNode, node);
    return newOpNode;
}

static Node* CalcNewNumNode(long double number)
{
    Node* newNumNode = (Node*)calloc(1, sizeof(Node));
    RET_IF_NULL(newNumNode);

    SetTypeNode(newNumNode, TYPE_NUMBER);
    SetNumVal(newNumNode, number);

    return newNumNode;
}

static Node* CopyNode(Node* oldNode)
{
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    memcpy(newNode, oldNode, sizeof(Node));
    if (GetLeft(oldNode) != NULL)
    {
        SetLeft(newNode, CopyNode(GetLeft(oldNode)));
        SetParent(GetLeft(newNode), newNode);
    }

    if (GetRight(oldNode) != NULL)
    {
        SetRight(newNode, CopyNode(GetRight(oldNode)));
        SetParent(GetRight(newNode), newNode);
    }
    return newNode;
}

static int FindVarInSubTree(Node* node, char varDifferentiation)
{
    if (node == NULL)
    {
        return 0;
    }
    if (GetTypeNode(node) == TYPE_VAR && GetVarIdentifierFromNode(node) == varDifferentiation)
    {
        return 1;
    }
    else if (GetTypeNode(node) == TYPE_VAR || GetTypeNode(node) == TYPE_NUMBER)
    {
        return 0;
    }

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    int result = 0;

    if (leftSon != NULL)
    {
        result = result || FindVarInSubTree(leftSon, varDifferentiation);
    }

    if (rightSon != NULL)
    {
        result = result || FindVarInSubTree(rightSon, varDifferentiation);
    }

    return result;
}

Node* TakeDerivativeAdd(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* result = ADD_(d_(left), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}
//NOTE Не закладывать заранее на множество всего

Node* TakeDerivativeSub(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* result = SUB_(d_(left), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeMul(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* result = ADD_(MUL_(cpy_(left), d_(right)), MUL_(cpy_(right), d_(left)));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeDiv(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* helpNode = CalcNewNumNode(2);
    Node* result = DIV_(SUB_(MUL_(cpy_(right), d_(left)), MUL_(cpy_(left), d_(right))), POW_(cpy_(right), helpNode));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativePow(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* helpNode = NULL;
    Node* result = NULL;
    int IsThereVarInLeftNode = FindVarInSubTree(left, varDifferentiation);
    int IsThereVarInRightNode = FindVarInSubTree(right, varDifferentiation);

    if (!IsThereVarInLeftNode && !IsThereVarInRightNode)
    {
        result = CalcNewNumNode(0);
    }
    else if (IsThereVarInLeftNode && !IsThereVarInRightNode)
    {
        helpNode = CalcNewNumNode(1);
        result = MUL_(MUL_(cpy_(right), POW_(cpy_(left), SUB_(cpy_(right), helpNode))), d_(left));
    }
    else if (!IsThereVarInLeftNode && IsThereVarInRightNode)
    {
        result = MUL_(MUL_( d_(right), cpy_(node)), LN_(cpy_(left)));
    }
    else
    {
        result = MUL_(cpy_(node), ADD_(MUL_(d_(right), LN_(cpy_(left))), MUL_(cpy_(right), DIV_(d_(left), cpy_(left)))));
    }

    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeLog(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* helpNode1 = NULL;
    Node* helpNode2 = NULL;
    Node* result = NULL;
    int IsThereVarInLeftNode = FindVarInSubTree(left, varDifferentiation);
    int IsThereVarInRightNode = FindVarInSubTree(right, varDifferentiation);

    if (!IsThereVarInLeftNode && !IsThereVarInRightNode)
    {
        result = CalcNewNumNode(0);
    }
    else if (IsThereVarInLeftNode && !IsThereVarInRightNode)
    {
        helpNode1 = CalcNewNumNode(2);
        helpNode2 = CalcNewNumNode(-1);
        result = MUL_(helpNode2, DIV_(MUL_(LN_(cpy_(right)), d_(left)), MUL_(cpy_(left), POW_(LN_(cpy_(left)), helpNode1))));
    }
    else if (!IsThereVarInLeftNode && IsThereVarInRightNode)
    {
        result = DIV_(d_(right), MUL_(cpy_(right), LN_(cpy_(left))));
    }
    else
    {
        helpNode1 = CalcNewNumNode(2);
        result = DIV_(SUB_(MUL_(LN_(cpy_(left)), DIV_(d_(right), cpy_(right))),
                           MUL_(LN_(cpy_(right)), DIV_(d_(left), cpy_(left)))),
                      POW_(LN_(cpy_(left)), helpNode1));
    }

    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeRoot(Node* node, char varDifferentiation)
{
    Node* left = GetLeft(node);
    Node* right = GetRight(node);
    Node* helpNode1 = NULL;
    Node* helpNode2 = NULL;
    Node* result = NULL;
    int IsThereVarInLeftNode = FindVarInSubTree(left, varDifferentiation);
    int IsThereVarInRightNode = FindVarInSubTree(right, varDifferentiation);

    if (!IsThereVarInLeftNode && !IsThereVarInRightNode)
    {
        result = CalcNewNumNode(0);
    }
    else if (IsThereVarInLeftNode && !IsThereVarInRightNode)
    {
        helpNode1 = CalcNewNumNode(2);
        helpNode2 = CalcNewNumNode(-1);
        result = MUL_(MUL_(cpy_(node), MUL_(helpNode2, DIV_(d_(left), POW_(cpy_(left), helpNode1)))),
                      LN_(cpy_(right)));
    }
    else if (!IsThereVarInLeftNode && IsThereVarInRightNode)
    {
        helpNode1 = CalcNewNumNode(1);
        helpNode2 = CalcNewNumNode(1);
        result = DIV_(MUL_(POW_(cpy_(right), SUB_(DIV_(helpNode1, cpy_(left)), helpNode2)), d_(right)), cpy_(left));
    }
    else
    {
        helpNode1 = CalcNewNumNode(2);
        result = MUL_(cpy_(node), SUB_(DIV_(d_(right), MUL_(cpy_(left), cpy_(right))),
                                       MUL_(DIV_(d_(left), POW_(cpy_(left), helpNode1)), LN_(cpy_(right)))));
    }

    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeSqrt(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(0.5);
    Node* helpNode2 = CalcNewNumNode(-0.5);
    Node* result = MUL_(MUL_(helpNode1, POW_(cpy_(right), helpNode2)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeLn(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode = CalcNewNumNode(1);
    Node* result = MUL_(DIV_(helpNode, cpy_(right)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeSin(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* result = MUL_(COS_(cpy_(right)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeCos(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode = CalcNewNumNode(-1);
    Node* result = MUL_(MUL_(SIN_(cpy_(right)), helpNode), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeTg(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(1);
    Node* helpNode2 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, POW_(COS_(cpy_(right)), helpNode2)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeCtg(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(-1);
    Node* helpNode2 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, POW_(SIN_(cpy_(right)), helpNode2)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeSh(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* result = MUL_(CH_(cpy_(right)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeCh(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* result = MUL_(SH_(cpy_(right)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeTh(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(1);
    Node* helpNode2 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, POW_(CH_(cpy_(right)), helpNode2)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeCth(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(-1);
    Node* helpNode2 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, POW_(SH_(cpy_(right)), helpNode2)), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeArcsin(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(1);
    Node* helpNode2 = CalcNewNumNode(1);
    Node* helpNode3 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, SQRT_(SUB_(helpNode2, POW_(cpy_(right), helpNode3)))), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeArccos(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(-1);
    Node* helpNode2 = CalcNewNumNode(1);
    Node* helpNode3 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, SQRT_(SUB_(helpNode2, POW_(cpy_(right), helpNode3)))), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeArctg(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(1);
    Node* helpNode2 = CalcNewNumNode(1);
    Node* helpNode3 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, ADD_(helpNode2, POW_(cpy_(right), helpNode3))), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}

Node* TakeDerivativeArcctg(Node* node, char varDifferentiation)
{
    Node* right = GetRight(node);
    Node* helpNode1 = CalcNewNumNode(-1);
    Node* helpNode2 = CalcNewNumNode(1);
    Node* helpNode3 = CalcNewNumNode(2);
    Node* result = MUL_(DIV_(helpNode1, ADD_(helpNode2, POW_(cpy_(right), helpNode3))), d_(right));
    $(WriteDerivative(node, result, varDifferentiation));
    return result;
}
