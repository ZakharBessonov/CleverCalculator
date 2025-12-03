#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "calc_structs.h"
#include "calc_consts.h"
#include "calc_general_funcs.h"
#include "calc_set_get.h"
#include "calc_dump.h"
#include "binary_search.h"
#include "calc_comparators.h"

FILE* logfileCalc = NULL;
char* ioFileName = NULL;
extern Operation operations[];

//NOTE: Попробовать разбить на папки - done

static void PrintVariables(MathExpression* mathExpression)
{
    fprintf(logfileCalc, "Variables:\n");
    Variable* varPointer = GetVariablesPointer(mathExpression);
    long double value = 0.0;

    for (int i = 0; i < MAX_NUM_OF_VARIABLES; i++)
    {
        if (varPointer[i].identifier == GetVariableSpelling(i))
        {
            value = GetVariableValue(mathExpression, GetVariableSpelling(i));
            fprintf(logfileCalc, "      <green>%c  =  %Lg</green>\n", GetVariableSpelling(i), value);
        }
    }
}

static void WriteDescriptionOfOpNode(MathExpression* mathExpression, FILE* graph, int number, Node* node)
{
    int codeOfOperation = GetOperation(node);
    fprintf(graph, "node%d [shape=Mrecord, style=filled, fillcolor=\"%s\", fontname=\"PT Mono\","
                   " label = \"{ Parent: %p | %p | val: %s | { <%s> %p | <%s> %p } }\";]\n",
                   number, COLOR_FOR_OPERATIONS, GetParent(node), node,
                   operations[codeOfOperation].spellingOfOperation, "Left", GetLeft(node),
                   "Right", GetRight(node));
}

static void WriteDescriptionOfVarNode(MathExpression* mathExpression, FILE* graph, int number, Node* node)
{
    char identifier = GetVarIdentifierFromNode(node);
    Variable* varPointer = GetVariablesPointer(mathExpression);

    if (varPointer[GetVariableIndex(identifier)].identifier != identifier)
    {
        fprintf(logfileCalc, "ERROR: Variable '%c' is not found.\n", identifier);
        identifier = '?';
    }

    long double valueOfVar = GetVariableValue(mathExpression, identifier);

    fprintf(graph, "node%d [shape=record, style=filled, fillcolor=\"%s\", fontname=\"PT Mono\","
                   " label = \"{ Parent: %p | %p | %c = %Lg | {%p | %p}}\";]\n",
                   number, COLOR_FOR_VARS, GetParent(node), node,
                   identifier, valueOfVar, GetLeft(node), GetRight(node));
}

static void WriteDescriptionOfNumNode(MathExpression* mathExpression, FILE* graph, int number, Node* node)
{
    long double value = 0.0;
    GetNumVal(node, &value);

    fprintf(graph, "node%d [shape=record, style=filled, fillcolor=\"%s\", fontname=\"PT Mono\","
                   " label = \"{ Parent: %p | %p | %Lg | {%p | %p}}\";]\n",
                   number, COLOR_FOR_NUMS, GetParent(node), node, value, GetLeft(node), GetRight(node));
}

void CalcDump(MathExpression* mathExpression, const char* fileName, const char* func, int line, const char* message, ...)
{
    va_list args = NULL;
    va_start(args, message);

    fprintf(logfileCalc, "<pre>\n"
                         "<head>\n<link rel=\"stylesheet\" href=\"styles.css\">\n</head>"
                         "<h3> DUMP <red> from %s:</red> <blue>", func);
    vfprintf(logfileCalc, message, args);
    fprintf(logfileCalc, "</blue> </h3>"
                         "MathExpression { %s:%d } from <b>\"%s\"</b>:", fileName, line, ioFileName);
    va_end(args);

    int errors = CalcVerify(mathExpression);
    if (errors & CALC_ERROR_NULL_MATH_EXPRESSION_POINTER)
    {
        fprintf(logfileCalc, " (CALC_ERROR_NULL_MATH_EXPRESSION_POINTER)\n"
                             "</pre>\n\n");
        return;
    }
    else
    {
        fprintf(logfileCalc, "\n");
    }

    fprintf(logfileCalc, "Root:             <green> %p </green>\n", GetRoot(mathExpression));

    PrintVariables(mathExpression);

    fprintf(logfileCalc, "Image: ");

    CalcCreateGraph(mathExpression);
    fprintf(logfileCalc, "\n<img src=graphs_svg/graph%d.svg weight=700px>\n"
                       "\n\n\n\n\n\n</pre>\n", GetFileCounter(mathExpression));
    fflush(logfileCalc);
    return;
}

