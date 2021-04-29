//
// Created by RyuuYou on 2020/12/3.
//

#include "parser.hpp"
#include <iostream>
using namespace std;

//#define PARSER_DEBUG
#ifdef PARSER_DEBUG
#define enter(x) cout<<"enter in "<< x <<endl;
#else
#define enter(x)
#endif

#ifdef PARSER_DEBUG
#define back(x) cout<<"enter from "<< x <<endl;
#else
#define back(x)
#endif

#ifdef PARSER_DEBUG
#define call_match(x) cout<<"\tmatch token "<< x <<endl
#else
#define call_match(x)
#endif

ExprNode *ParmT;
static Token token;                    //当前记号
//通过词法分析器接口GetToken获取一个记号
void Parser::FetchToken() const
{
    token = theLexer->GetToken();
    if(token.type == ERRTOKEN) throw SyntaxError(theLexer->LineNo);
}

//匹配记号
void Parser::MatchToken(enum Token_Type The_Token)
{
    if(token.type != The_Token) throw UnexpectedSyntax(theLexer->LineNo);
    FetchToken();
}

void Parser::Parser_exec(char *SrcFilePtr){
    enter("Parser_exec")
    this->theLexer = new Lexer();
    if (!theLexer->InitScanner(SrcFilePtr))              //初始化词法分析器
    {
        printf("Open Source File Error!\n");
        return;
    }
    FetchToken();                                       //获取第一个记号
    this->Root = Program();                               //递归下降分析
    theLexer->CloseScanner();                           //关闭词法分析器
    back("Parser_exec")
}

//Program的递归子程序
/*
 * Program -> Statement SEMICO Program | ε
 */
ProgramNode* Parser::Program(){
    auto *tmpProg=new ProgramNode(nullptr, nullptr);
    ProgramNode *depth=tmpProg,*sub_depth;
    enter("Program");
    while(token.type != NONTOKEN){
        depth->Statement=Statement();
        depth->Program=new ProgramNode(nullptr, nullptr);
        sub_depth=depth;
        depth=depth->Program;
        MatchToken(SEMICO);
    }
    delete sub_depth->Program;
    sub_depth->Program=nullptr;
    back("Program");
    return tmpProg;
}

//Statement的递归子程序
/*
 * Statement -> OriginStatement|ScaleStatement|RotStatement|ForStatement|ColorStatement|WidthStatement
 */
StatementNode* Parser::Statement(){
    enter("Statement");
    StatementNode *tmpStat;
    switch(token.type)
    {
        case ORIGIN :   tmpStat=OriginStatement();      break;
        case SCALE :    tmpStat=ScaleStatement();       break;
        case ROT :      tmpStat=RotStatement();         break;
        case FOR :      tmpStat=ForStatement();         break;
        case COLOR:     tmpStat=ColorStatement();       break;
        case WIDTH:     tmpStat=WidthStatement();       break;
        default :       throw UnexpectedSyntax(theLexer->LineNo);
    }
    back("Statement");
    return tmpStat;
}

//OriginStatement的递归子程序
/*
 * OriginStatement -> ORIGIN IS (Expression,Expression);
 */
OriginNode* Parser::OriginStatement()
{
    ExprNode *X,*Y;
    enter("OriginStatement");
    //匹配终结符
    MatchToken(ORIGIN);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    X = Expression();//获取X位移
    MatchToken(COMMA);
    Y = Expression();//获取Y位移
    MatchToken(R_BRACKET);
    back("OriginStatement");
    return new OriginNode(X,Y);
}

//ScaleStatement的递归子程序
/*
 * ScaleStatement -> SCALE IS (Expression,Expression);
 */
ScaleNode* Parser::ScaleStatement()
{
    ExprNode *X,*Y;
    enter("ScaleStatement");
    MatchToken(SCALE);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    X = Expression();//获取X坐标
    MatchToken(COMMA);
    Y = Expression();//获取Y坐标
    MatchToken(R_BRACKET);
    back("ScaleStatement");
    return new ScaleNode(X,Y);
}

//RotStatement的递归子程序
/*
 * RotStatement -> ROT IS Expression;
 */
RotNode* Parser::RotStatement()
{
    ExprNode *Angel;
    enter("RotStatement");
    MatchToken(ROT);
    MatchToken(IS);
    Angel = Expression();
    back("RotStatement");
    return new RotNode(Angel);
}

//ForStatement的递归子程序
/*
 * ForStatement -> FOR T FROM Expression TO Expression STEP DRAW (Expression,Expression);
 */
