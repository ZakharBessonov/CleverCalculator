#ifndef CALC_GENERAL_FUNCS_HEADER
#define CALC_GENERAL_FUNCS_HEADER

void CalcCtor(MathExpression* mathExpression);

int CalcVerify(MathExpression* mathExpression);

// void CalcStart(MathExpression* mathExpression);

void CalcDtor(MathExpression* mathExpression);

void NodeFree(Node* node);

#endif
