#include "expressionbuilder.h"
#include "Nodes.h"
#include <sstream>

using namespace std;

int BinaryExpressionBuilder::precedence(char op)
{
    enum {low, mid, hi};
    switch (op)
    {
        case '+':
        case '-':
            return mid;

        case '/':
        case '*':
        case '^':
            return hi;

        default:
            return low;
    }
}

ExpressionElementNode *BinaryExpressionBuilder::parse(std::string& str) throw(NotWellFormed)
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
                processOperator(token);
                break;

            case ')':
                processRightParenthesis();
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
        doBinary(operatorStack.top());
        operatorStack.pop();
    }

    if (operandStack.size()!=1)
    {
        throw NotWellFormed();
    }

    ExpressionElementNode *p = operandStack.top();
    return p;
}

void BinaryExpressionBuilder::processOperator(char op)
{
    int opPrecedence = precedence(op);
    while ((!operatorStack.empty()) && (opPrecedence <= precedence(operatorStack.top())))
    {
        doBinary( operatorStack.top());
        operatorStack.pop();
    }

    operatorStack.push(op);
}

void BinaryExpressionBuilder::processRightParenthesis()
{
    while (!operatorStack.empty() && operatorStack.top()!= '(')
    {
        doBinary(operatorStack.top());
        operatorStack.pop();
    }

    operatorStack.pop();
}

void BinaryExpressionBuilder::doBinary(char op)
{
    ExpressionElementNode *right = operandStack.top();
    operandStack.pop();

    ExpressionElementNode *left = operandStack.top();
    operandStack.pop();

    ExpressionElementNode *p= new ExpressionElementNode(operatorStack.top(), left, right);
    operandStack.push(p);
}

double BinaryExpressionBuilder::calculate(ExpressionElementNode* root, double variableValue)
{
    double left, right, result;
    if(root->left != 0 && root->right != 0)
    {
        left = calculate(root->left, variableValue);
        right = calculate(root->right, variableValue);
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
        left = root->number;
        right = calculate(root->right, variableValue);
    }

    else if(root->right == 0)
    {
        left = calculate(root->left, variableValue);
        right = root->number;
    }

    switch ( root->binary_op )
    {
        case '+':  result = left + right;
        break;

        case '-':  result = left - right;
        break;

        case '*':  result = left * right;
        break;

        case '/':  result = left / right;
        break;

        case '^':  result = pow(left, right);
        break;
    }

    return result;
}
