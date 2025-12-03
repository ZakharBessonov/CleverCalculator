#include <stdio.h>

#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_funcs_for_writing_to_TeX.h"
#include "calc_set_get.h"

extern FILE* logfileCalc;
extern char* ioFileName;
extern Operation operations[];
extern size_t numOfOperations;
extern FILE* texFile;

static void ProcessNumberOrLabel(Node* node)
{
    if (node == NULL) return;

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

void WriteAddInTeX(Node* node)
{
    if (node == NULL) return;

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
        operations[operationCodeLeft].funcToWritingInTeXFile(GetLeft(node));
        fprintf(texFile, "+");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(GetRight(node));
    }
    else
    {
        ProcessNumberOrLabel(node);
    }

    if (GetParent(node) != NULL && GetOperation(GetParent(node)) == OP_MUL)
    {
        fprintf(texFile, "\\right)");
    }

}

void WriteSubInTeX(Node* node)
{
    if (node == NULL) return;

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
        operations[operationCodeLeft].funcToWritingInTeXFile(GetLeft(node));
        fprintf(texFile, "-");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(GetRight(node));
    }
    else
    {
        ProcessNumberOrLabel(node);
    }

    if (GetParent(node) != NULL && GetOperation(GetParent(node)) == OP_MUL)
    {
        fprintf(texFile, "\\right)");
    }
}

void WriteMulInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        operations[operationCodeLeft].funcToWritingInTeXFile(GetLeft(node));
        fprintf(texFile, " \\cdot ");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(GetRight(node));
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteDivInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        fprintf(texFile, "\\frac{");
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        operations[operationCodeLeft].funcToWritingInTeXFile(GetLeft(node));
        fprintf(texFile, "}{");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "}");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WritePowInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        fprintf(texFile, (typeLeft == TYPE_OP) ? "\\left(" : "");
        operations[operationCodeLeft].funcToWritingInTeXFile(GetLeft(node));
        fprintf(texFile, (typeLeft == TYPE_OP) ? "\\right)^{" : "^{");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "}");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteLogInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeLeft = GetTypeNode(GetLeft(node));
        NodeType typeRight = GetTypeNode(GetRight(node));
        OperationCode operationCodeLeft = (typeLeft == TYPE_OP) ? GetOperation(GetLeft(node)) : DEFAULT_OP;
        fprintf(texFile,"\\log_{");
        operations[operationCodeLeft].funcToWritingInTeXFile(GetLeft(node));
        fprintf(texFile, "}\\left(");
        OperationCode operationCodeRight = (typeRight == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        operations[operationCodeRight].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteLnInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\ln\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteSinInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\sin\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteCosInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\cos\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteTgInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\tg\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteCtgInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\ctg\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteShInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\sh\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteChInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\ch\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteThInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\th\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteCthInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\cth\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteArcsinInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arcsin\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteArccosInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arccos\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteArctgInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arctg\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}

void WriteArcctgInTeX(Node* node)
{
    if (node == NULL) return;

    NodeType type = GetTypeNode(node);
    if (type == TYPE_OP)
    {
        NodeType typeOfSon = GetTypeNode(GetRight(node));
        OperationCode operationCode = (typeOfSon == TYPE_OP) ? GetOperation(GetRight(node)) : DEFAULT_OP;
        fprintf(texFile,"\\arcctg\\left(");
        operations[operationCode].funcToWritingInTeXFile(GetRight(node));
        fprintf(texFile, "\\right)");
    }
    else
    {
        ProcessNumberOrLabel(node);
    }
}
