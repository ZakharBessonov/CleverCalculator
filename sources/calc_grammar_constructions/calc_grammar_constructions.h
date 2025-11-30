#ifndef CALC_GRAMMAR_CONSTRUCTIONS_HEADER
#define CALC_GRAMMAR_CONSTRUCTIONS_HEADER

Node* GetN(char** curPos);

Node* GetV(MathExpression* mathExpression, char** curPos);

Node* GetE(MathExpression* mathExpression, char** curPos);

Node* GetT(MathExpression* mathExpression, char** curPos);

Node* GetS(MathExpression* mathExpression, char** curPos);

Node* GetG(MathExpression* mathExpression, char** curPos);

Node* GetP(MathExpression* mathExpression, char** curPos);

Node* GetF(MathExpression* mathExpression, char** curPos, int indexOfOperation);

#endif
