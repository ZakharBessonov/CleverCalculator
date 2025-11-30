#ifndef CALC_READ_WRITE_TO_FILE_HEADER
#define CALC_READ_WRITE_TO_FILE_HEADER

// Reading

FILE* CalcOpenFile(int argc, char *argv[]);

void CalcReadFile(MathExpression* mathExpression, FILE* ioStream);

void CalcCreateTree(MathExpression* mathExpression);

// Writing

void CalcWriteExpressionToTeXFile(MathExpression* mathExpression, const char* message, const char* title);

void CalcWriteTreeToFile(MathExpression* mathExpression);

void CalcWriteNodeToFile(Node* node, FILE* ioFile);

void CalcFinishTeXFile();

#endif
