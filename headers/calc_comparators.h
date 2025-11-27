#ifndef CALC_COMPARATORS_HEADER
#define CALC_COMPARATORS_HEADER

int ComparatorOfVars(const void* varPt1, const void* varPt2);

int ComparatorOfOperationsByHash(const void* opPt1, const void* opPt2);

int ComparatorOfOperationsByCodes(const void* opPt1, const void* opPt2);

int IsEqual(long double num1, long double num2);

#endif
