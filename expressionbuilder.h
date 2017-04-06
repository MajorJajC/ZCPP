#include <string>
#include <stack>
#include <exception>

class ExpressionElementNode;

class BinaryExpressionBuilder
{
   private:
     // holds either (, +, -, /, or *
     std::stack<char> operatorStack;

     // operandStack is made up of BinaryOperationNodes and NumericElementNode
     std::stack<ExpressionElementNode *> operandStack;

     void przetworzDzialanie(char op);
     void przetworzPrawyNawias();

     void tworzDrzewo(char op);

     int kolejnoscDzialan(char op);

   public:
     class NotWellFormed : public std::exception {

        public:
          virtual const char* what() const throw()
          {
                return "The expression is not valid";
          }
      };

     ExpressionElementNode *parsujDzialanie(std::string& istr) throw(NotWellFormed);
     double oblicz(ExpressionElementNode* root, double variableValue);
};
