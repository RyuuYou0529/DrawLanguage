//
// Created by RyuuYou on 2020/12/3.
//

#ifndef GRAPH_TREENODE_HPP
#define GRAPH_TREENODE_HPP

#include "../lexer/scanner.hpp"
#include <iostream>

using namespace std;

typedef double (*FuncPtr)(double);

//===========================================
class SourceNode {
public:
    Token_Type Type;
    string Info;

    explicit SourceNode(Token_Type theType, string theInfo) : Type(theType), Info(move(theInfo)) {}

    virtual void PrintSyntaxTree(int indent) const;

    virtual ~SourceNode() = default;;
};

//===========================================
//===========================================
//======表达式节点：ExprNode
class ExprNode : public SourceNode {
public:
    explicit ExprNode(Token_Type theType, string theInfo) : SourceNode(theType, move(theInfo)) {}

    virtual double Calculate() const = 0;
};

//===========================================
//======运算符节点：OperatorNode，继承于ExprNode
class OperatorNode : public ExprNode {
public:
    ExprNode *Left, *Right;

    OperatorNode(Token_Type theType, ExprNode *left,
                 ExprNode *right, string theInfo) : ExprNode(theType,move(theInfo)) {
        this->Left = left;
        this->Right = right;
    }

    void PrintSyntaxTree(int indent) const override;

    ~OperatorNode() override {
        delete Left;
        delete Right;
    }
};

//===加法操作节点：PlusNode，继承于OperatorNode
class PlusNode : public OperatorNode {
public:
    PlusNode(ExprNode *left, ExprNode *right) : OperatorNode(PLUS, left, right, "+") {}

    double Calculate() const override {
        return Left->Calculate() + Right->Calculate();
    }
};

//===减法操作节点：MinusNode，继承于OperatorNode
class MinusNode : public OperatorNode {
public:
    MinusNode(ExprNode *left, ExprNode *right) : OperatorNode(MINUS, left, right, "-") {}

    double Calculate() const override {
        return Left->Calculate() - Right->Calculate();
    }
};

//===乘方操作节点：PowerNode，继承于OperatorNode
class PowerNode : public OperatorNode {
public:
    PowerNode(ExprNode *left, ExprNode *right) : OperatorNode(POWER, left, right, "**") {}

    double Calculate() const override {
        return pow(Left->Calculate(), Right->Calculate());
    }
};

//===乘法操作节点：MulNode，继承于OperatorNode
class MulNode : public OperatorNode {
public:
    MulNode(ExprNode *left, ExprNode *right) : OperatorNode(MUL, left, right, "*") {}

    double Calculate() const override {
        return Left->Calculate() * Right->Calculate();
    }
};

//===除法操作节点：DivNode，继承于OperatorNode
class DivNode : public OperatorNode {
public:
    DivNode(ExprNode *left, ExprNode *right) : OperatorNode(DIV, left, right, "/") {}

    double Calculate() const override {
        return Left->Calculate() / Right->Calculate();
    }
};

//===========================================
//======函数节点：FuncNode，继承于ExprNode
class FuncNode : public ExprNode {
public:
    ExprNode *Child;
    FuncPtr MathFuncPtr;

    FuncNode(ExprNode *child, FuncPtr funcPtr, string FuncName) : ExprNode(FUNC, move(FuncName)) {
        this->Child = child;
        this->MathFuncPtr = funcPtr;
    }

    void PrintSyntaxTree(int indent) const override;

    double Calculate() const override {
        return MathFuncPtr(Child->Calculate());
    }

    ~FuncNode() override {
        delete Child;
    }
};

//===========================================
//======常数节点：ConstNode，继承于ExprNode
class ConstNode : public ExprNode {
public:
    double value;

    explicit ConstNode(double theValue) : ExprNode(CONST_ID, "") {
        this->value = theValue;
        this->Info = to_string(theValue);
    }

    double Calculate() const override {
        return this->value;
    }
};

//===========================================
//======参数节点：ParmNode，继承于ExprNode
class ParmNode : public ExprNode {
public:
    double ParmValue{};