void CalcCreateGraph(MathExpression* mathExpression)
{
    IncrementFileCounter(mathExpression);
    int fileCounter = GetFileCounter(mathExpression);

    char graphFileName[GRAPH_FILE_NAME_MAX_LEN] = "";
    snprintf(graphFileName, GRAPH_FILE_NAME_MAX_LEN - 1, "graphs_txt/graph%d.txt", fileCounter);

    FILE* graph = fopen(graphFileName, "w");

    if (graph == NULL)
    {
        fprintf(logfileCalc, "ERROR: An error was occurred while opening \"%s\".\n", graphFileName);
        return;
    }

    CalcPrintTitleOfGraphFile(graph);
    Node* root = GetRoot(mathExpression);
    if (root != NULL)
    {
        int number = 0;
        CalcWriteDescriptionOfNode(mathExpression, root, graph, &number);
        number = 0;
        CalcWriteDescriptionOfEdge(root, graph, &number);
    }

    fprintf(graph, "}\n");
    fclose(graph);

    char fmt[MAX_SIZE_OF_STRING] = "";
    snprintf(fmt, MAX_SIZE_OF_STRING - 1, "dot graphs_txt/graph%d.txt -Tsvg -o graphs_svg/graph%d.svg",
                                               fileCounter, fileCounter);
    system(fmt);
}

void CalcPrintTitleOfGraphFile(FILE* graph)
{
    fprintf(graph, "digraph {\n"
                   "nodesep = 0.5;\n"
                   "rankdir=\"TB\";\n");
}

void CalcWriteDescriptionOfNode(MathExpression* mathExpression, Node* node, FILE* graph, int* number)
{
    int isLeftNull = GetLeft(node) == NULL;
    int isRightNull = GetRight(node) == NULL;
    NodeType type = GetTypeNode(node);

    void (*funcsOfWritingDescription[])(MathExpression*, FILE*, int, Node*) =
    {
        WriteDescriptionOfOpNode,
        WriteDescriptionOfVarNode,
        WriteDescriptionOfNumNode
    };

    funcsOfWritingDescription[type](mathExpression, graph, *number, node);
    (*number)++;

    if (!isLeftNull)
    {
        CalcWriteDescriptionOfNode(mathExpression, GetLeft(node), graph, number);
    }

    if (!isRightNull)
    {
        CalcWriteDescriptionOfNode(mathExpression, GetRight(node), graph, number);
    }
}

void CalcWriteDescriptionOfEdge(Node* node, FILE* graph, int* number)
{
    int startNumber = *number;

    if (GetLeft(node) != NULL)
    {
        fprintf(graph, "node%d: <Left> -> node%d;\n", *number, *number + 1);
        (*number)++;
        CalcWriteDescriptionOfEdge(GetLeft(node), graph, number);
    }

    if (GetRight(node) != NULL)
    {
        fprintf(graph, "node%d: <Right> -> node%d;\n", startNumber, *number + 1);
        (*number)++;
        CalcWriteDescriptionOfEdge(GetRight(node), graph, number);
    }
}

int OpenLogFileCalc()
{
    logfileCalc = fopen("logfileCalc.html", "w");
    if (logfileCalc == NULL)
    {
        return 1;
    }

    return 0;
}

void CloseLogFileCalc()
{
    fclose(logfileCalc);
}

