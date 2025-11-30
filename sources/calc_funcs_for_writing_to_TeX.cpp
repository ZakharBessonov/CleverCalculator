#include <stdio.h>

#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_funcs_for_writing_to_TeX.h"
#include "calc_set_get.h"

extern FILE* logfileCalc;
extern char* ioFileName;
extern Operation operations[];
extern size_t numOfOperations;

static void ProcessNumberOrLabel(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_VAR)
    {
        fprintf(texFile, "%c", GetVarIdentifierFromNode(node));
    }
    else if (type == TYPE_NUMBER)
    {
        long double number = 0.0;
        GetNumVal(node, &number);
        if (number < 0)
        {
            fprintf(texFile, "(%Lg)", number);
        }
        else
        {
            fprintf(texFile, "%Lg", number);
        }
    }
}

void WriteAddInTeX(FILE* texFile, Node* node)
{
    if (GetParent(node) != NULL && GetOperation(GetParent(node)) == OP_MUL)
    {
        fprintf(texFile, "\\left(");
    }

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        operations[operationCodeLeft].funcToWritingInTeXFile(texFile, GetLeft(node));
        fprintf(texFile, "+");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(texFile, GetRight(node));
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }

    if (GetParent(node) != NULL && GetOperation(GetParent(node)) == OP_MUL)
    {
        fprintf(texFile, "\\right)");
    }

}

void WriteSubInTeX(FILE* texFile, Node* node)
{
    if (GetParent(node) != NULL && GetOperation(GetParent(node)) == OP_MUL)
    {
        fprintf(texFile, "\\left(");
    }

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        operations[operationCodeLeft].funcToWritingInTeXFile(texFile, GetLeft(node));
        fprintf(texFile, "-");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(texFile, GetRight(node));
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }

    if (GetParent(node) != NULL && GetOperation(GetParent(node)) == OP_MUL)
    {
        fprintf(texFile, "\\right)");
    }
}

void WriteMulInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        operations[operationCodeLeft].funcToWritingInTeXFile(texFile, GetLeft(node));
        fprintf(texFile, " \\cdot ");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(texFile, GetRight(node));
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteDivInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        fprintf(texFile, "\\frac{");
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        operations[operationCodeLeft].funcToWritingInTeXFile(texFile, GetLeft(node));
        fprintf(texFile, "}{");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "}");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WritePowInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        fprintf(texFile, (typeLeft == TYPE_OP) ? "\\left(" : "");
        operations[operationCodeLeft].funcToWritingInTeXFile(texFile, GetLeft(node));
        fprintf(texFile, (typeLeft == TYPE_OP) ? "\\right)^{" : "^{");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "}");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteLogInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        fprintf(texFile,"\\log_{");
        operations[operationCodeLeft].funcToWritingInTeXFile(texFile, GetLeft(node));
        fprintf(texFile, "}\\left(");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteLnInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\ln\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteSinInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\sin\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteCosInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\cos\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteTgInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\tg\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteCtgInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\ctg\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteShInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\sh\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteChInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\ch\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteThInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\th\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteCthInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\cth\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteArcsinInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arcsin\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteArccosInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arccos\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteArctgInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arctg\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}

void WriteArcctgInTeX(FILE* texFile, Node* node)
{
    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arcctg\\left(");
        operations[operationCode].funcToWritingInTeXFile(texFile, GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(texFile, node);
    }
}
