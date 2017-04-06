#include "expressionbuilder.h"
#include "Nodes.h"
#include <sstream>

using namespace std;

int BinaryExpressionBuilder::kolejnoscDzialan(char op)
{
    enum {zero, low, mid, hi};
    switch (op)
    {
        default:
            return zero;

        case '+':
        case '-':
            return low;

        case '/':
        case '*':
            return mid;

        case '^':
            return hi;
    }
}

void BinaryExpressionBuilder::przetworzDzialanie(char op)
{
    int opKolejnosc = kolejnoscDzialan(op);
    while ((!operatorStack.empty()) && (opKolejnosc <= kolejnoscDzialan(operatorStack.top())))
    {
        tworzDrzewo(operatorStack.top());
        operatorStack.pop();
    }

    operatorStack.push(op);
}

void BinaryExpressionBuilder::tworzDrzewo(char op)
{
    ExpressionElementNode *prawy = operandStack.top();
    operandStack.pop();

    ExpressionElementNode *lewy = operandStack.top();
    operandStack.pop();

    ExpressionElementNode *p= new ExpressionElementNode(operatorStack.top(), lewy, prawy);
    operandStack.push(p);
}

ExpressionElementNode *BinaryExpressionBuilder::parsujDzialanie(std::string& str) throw(NotWellFormed)
{
    istringstream istr(str);
    char token;
    while (istr >> token)
    {
        switch (token)
        {
            case '+':
            case '-':
            case '*':
            case '^':
            case '/':
                przetworzDzialanie(token);
                break;

            case ')':
                przetworzPrawyNawias();
                break;

            case '(':
                operatorStack.push(token);
                break;

            case 'x':
            {
                ExpressionElementNode *variable = new ExpressionElementNode(token);
                operandStack.push(variable);
                break;
            }

            default:
                istr.putback (token);
                double number;

                istr >> number;

                ExpressionElementNode *newNode = new ExpressionElementNode(number);
                operandStack.push(newNode);

                continue;
        }
    }

    while (!operatorStack.empty())
    {
        tworzDrzewo(operatorStack.top());
        operatorStack.pop();
    }

    if (operandStack.size()!=1)
    {
        throw NotWellFormed();
    }

    ExpressionElementNode *p = operandStack.top();
    return p;
}

void BinaryExpressionBuilder::przetworzPrawyNawias()
{
    while (!operatorStack.empty() && operatorStack.top()!= '(')
    {
        tworzDrzewo(operatorStack.top());
        operatorStack.pop();
    }

    operatorStack.pop();
}

double BinaryExpressionBuilder::oblicz(ExpressionElementNode* root, double variableValue)
{
    double lewy, prawy, wynik;
    if(root->left != 0 && root->right != 0)
    {
        lewy = oblicz(root->left, variableValue);
        prawy = oblicz(root->right, variableValue);
    }

    else if(root->left == 0 && root->right == 0)
    {
        if(root->variable)
        {
            return variableValue;
        }

        else
        {
            return root->number;
        }
    }

    else if(root->left == 0)
    {
        lewy = root->number;
        prawy = oblicz(root->right, variableValue);
    }

    else if(root->right == 0)
    {
        lewy = oblicz(root->left, variableValue);
        prawy = root->number;
    }

    switch ( root->binary_op )
    {
        case '+':  wynik = lewy + prawy;
        break;

        case '-':  wynik = lewy - prawy;
        break;

        case '*':  wynik = lewy * prawy;
        break;

        case '/':  wynik = lewy / prawy;
        break;

        case '^':  wynik = pow(lewy, prawy);
        break;
    }

    return wynik;
}
