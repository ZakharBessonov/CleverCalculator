#ifndef CALC_SIMPLIFICATION_HEADER
#define CALC_SIMPLIFICATION_HEADER

void CalcSimplifyExpression(MathExpression* mathExpression);

void CalcConstantFolding(Node* node, int* breaker);

void CalcRemovingNeutralElements(Node* node, int* breaker);

void RemoveNeutElemAdd(Node* node, int* breaker);

void RemoveNeutElemSub(Node* node, int* breaker);

void RemoveNeutElemMul(Node* node, int* breaker);

void RemoveNeutElemDiv(Node* node, int* breaker);

void RemoveNeutElemPow(Node* node, int* breaker);

void RemoveNeutElemLog(Node* node, int* breaker);

#endif