ForNode* Parser::ForStatement(){
    ExprNode *Start, *End, *Step, *X, *Y;
    ParmNode *Parm;
    //各表达式语法树根节点指针
    enter("ForStatement");
    MatchToken(FOR);        call_match("FOR");

    MatchToken(T);          call_match("T");
    Parm=new ParmNode();
    ParmT=Parm;

    MatchToken(FROM);       call_match("FROM");
    Start = Expression();                    //构造参数起点表达式语法树
    Parm->ParmValue=Start->Calculate();

    MatchToken(TO);         call_match("TO");
    End = Expression();                     //构造参数终点表达式语法树

    MatchToken(STEP);       call_match("STEP");
    Step = Expression();                    //构造参数步长表达式语法树

    MatchToken(DRAW);       call_match("DRAW");
    MatchToken(L_BRACKET);  call_match("(");
    X = Expression();                        //构造横坐标表达式语法树

    MatchToken(COMMA);      call_match(",");
    Y = Expression();                        //构造纵坐标表达式语法树
    MatchToken(R_BRACKET);  call_match(")");
    back("ForStatement");
    return new ForNode(Parm,Start,End,Step,X,Y);
}

//ColorStatement的递归子程序
/*
 * ColorStatement -> COLOR IS (Expression,Expression,Expression);
 */
ColorNode* Parser::ColorStatement() {
    ExprNode *R,*G,*B;
    enter("ColorStatement");
    MatchToken(COLOR);
    MatchToken(IS);
    MatchToken(L_BRACKET);
    R=Expression();
    MatchToken(COMMA);
    G=Expression();
    MatchToken(COMMA);
    B=Expression();
    MatchToken(R_BRACKET);
    back("ColorStatement");
    return new ColorNode(R,G,B);
}

//WidthStatement的递归子程序
/*
 * WidthStatement -> WIDTH IS Expression;
 */
WidthNode* Parser::WidthStatement() {
    ExprNode *Width;
    enter("WidthStatement");
    MatchToken(WIDTH);
    MatchToken(IS);
    Width=Expression();
    back("WidthStatement");
    return new WidthNode(Width);
}

//Expression的递归子程序
ExprNode* Parser::Expression()
{
    ExprNode *left, *right;                             //左右子树节点的指针
    Token_Type token_tmp;                               //当前记号
    enter("Expression");
    left = Term();                                      //分析左操作数且得到其语法树
    while(token.type == PLUS || token.type == MINUS){
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Term();                                 //分析右操作数且得到其语法树
        switch (token_tmp) {                            //构造运算的语法树, 结果为左子树
            case PLUS:left=new PlusNode(left,right);break;
            case MINUS:left=new MinusNode(left,right);break;
        }

    }
    back("Expression");
    return left;                                        //返回最终表达式的语法树
}

// -------------Term的递归子程序
ExprNode* Parser::Term()
{
    ExprNode *left, *right;
    Token_Type token_tmp;
    left = Factor();
    while(token.type == MUL || token.type == DIV)
    {
        token_tmp = token.type;
        MatchToken(token_tmp);
        right = Factor();
        switch (token_tmp) {                            //构造运算的语法树, 结果为左子树
            case MUL:
                left=new MulNode(left,right);
                break;
            case DIV:
                if(right->Calculate()==0)
                    throw DivisorIsZero(theLexer->LineNo);
                left=new DivNode(left,right);
                break;
        }
    }
    return left;
}

//Factor的递归子程序
ExprNode* Parser::Factor(){
    ExprNode *node;
    if(token.type == PLUS)                        //匹配一元加运算
    {
        MatchToken(PLUS);
        node = Factor();                        //表达式退化为仅有右操作数的表达式
    }
    else if(token.type == MINUS)                //匹配一元减运算
    {
        MatchToken(MINUS);                        //表达式转化为二元减运算的表达式
        node = Factor();
        node = new MinusNode(new ConstNode(0.0),node);
    }
    else
        node = Component();                    //匹配非终结符Component
    return node;
}

//Comoenent的递归子程序
ExprNode* Parser::Component()
{
    ExprNode *left, *right;
    left = Atom();
    if(token.type == POWER)
    {
        MatchToken(POWER);
        right = Component();                    //递归调用Component以实现POWER的右集合
        left = new PowerNode(left,right);
    }
    return left;
}

//Atom的递归子程序
ExprNode* Parser::Atom()
{
    Token t = token;
    ExprNode *address, *tmp;
    string FuncName;
    switch(token.type)
    {
        case CONST_ID :
            MatchToken(CONST_ID);
            address = new ConstNode(t.value);
            break;
        case T :
            MatchToken(T);
            address = ParmT;
            break;
        case FUNC :
            FuncName=t.lexeme;
            MatchToken(FUNC);
            MatchToken(L_BRACKET);
            tmp = Expression();
            address = new FuncNode(tmp,t.FuncPtr,FuncName);
            MatchToken(R_BRACKET);
            break;
        case L_BRACKET :
            MatchToken(L_BRACKET);
            address = Expression();
            MatchToken(R_BRACKET);
            break;
        default :
            throw UnexpectedSyntax(theLexer->LineNo);
    }
    return address;
}