    explicit ParmNode() : ExprNode(T, "T") {}

    double Calculate() const override { return ParmValue; }
};

//===========================================
//===========================================
//======Staement节点，继承于SourceNode
class StatementNode : public SourceNode {
public:
    explicit StatementNode(Token_Type theType = STATEMENT, string theInfo = "Statement") : SourceNode(theType,
                                                                                                      move(theInfo)) {}

    virtual void Calculate(double *Result) const = 0;
};

//===========================================
//===ORIGIN节点，继承于StatementNode
class OriginNode : public StatementNode {
public:
    ExprNode *X, *Y;

    OriginNode(ExprNode *theX, ExprNode *theY) : StatementNode(ORIGIN, "Origin") {
        this->X = theX;
        this->Y = theY;
    }

    ~OriginNode() override {
        delete X;
        delete Y;
    }

    void PrintSyntaxTree(int indent) const override;

    void Calculate(double *Result) const override;
};

//===========================================
//===Scale节点，继承于StatementNode
class ScaleNode : public StatementNode {
public:
    ExprNode *X, *Y;

    ScaleNode(ExprNode *theX, ExprNode *theY) : StatementNode(SCALE, "Scale") {
        this->X = theX;
        this->Y = theY;
    }

    ~ScaleNode() override {
        delete X;
        delete Y;
    }

    void PrintSyntaxTree(int indent) const override;

    void Calculate(double *Result) const override;
};

//===========================================
//===Rot节点，继承于StatementNode
class RotNode : public StatementNode {
public:
    ExprNode *Angle;

    explicit RotNode(ExprNode *theAngle) : StatementNode(ROT, "Rot") {
        this->Angle = theAngle;
    }

    ~RotNode() override { delete Angle; }

    void PrintSyntaxTree(int indent) const override;

    void Calculate(double *Result) const override;
};

//===========================================
//===For节点，继承于StatementNode
class ForNode : public StatementNode {
public:
    ExprNode *Start, *End, *Step, *X, *Y;
    ParmNode *ParmT;

    ForNode(ParmNode *theParm, ExprNode *theStart, ExprNode *theEnd, ExprNode *theStep,
            ExprNode *theX, ExprNode *theY) : StatementNode(FOR, "For") {
        this->ParmT = theParm;
        this->Start = theStart;
        this->End = theEnd;
        this->Step = theStep;
        this->X = theX;
        this->Y = theY;
    }

    ~ForNode() override {
        delete Start;
        delete End;
        delete Step;
        delete X;
        delete Y;
    }

    void PrintSyntaxTree(int indent) const override;

    void Calculate(double *Result) const override;
};
//===========================================
//===Color节点，继承于StatementNode
class ColorNode: public StatementNode{
public:
    ExprNode *Red,*Green,*Blue;

    ColorNode(ExprNode *R,ExprNode *G,ExprNode *B):StatementNode(COLOR,"Color"){
        this->Red=R;
        this->Green=G;
        this->Blue=B;
    }

    ~ColorNode() override{
        delete Red;
        delete Green;
        delete Blue;
    }

    void PrintSyntaxTree(int indent) const override;

    void Calculate(double *Result) const override;
};
class WidthNode: public StatementNode{
public:
    ExprNode *Width;

    explicit WidthNode(ExprNode *theWidth):StatementNode(WIDTH,"Width"){
        this->Width=theWidth;
    }

    ~WidthNode() override{delete Width;}

    void PrintSyntaxTree(int indent) const override;

    void Calculate(double *Result) const override;
};
//===========================================
//===========================================
//======Program节点，继承于SourceNode
class ProgramNode : public SourceNode {
public:
    StatementNode *Statement;
    ProgramNode *Program;

    ProgramNode(StatementNode *stat, ProgramNode *prog) : SourceNode(PROGRAM, "Program") {
        this->Statement = stat;
        this->Program = prog;
    }

    ~ProgramNode() override {
        delete Statement;
        delete Program;
    }

    void PrintSyntaxTree(int indent) const override;
};
//===========================================
#endif //GRAPH_TREENODE_HPP
