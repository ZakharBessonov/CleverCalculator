#include <stdio.h>
#include <math.h>

#include "calc_structs.h"
#include "calc_macros.h"

extern FILE* logfileCalc;

int GetVariableIndex(char variable)
{
    if (variable >= 'A' && variable <= 'Z')
    {
        return variable - 'A';
    }
    else if (variable >= 'a' && variable <= 'z')
    {
        return ('Z' - 'A' + 1) + variable - 'a';
    }
    else
    {
        PRINT_LOG_FILE_CALC("ERROR: '%c' is not name of variable.", variable);
        return -1;
    }
}

char GetVariableSpelling(int index)
{
    if (index >= 0 && index < 26)
    {
        return (char)('A' + index);
    }
    else if (index >= 26 && index < 52)
    {
        return (char)('a' + index - 26);
    }
    else
    {
        PRINT_LOG_FILE_CALC("ERROR: Variable index '%d' out of range.", index);
        return '\0';
    }
}

// Getters

//NOTE: Заменить ифы на ассерты

Node* GetRight(Node* node)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return NULL;
    }

    return node->right;
}

Node* GetLeft(Node* node)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return NULL;
    }

    return node->left;
}

Node* GetParent(Node* node)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return NULL;
    }

    return node->parent;
}

Node* GetRoot(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return NULL;
    }

    return mathExpression->root;
}

char* GetBufferPointer(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return NULL;
    }

    return mathExpression->buffer;
}

size_t* GetLenOfBuffer(MathExpression* mathExpression, size_t* target)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return NULL;
    }

    *target = mathExpression->lenOfBuffer;
    return target;
}

int GetFileCounter(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return -1;
    }

    return *(mathExpression->fileCounter);
}

Variable* GetVariablesPointer(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return NULL;
    }

    return mathExpression->variables;
}

Variable GetVariable(MathExpression* mathExpression, char variable)
{
    int index = GetVariableIndex(variable);
    if (index == -1)
    {
        return {'\0', NAN};
    }
    return mathExpression->variables[index];
}

long double GetVariableValue(MathExpression* mathExpression, char variable)
{
    int index = GetVariableIndex(variable);
    if (index == -1)
    {
        return NAN;
    }

    return mathExpression->variables[index].value;
}

char GetVarIdentifierFromArrayOfVars(MathExpression* mathExpression, char varIdentifier)
{
    int index = GetVariableIndex(varIdentifier);
    if (index == -1)
    {
        return '\0';
    }
    return mathExpression->variables[index].identifier;
}

int GetVariablesCounter(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return -1;
    }

    return mathExpression->variablesCounter;
}

Value* GetValue(Node* node, Value* target)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: node == NULL.");
        return NULL;
    }

    *target = node->val;
    return target;
}

long double* GetNumVal(Node* node, long double* target)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: node == NULL.");
        return NULL;
    }

    *target = node->val.numVal;
    return target;
}

char GetVarIdentifierFromNode(Node* node)
{
    return node->val.varIdentifier;
}

OperationCode GetOperation(Node* node)
{
    return node->val.operationCode;
}

NodeType GetTypeNode(Node* node)
{
    if (node == NULL)
    {
       PRINT_LOG_FILE_CALC("ERROR: Node %p == NULL.", node);
    }
    return node->type;
}

// Setters

int SetRight(Node* node, Node* newRight)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->right = newRight;
    return CALC_OK;
}

int SetLeft(Node* node, Node* newLeft)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->left = newLeft;
    return CALC_OK;
}

int SetParent(Node* node, Node* newParent)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->parent = newParent;
    return CALC_OK;
}

int SetRoot(MathExpression* mathExpression, Node* newRoot)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    mathExpression->root = newRoot;
    return CALC_OK;
}

int SetBufferPointer(MathExpression* mathExpression, char* newPointer)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    mathExpression->buffer = newPointer;
    return CALC_OK;
}

int SetLenOfBuffer(MathExpression* mathExpression, size_t newLength)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    mathExpression->lenOfBuffer = newLength;
    return CALC_OK;
}

int SetFileCounter(MathExpression* mathExpression, int newFileCounter)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    *(mathExpression->fileCounter) = newFileCounter;
    return CALC_OK;
}

int IncrementFileCounter(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    (*(mathExpression->fileCounter))++;
    return CALC_OK;
}

int SetVariableValue(MathExpression* mathExpression, char variable, long double newVarValue)
{
    int index = GetVariableIndex(variable);
    if (index == -1)
    {
        return CALC_NON_EXISTED_VARIABLE;
    }
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    mathExpression->variables[index].value = newVarValue;
    return CALC_OK;
}

int SetVariablesCounter(MathExpression* mathExpression, int newVariablesCounter)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    mathExpression->variablesCounter = newVariablesCounter;
    return CALC_OK;
}

int IncrementVariablesCounter(MathExpression* mathExpression)
{
    if (mathExpression == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: mathExpression == NULL.");
        return CALC_ERROR_NULL_MATH_EXPRESSION_POINTER;
    }

    mathExpression->variablesCounter++;
    return CALC_OK;
}

int SetValue(Node* node, Value newValue)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->val = newValue;
    return CALC_OK;
}

int SetNumVal(Node* node, long double newNumVal)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->val.numVal = newNumVal;
    return CALC_OK;
}

int SetVarIdentifierToArrayOfVars(MathExpression* mathExpression, char varIdentifier)
{
    int index = GetVariableIndex(varIdentifier);
    if (index == -1)
    {
        return 1;
    }
    mathExpression->variables[index].identifier = varIdentifier;
    return 0;
}

int SetVarIdentifierToNode(Node* node, char newVarIdentifier)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->val.varIdentifier = newVarIdentifier;
    return CALC_OK;
}

int SetOperation(Node* node, OperationCode newOperation)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->val.operationCode = newOperation;
    return CALC_OK;
}

int SetTypeNode(Node* node, NodeType newNodeType)
{
    if (node == NULL)
    {
        PRINT_LOG_FILE_CALC("ERROR: Node == NULL.");
        return CALC_ERROR_NULL_NODE_POINTER;
    }

    node->type = newNodeType;
    return CALC_OK;
}
