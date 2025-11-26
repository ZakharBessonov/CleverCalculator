#ifndef CALC_DUMP_HEADER
#define CALC_DUMP_HEADER

void CalcDump(MathExpression* mathExpression, const char* fileName, const char* func, int line, const char* message, ...);

void CalcCreateGraph(MathExpression* mathExpression);

void CalcPrintTitleOfGraphFile(FILE* graph);

void CalcWriteDescriptionOfNode(MathExpression* mathExpression, Node* node, FILE* graph, int* number);

void CalcWriteDescriptionOfEdge(Node* node, FILE* graph, int* number);

int OpenLogFileCalc();

void CloseLogFileCalc();

#endif
