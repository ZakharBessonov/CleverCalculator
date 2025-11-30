#ifndef CALC_FUNCS_FOR_WRITING_TO_TEX_HEADER
#define CALC_FUNCS_FOR_WRITING_TO_TEX_HEADER

void WriteAddInTeX(FILE* texFile, Node* node);

void WriteSubInTeX(FILE* texFile, Node* node);

void WriteMulInTeX(FILE* texFile, Node* node);

void WriteDivInTeX(FILE* texFile, Node* node);

void WritePowInTeX(FILE* texFile, Node* node);

void WriteLogInTeX(FILE* texFile, Node* node);

void WriteLnInTeX(FILE* texFile, Node* node);

void WriteSinInTeX(FILE* texFile, Node* node);

void WriteCosInTeX(FILE* texFile, Node* node);

void WriteTgInTeX(FILE* texFile, Node* node);

void WriteCtgInTeX(FILE* texFile, Node* node);

void WriteShInTeX(FILE* texFile, Node* node);

void WriteChInTeX(FILE* texFile, Node* node);

void WriteThInTeX(FILE* texFile, Node* node);

void WriteCthInTeX(FILE* texFile, Node* node);

void WriteArcsinInTeX(FILE* texFile, Node* node);

void WriteArccosInTeX(FILE* texFile, Node* node);

void WriteArctgInTeX(FILE* texFile, Node* node);

void WriteArcctgInTeX(FILE* texFile, Node* node);


#endif
