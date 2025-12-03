#ifndef CALC_SET_GET_HEADER
#define CALC_SET_GET_HEADER

#include "calc_structs.h"

int GetVariableIndex(char variable);

char GetVariableSpelling(int index);

// Getters

Node* GetRight(Node* node);

Node* GetLeft(Node* node);

Node* GetParent(Node* node);

Node* GetRoot(MathExpression* mathExpression);

char* GetBufferPointer(MathExpression* mathExpression);

size_t* GetLenOfBuffer(MathExpression* mathExpression, size_t* target);

int GetFileCounter(MathExpression* mathExpression);

Variable* GetVariablesPointer(MathExpression* mathExpression);

Variable GetVariable(MathExpression* mathExpression, char variable);

long double GetVariableValue(MathExpression* mathExpression, char variable);

char GetVarIdentifierFromArrayOfVars(MathExpression* mathExpression, char varIdentifier);

int GetVariablesCounter(MathExpression* mathExpression);

Value* GetValue(Node* node, Value* target);

long double* GetNumVal(Node* node, long double* target);

char GetVarIdentifierFromNode(Node* node);

OperationCode GetOperation(Node* node);

NodeType GetTypeNode(Node* node);

// Setters

int SetRight(Node* node, Node* newRight);

int SetLeft(Node* node, Node* newLeft);

int SetParent(Node* node, Node* newParent);

int SetRoot(MathExpression* mathExpression, Node* newRoot);

int SetBufferPointer(MathExpression* mathExpression, char* newPointer);

int SetLenOfBuffer(MathExpression* mathExpression, size_t newLength);

int SetFileCounter(MathExpression* mathExpression, int newFileCounter);

int IncrementFileCounter(MathExpression* mathExpression);

int SetVariableValue(MathExpression* mathExpression, char variable, long double newVarValue);

int SetVariablesCounter(MathExpression* mathExpression, int newVariablesCounter);

int IncrementVariablesCounter(MathExpression* mathExpression);

int SetValue(Node* node, Value newValue);

int SetNumVal(Node* node, long double newNumVal);

int SetVarIdentifierToArrayOfVars(MathExpression* mathExpression, char varIdentifier);

int SetVarIdentifierToNode(Node* node, char newVarIdentifier);

int SetOperation(Node* node, OperationCode newOperation);

int SetTypeNode(Node* node, NodeType newNodeType);

#endif

