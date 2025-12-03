#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "calc_structs.h"
#include "calc_simplification.h"
#include "calc_consts.h"
#include "calc_comparators.h"
#include "calc_set_get.h"
#include "calc_general_funcs.h"
#include "calc_dump.h"
#include "calc_macros.h"

extern FILE* logfileCalc;
extern char* ioFileName;
extern Operation operations[];
extern size_t numOfOperations;
extern FILE* texFile;

static void CopyNode(Node* dst, Node* src)
{
    assert(dst != NULL);

    SetParent(dst, GetParent(src));
    SetRight(dst, GetRight(src));
    SetLeft(dst, GetLeft(src));
    SetTypeNode(dst, GetTypeNode(src));
    Value tempValue = {};
    GetValue(src, &tempValue);
    SetValue(dst, tempValue);
    Node* leftSon = GetLeft(src);
    Node* rightSon = GetRight(src);

    if (leftSon != NULL)
    {
        SetParent(leftSon, dst);
    }

    if (rightSon != NULL)
    {
        SetParent(rightSon, dst);
    }
}

void CalcSimplifyExpression(MathExpression* mathExpression)
{
    int wereAnyChanges = 1;
    Node* root = NULL;

    while (wereAnyChanges--)
    {
        root = GetRoot(mathExpression);

        CalcConstantFolding(root, &wereAnyChanges);

        CalcRemovingNeutralElements(root, &wereAnyChanges);
        //CALL_DUMP(mathExpression, "After RemovingNeutralElements");
    }
}

void CalcRemovingNeutralElements(Node* node, int* breaker)
{
    if (GetTypeNode(node) != TYPE_OP)
    {
        return;
    }

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_OP)
    {
        CalcRemovingNeutralElements(leftSon, breaker);
    }

    if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_OP)
    {
        CalcRemovingNeutralElements(rightSon, breaker);
    }

    OperationCode operationCode = GetOperation(node);
    if (operations[operationCode].funcForRemoveNeutralElem != NULL)
    {
        operations[operationCode].funcForRemoveNeutralElem(node, breaker);
    }
}

static void TryFoldingNodeWithOneArg(Node* node, int* breaker)
{
    Node* rightSon = GetRight(node);
    OperationCode operationCode = GetOperation(node);
    if (GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        *breaker = 1;
        long double num = 0.0;
        GetNumVal(rightSon, &num);
        long double result = operations[operationCode].funcForOperation({num});
        SetTypeNode(node, TYPE_NUMBER);
        SetNumVal(node, result);
        free(rightSon);
        SetRight(node, NULL);
    }
}

static void TryFoldingNodeWithTwoArgs(Node* node, int* breaker)
{
    Node* rightSon = GetRight(node);
    Node* leftSon = GetLeft(node);
    OperationCode operationCode = GetOperation(node);
    if (GetTypeNode(rightSon) == TYPE_NUMBER && GetTypeNode(leftSon) == TYPE_NUMBER)
    {
        *breaker = 1;
        long double num1 = 0.0;
        long double num2 = 0.0;
        GetNumVal(leftSon, &num1);
        GetNumVal(rightSon, &num2);
        long double result = operations[operationCode].funcForOperation({num1, num2});
        SetTypeNode(node, TYPE_NUMBER);
        SetNumVal(node, result);
        free(rightSon);
        free(leftSon);
        SetRight(node, NULL);
        SetLeft(node, NULL);
    }
}

void CalcConstantFolding(Node* node, int* breaker)
{
    if (GetTypeNode(node) != TYPE_OP)
    {
        return;
    }

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_OP)
    {
        CalcConstantFolding(leftSon, breaker);
    }

    if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_OP)
    {
        CalcConstantFolding(rightSon, breaker);
    }

    if (leftSon == NULL)
    {
        TryFoldingNodeWithOneArg(node, breaker);
    }
    else
    {
        TryFoldingNodeWithTwoArgs(node, breaker);
    }
}

// Func for removing neutral elements

void RemoveNeutElemAdd(Node* node, int* breaker)
{
    assert(node != NULL);

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    Node* tempParent = NULL;
    long double tempVal = 0.0;

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_NUMBER)
    {
        GetNumVal(leftSon, &tempVal);
        if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            free(leftSon);
            leftSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, rightSon);
            free(rightSon);
            rightSon = NULL;
            SetParent(node, tempParent);


        }
    }
    else if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        GetNumVal(rightSon, &tempVal);
        if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            free(rightSon);
            rightSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            free(leftSon);
            leftSon = NULL;
            SetParent(node, tempParent);
        }
    }
}

