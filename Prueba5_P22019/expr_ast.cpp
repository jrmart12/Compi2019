#include "expr_ast.h"

int AddExpr::eval(std::unordered_map<std::string, int> vars)
{
    int numero1;
    int numero2;
    numero1=expr1->eval(vars);
    numero2=expr2->eval(vars);
    return numero1+numero2;
}

int SubExpr::eval(std::unordered_map<std::string, int> vars)
{
    int numero1;
    int numero2; 
    numero1=expr1->eval(vars);
    numero2=expr2->eval(vars);
    return numero1-numero2;
}

int MulExpr::eval(std::unordered_map<std::string, int> vars)
{
    int numero1;
    int numero2;
    numero1=expr1->eval(vars);
    numero2=expr2->eval(vars);
    return numero1*numero2;
}

int DivExpr::eval(std::unordered_map<std::string, int> vars)
{
    int numero1;
    int numero2;

    numero1=expr1->eval(vars);
    numero2=expr2->eval(vars);
    return numero1/numero2;
}

int ModExpr::eval(std::unordered_map<std::string, int> vars)
{
    int numero1;
    int numero2;
    numero1=expr1->eval(vars);
    numero2=expr2->eval(vars);
    return numero1%numero2;
}
