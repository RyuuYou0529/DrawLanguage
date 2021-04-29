//
// Created by RyuuYou on 2020/12/3.
//

#ifndef GRAPH_PARSER_HPP
#define GRAPH_PARSER_HPP

#include "../lexer/scanner.hpp"
#include "../tree/TreeNode.hpp"
#include "../error/exception.hpp"

typedef double (*FuncPtr)(double);

class Parser {
public:
    Lexer *theLexer{};
    ProgramNode *Root{};
    double Origin_X = 0, Origin_Y = 0;
    double Rot_Angle = 0;
    double Scale_X = 1, Scale_Y = 1;

    void Parser_exec(char *SrcFilePtr);

    void PrintSyntaxTree() const {
        if (this->Root != nullptr)
            this->Root->PrintSyntaxTree(0);
    }

private:
    void FetchToken() const;                                           //获取记号
    void MatchToken(enum Token_Type The_Token);                  //匹配终结符

    ProgramNode *Program();

    StatementNode *Statement();

    OriginNode *OriginStatement();

    RotNode *RotStatement();

    ScaleNode *ScaleStatement();

    ForNode *ForStatement();

    ColorNode *ColorStatement();

    WidthNode *WidthStatement();

    ExprNode *Expression();

    ExprNode *Term();

    ExprNode *Factor();

    ExprNode *Component();

    ExprNode *Atom();
};

#endif //GRAPH_PARSER_HPP