void RemoveNeutElemSub(Node* node, int* breaker)
{
    assert(node != NULL);

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    Node* tempParent = NULL;
    long double tempVal = 0.0;

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_NUMBER)
    {
        GetNumVal(leftSon, &tempVal);
        if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            SetNumVal(leftSon, -1);
            SetOperation(node, OP_MUL);
        }
    }
    else if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        GetNumVal(rightSon, &tempVal);
        if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            free(rightSon);
            rightSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            free(leftSon);
            leftSon = NULL;
            SetParent(node, tempParent);
        }
    }
}

void RemoveNeutElemMul(Node* node, int* breaker)
{
    assert(node != NULL);

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    Node* tempParent = NULL;
    long double tempVal = 0.0;

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_NUMBER)
    {
        GetNumVal(leftSon, &tempVal);
        if (IsEqual(tempVal, 1.0))
        {
            *breaker = 1;
            free(leftSon);
            leftSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, rightSon);
            free(rightSon);
            rightSon = NULL;
            SetParent(node, tempParent);
        }
        else if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            SetParent(node, tempParent);
            free(leftSon);
            leftSon = NULL;
            NodeFree(rightSon);
            rightSon = NULL;
        }
    }
    else if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        GetNumVal(rightSon, &tempVal);
        if (IsEqual(tempVal, 1.0))
        {
            *breaker = 1;
            free(rightSon);
            rightSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            free(leftSon);
            leftSon = NULL;
            SetParent(node, tempParent);
        }
        else if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            tempParent = GetParent(node);
            CopyNode(node, rightSon);
            SetParent(node, tempParent);
            free(rightSon);
            rightSon = NULL;
            NodeFree(leftSon);
            leftSon = NULL;
        }
    }
}

void RemoveNeutElemDiv(Node* node, int* breaker)
{
    assert(node != NULL);

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    Node* tempParent = NULL;
    long double tempVal = 0.0;

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_NUMBER)
    {
        GetNumVal(leftSon, &tempVal);
        if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            SetParent(node, tempParent);
            free(leftSon);
            leftSon = NULL;
            NodeFree(rightSon);
        }
    }
    else if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        GetNumVal(rightSon, &tempVal);
        if (IsEqual(tempVal, 1.0))
        {
            *breaker = 1;
            free(rightSon);
            rightSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            free(leftSon);
            leftSon = NULL;
            SetParent(node, tempParent);
        }
    }
}

void RemoveNeutElemPow(Node* node, int* breaker)
{
    assert(node != NULL);

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    Node* tempParent = NULL;
    long double tempVal = 0.0;

    if (leftSon != NULL && GetTypeNode(leftSon) == TYPE_NUMBER)
    {
        GetNumVal(leftSon, &tempVal);
        if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            SetParent(node, tempParent);
            free(leftSon);
            leftSon = NULL;
            NodeFree(rightSon);
            rightSon = NULL;
        }
        else if (IsEqual(tempVal, 1.0))
        {
            *breaker = 1;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            SetParent(node, tempParent);
            free(leftSon);
            leftSon = NULL;
            NodeFree(rightSon);
            rightSon = NULL;
        }
    }
    else if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        GetNumVal(rightSon, &tempVal);
        if (IsEqual(tempVal, 1.0))
        {
            *breaker = 1;
            free(rightSon);
            rightSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, leftSon);
            free(leftSon);
            leftSon = NULL;
            SetParent(node, tempParent);
        } else if (IsEqual(tempVal, 0.0))
        {
            *breaker = 1;
            tempParent = GetParent(node);
            CopyNode(node, rightSon);
            SetNumVal(node, 1);
            SetParent(node, tempParent);
            free(rightSon);
            rightSon = NULL;
            NodeFree(leftSon);
            leftSon = NULL;
        }
    }
}

void RemoveNeutElemLog(Node* node, int* breaker)
{
    assert(node != NULL);

    Node* leftSon = GetLeft(node);
    Node* rightSon = GetRight(node);
    Node* tempParent = NULL;
    long double tempVal = 0.0;

    if (rightSon != NULL && GetTypeNode(rightSon) == TYPE_NUMBER)
    {
        GetNumVal(rightSon, &tempVal);
        if (IsEqual(tempVal, 1.0))
        {
            *breaker = 1;
            free(leftSon);
            leftSon = NULL;
            tempParent = GetParent(node);
            CopyNode(node, rightSon);
            SetNumVal(node, 0);
            free(rightSon);
            rightSon = NULL;
            SetParent(node, tempParent);
        }
    }
}
