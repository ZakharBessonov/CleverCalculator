#include <stdio.h>

#include "calc_structs.h"
#include "calc_set_get.h"
#include "calc_comparators.h"

int ComparatorOfVars(const void* varPt1, const void* varPt2)
{
    Variable var1 = *(const Variable*)varPt1;
    Variable var2 = *(const Variable*)varPt2;

    return var1.identifier - var2.identifier;
}

int ComparatorOfOperationsByHash(const void* opPt1, const void* opPt2)
{
    Operation op1 = *(const Operation*)opPt1;
    Operation op2 = *(const Operation*)opPt2;

    if (op1.hashOfOperation > op2.hashOfOperation)
    {
        return 1;
    }
    else if (op1.hashOfOperation == op2.hashOfOperation)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int ComparatorOfOperationsByCodes(const void* opPt1, const void* opPt2)
{
    Operation op1 = *(const Operation*)opPt1;
    Operation op2 = *(const Operation*)opPt2;

    return op1.operationCode - op2.operationCode;
}
