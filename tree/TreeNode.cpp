//
// Created by RyuuYou on 2020/12/3.
//

#include "TreeNode.hpp"

void SourceNode::PrintSyntaxTree(int indent) const {
    for (int i = 0; i < indent; ++i) cout << "\t";
    cout << Info << endl;
}

void OperatorNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    if (Left != nullptr)
        Left->PrintSyntaxTree(indent + 1);
    if (Right != nullptr)
        Right->PrintSyntaxTree(indent + 1);
}

void FuncNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    if (Child != nullptr)
        Child->PrintSyntaxTree(indent + 1);
}

void ProgramNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    if (Statement != nullptr)
        Statement->PrintSyntaxTree(indent + 1);
    if (Program != nullptr)
        this->Program->PrintSyntaxTree(indent);
}

void OriginNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    X->PrintSyntaxTree(indent + 1);
    Y->PrintSyntaxTree(indent + 1);
}

void ScaleNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    X->PrintSyntaxTree(indent + 1);
    Y->PrintSyntaxTree(indent + 1);
}

void RotNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    Angle->PrintSyntaxTree(indent + 1);
}

void ForNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    ParmT->PrintSyntaxTree(indent+1);
    Start->PrintSyntaxTree(indent + 1);
    End->PrintSyntaxTree(indent + 1);
    Step->PrintSyntaxTree(indent + 1);
    X->PrintSyntaxTree(indent + 1);
    Y->PrintSyntaxTree(indent + 1);
}

void ColorNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    Red->PrintSyntaxTree(indent+1);
    Green->PrintSyntaxTree(indent+1);
    Blue->PrintSyntaxTree(indent+1);
}

void WidthNode::PrintSyntaxTree(int indent) const {
    SourceNode::PrintSyntaxTree(indent);
    Width->PrintSyntaxTree(indent+1);
}
//==================================================
void OriginNode::Calculate(double *Result) const {
    Result[0] = X->Calculate();
    Result[1] = Y->Calculate();
}

void ScaleNode::Calculate(double *Result) const {
    Result[0] = X->Calculate();
    Result[1] = Y->Calculate();
}

void RotNode::Calculate(double *Result) const {
    Result[0] = Angle->Calculate();
}

void ForNode::Calculate(double *Result) const {
    //FOR T FROM (Start) TO (End) STEP (Step) DRAW(X,Y)
    //T += Step
    Result[0] = X->Calculate();
    Result[1] = Y->Calculate();
    ParmT->ParmValue += Step->Calculate();
    Result[2] = ParmT->ParmValue > End->Calculate() ? 0 : 1;
}

void ColorNode::Calculate(double *Result) const {
    Result[0]=Red->Calculate();
    Result[1]=Green->Calculate();
    Result[2]=Blue->Calculate();
}

void WidthNode::Calculate(double *Result) const {
    Result[0]=(int)Width->Calculate();
}