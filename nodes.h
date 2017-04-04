class ExpressionElementNode
{
   public:
       ExpressionElementNode(double val);
       ExpressionElementNode(char var);
       ExpressionElementNode(char op, ExpressionElementNode *l, ExpressionElementNode *r);
       ExpressionElementNode *left = 0;
       ExpressionElementNode *right = 0;
       char binary_op = 0;
       double number;
       char variable;
};

inline ExpressionElementNode::ExpressionElementNode(double val): number(val), variable(NULL), binary_op(0), left(0), right(0) {}
inline ExpressionElementNode::ExpressionElementNode(char var): number(0.0), variable(var), binary_op(0), left(0), right(0) {}

inline ExpressionElementNode::ExpressionElementNode(char op, ExpressionElementNode *l, ExpressionElementNode *r):
    binary_op(op), left(l), right(r) {}
