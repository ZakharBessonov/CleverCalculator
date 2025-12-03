#ifndef CALC_FUNCS_FOR_OPTIONS_HEADER
#define CALC_FUNCS_FOR_OPTIONS_HEADER

int CalcCountExpression(MathExpression* mathExpression);

int CalcCountDerivative(MathExpression* mathExpression);

Node* CalcDifferentiate(Node* node, char varDifferentiation);

int CalcCountTaylorSeries(MathExpression* mathExpression);

int CalcPlotGraph(MathExpression* mathExpression);

int CalcPlotTangentToGraph(MathExpression* mathExpression);

int CalcCountExpressionTakingIntoAccountTheError(MathExpression* mathExpression);

void WriteDerivative(Node* node, Node* dNode, char varDifferentiation);

const char* const  phrases[] =
{
    "Это всё напридумывали учёные...",
    "Все знают тысячелетнюю историю землян - как они варят стекло...",
    " - Так кремний - не металл!\\\\"
    " - Нееееет, кремний - чистокровный металл. Я материаловед :)!",
    "Самым современным абаком были счёты древних русов."
    " А нам говорят, что мы научились считать у арабов, в Индии... Только у русов были счёты!",
    "Здесь показано, как детей, со школьной скамьи, переводят в состояние зомби по математике.",
    "Все задумались - а что же такое ноль? Так вот с нулём нас опять обманули.",
    "Если вы как математик, то вы должны знать двоичный счёт древних русов - ноль, целковый, "
    "полушка, четвертушка, осьмушка, пудовичок, медичок, серебрячок, золотничок, осьмичок, девятичок, десятичок.",
    "Так считали наши предки...",
    "Ни один закон Ньютона в природе не работает!",
    "А нам говорят - гравитация, а никакой гравитации в природе нет. Всего-то навсего, проверяется элементарно.",
    "Все говорят, что это заговор масонов. А вот кто они - это другой разговор.",
    "А таблицу умножения никто и не видел!",
    "Двойку просуммировать 10 раз - будет 20, а умножить - 1024, то есть килобайт. И это вот всё двоичный счёт древних русов.",
    " - Вот я уже подаю в суд на академию наук...\\\\"
    " - Из-за таблицы умножения?\n",
    "А скорость света равна нулю. Это элементарно проверяется - спичкой щас вам показываю...",
    "Два года уже каждый знает, что свет никуда не бежит!"
};

const int numOfPhrases = sizeof(phrases) / sizeof(*phrases);

#endif
