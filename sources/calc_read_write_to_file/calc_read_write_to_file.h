#ifndef CALC_READ_WRITE_TO_FILE_HEADER
#define CALC_READ_WRITE_TO_FILE_HEADER

// Reading

FILE* CalcOpenFile(int argc, char *argv[]);

void CalcReadFile(MathExpression* mathExpression, FILE* ioStream);

void CalcCreateTree(MathExpression* mathExpression);

// Writing

void CalcOpenTexFileAndWritePreamble();

void CalcWriteExpressionToTeXFile(MathExpression* mathExpression, const char* message);

void CalcWriteTitleToTexFile(const char* title, ...);

void CalcFinishTeXFile();

#endif
