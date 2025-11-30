#include <stdio.h>
#include <stdlib.h>

#include "calc_structs.h"
#include "calc_simplification.h"
#include "calc_consts.h"
#include "calc_comparators.h"
#include "calc_set_get.h"

extern FILE* logfileCalc;
extern char* ioFileName;
extern Operation operations[];
extern size_t numOfOperations;

void CalcSimplifyExpression(MathExpression* mathExpression)
{
    int wereAnyChanges = 1;
    OperationCode operationCode = OP_ADD;
    Node* root = NULL;
    Node* tempNode = NULL;

    while (wereAnyChanges--)
    {
        root = GetRoot(mathExpression);

        operationCode = GetOperation(root);

        tempNode = CalcConstantFolding(root, &wereAnyChanges);
        if (tempNode != NULL)
        {
            SetRoot(mathExpression, tempNode);
        }
    }
}

Node* CalcConstantFolding(Node* node, int* breaker)
{
    if (node == NULL)
    {
        return NULL;
    }

    NodeType type = GetTypeNode(node);
    if (type == TYPE_NUMBER)
    {
        long double numDebug = 0.0;
        GetNumVal(node, &numDebug);
        printf("Дошли до числа %Lg\n", numDebug);
        return node;
    }

    if (type == TYPE_VAR)
    {
        printf("Дошли до переменной %c\n", GetVarIdentifierFromNode(node));
        return NULL;
    }

    OperationCode operationCode = GetOperation(node);
    printf("Дошли до функции %s\n", operations[operationCode].spellingOfOperation);
    Node* trySimplifyLeft = CalcConstantFolding(GetLeft(node), breaker);
    Node* trySimplifyRight = CalcConstantFolding(GetRight(node), breaker);

    if (operations[operationCode].numOfArgs == ONE_ARG)
    {
        if (trySimplifyRight != NULL)
        {
            *breaker = 1;
            long double num = 0.0;
            GetNumVal(trySimplifyRight, &num);
            long double result = operations[operationCode].funcForOperation({num});
            printf("Упростили до %Lg\n", result);
            SetNumVal(trySimplifyRight, result);
            SetParent(trySimplifyRight, GetParent(node));

            SetTypeNode(node, TYPE_NUMBER);
            SetNumVal(node, result);
            free(GetRight(node));
            SetRight(node, NULL);
            return node;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if (trySimplifyRight != NULL && trySimplifyLeft != NULL)
        {
            *breaker = 1;
            long double num1 = 0.0;
            long double num2 = 0.0;
            GetNumVal(trySimplifyLeft, &num1);
            GetNumVal(trySimplifyRight, &num2);
            long double result = operations[operationCode].funcForOperation({num1, num2});
            printf("Упростили до %Lg\n", result);
            SetNumVal(trySimplifyRight, result);
            SetParent(trySimplifyRight, GetParent(node));

            SetTypeNode(node, TYPE_NUMBER);
            SetNumVal(node, result);
            free(GetRight(node));
            free(GetLeft(node));
            SetRight(node, NULL);
            SetLeft(node, NULL);
            return node;
        }
        else
        {
            return NULL;
        }
    }
}

Node* CalcRemovingNeutralElements(MathExpression* mathExpression, Node* node, int* breaker)
{
    return NULL;
}


