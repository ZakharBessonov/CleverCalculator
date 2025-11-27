#ifndef CALC_FUNCS_FOR_OPTIONS_HEADER
#define CALC_FUNCS_FOR_OPTIONS_HEADER

int CalcCountExpression(MathExpression* mathExpression);

int CalcCountDerivative(MathExpression* mathExpression);

int CalcCountTaylorSeries(MathExpression* mathExpression);

int CalcPlotGraph(MathExpression* mathExpression);

int CalcPlotTangentToGraph(MathExpression* mathExpression);

int CalcCountExpressionTakingIntoAccountTheError(MathExpression* mathExpression);

#endif
