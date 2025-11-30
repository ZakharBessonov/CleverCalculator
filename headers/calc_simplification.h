#ifndef CALC_SIMPLIFICATION_HEADER
#define CALC_SIMPLIFICATION_HEADER

void CalcSimplifyExpression(MathExpression* mathExpression);

Node* CalcConstantFolding(Node* node, int* breaker);

Node* CalcRemovingNeutralElements(MathExpression* mathExpression, Node* node);

#endif
